#include <thread>
#include <filesystem>
#include <atomic>

#include <mainframe/webguin/server.h>
#include <mainframe/webguin/client.h>
#include <mainframe/webguin/method.h>
#include <mainframe/webguin/controller.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>
#include <mainframe/networking/socket.h>
#include <mainframe/utils/thread.h>
#include <mainframe/utils/string.h>
#include <mainframe/utils/time.h>

#include <fmt/printf.h>
#include <fmt/format.h>

#ifndef WIN32
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#define DEBUG


namespace mainframe {
	namespace webguin {
		Server::Server() {
			sockListener.create();

			worker = new std::thread([this] {
				threadListener();
			});

			const size_t threads = 4;

			std::lock_guard<std::mutex> lockGuard(lock);
			for (size_t i = 0; i < threads; i++) {
				auto client = std::make_unique<Client>();
				auto clientPtr = client.get();
				std::thread* thread = new std::thread([this, clientPtr] {
					threadWorker(*clientPtr);
				});

				client->thread = thread;
				workerThreads[thread] = std::move(client);
			}
		}

		Server::~Server() {
			hitTheBrakes = true;
			sockListener.close();

			if (worker != nullptr) {
				if (worker->joinable()) worker->join();
				delete worker;
			}

			std::lock_guard<std::mutex> lockGuard(lock);
			for (auto& pair : workerThreads) {
				auto& t = pair.second->thread;

				if (t->joinable()) t->join();
				delete t;
			}

			workerThreads.clear();
		}

		void Server::threadListener() {
			while (!hitTheBrakes) {
				auto clientsock = std::make_unique<mainframe::networking::Socket>();
				if (!sockListener.accept(clientsock.get())) {
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					continue;
				}

				std::lock_guard<std::mutex> lockGuard(lock);
				socks.push_back(std::move(clientsock));
			}
		}

		void Server::setVerbose(bool mode) {
			verbose = mode;
		}

		std::string Server::readHeaders(Client& threadData) {
			auto& req = threadData.request;

			std::string ret;
			ret.reserve(4096);

			while (true) {
				size_t size = ret.size();
				const char headerspliter[] = "\r\n\r\n";
				if (size >= sizeof(headerspliter)) {
					size_t pos = size - sizeof(headerspliter) + 1;
					if (ret[pos + 0] == '\r' &&
						ret[pos + 1] == '\n' &&
						ret[pos + 2] == '\r' &&
						ret[pos + 3] == '\n') {

						break;
					}
				}

				unsigned char tmpbuff = 0;
				if (threadData.sock->receive(&tmpbuff, 1) <= 0) {
					return {};
				}

				ret.push_back(tmpbuff);
			}

			return ret;
		}

		bool Server::readHttpHeader(Client& threadData, const std::string& rawheaders) {
			auto& req = threadData.request;

			auto reqtypepos = rawheaders.find(' ');
			if (reqtypepos == std::string::npos) {
				return false;
			}

			auto method = rawheaders.substr(0, rawheaders.find(' '));
			req->setMethod(this->createMethodFromString(method));


			auto reqpathpos = rawheaders.find("HTTP");
			if (reqpathpos == std::string::npos) {
				return false;
			}

			std::string path = rawheaders.substr(method.size() + 1, reqpathpos - method.size() - 2);
			if (path.size() > 0 && path[0] == '/') {
				path = path.substr(1);
			}

			req->setPath(path);
			return true;
		}

		MethodType Server::createMethodFromString(const std::string& method) {
			auto lowered = method;
			std::transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);

			if (lowered == "get") return MethodType::get;
			if (lowered == "put") return MethodType::put;
			if (lowered == "post") return MethodType::post;
			if (lowered == "delete") return MethodType::delete_;
			if (lowered == "options") return MethodType::options;

			return MethodType::none;
		}

		std::unique_ptr<Request> Server::createRequest() const {
			return std::make_unique<Request>();
		}

		std::unique_ptr<Response> Server::createResponse() const {
			return std::make_unique<Response>();
		}

		void Server::processHeaders(Client& threadData, const std::string& rawheaders) {
			auto& req = threadData.request;

			// deal with headers
			auto headerlines = mainframe::utils::string::split(rawheaders, '\r');
			for (const auto& line : headerlines) {
				auto parts = mainframe::utils::string::split(line, ':');
				if (parts.size() < 2) continue;

				std::string name = parts[0];
				std::string value;
				std::transform(name.begin(), name.end(), name.begin(), ::tolower);

				for (size_t i = 1; i < parts.size(); i++) {
					if (i > 1) {
						value += ":";
						value += parts[i];
					} else {
						value += parts[i].c_str() + 1;
					}
				}

				if (name.size() == 0) continue;
				if (name[0] == '\n') name = name.c_str() + 1;

				req->addHeader({name, value});
			}
		}

		bool Server::readHttpBody(Client& threadData) {
			auto& req = threadData.request;

			if (!req->hasHeader("content-length") && (req->getMethod() == MethodType::post || req->getMethod() == MethodType::put)) {
				return false;
			}

			// process request data
			int datalength = 0;
			if (req->hasHeader("content-length")) {
				datalength = std::stoi(req->getHeader("content-length").getValue());
			}

			std::string body;
			body.resize(datalength);

			int datacurpos = 0;
			while (datacurpos != datalength) {
				int ret = threadData.sock->receive(reinterpret_cast<unsigned char*>(&body[0] + datacurpos), datalength - datacurpos);
				if (ret <= 0) break;

				datacurpos += ret;
			}

			if (datacurpos != datalength) {
				return false;
			}

			req->setData(body);
			return true;
		}

		void Server::handleCallback(Client& client, std::string& headersstr) {
			auto& req = client.request;
			auto& resp = client.response;

			if (req->getMethod() == MethodType::options) {
				headersstr += "HTTP/1.1 200\r\n";
				headersstr += "Access-Control-Allow-Methods: *\r\n";
				headersstr += "Access-Control-Allow-Headers: *\r\n";
				headersstr += "Access-Control-Max-Age: 1728000\r\n";
			} else {
#ifndef DEBUG
				try {
#endif
					if (!onRequest(client)) {
						client.sock->close();
						return;
					}
#ifndef DEBUG
				} catch (const std::exception& err) {
					fmt::print(" worker error {}: {}\n", req->getPath(), err.what());
				}
#endif

				headersstr += fmt::format("HTTP/1.1 {}\r\n", resp->getCode());
			}
		}

		void Server::printVerbose(const Client& threadData) const {}

		bool Server::HandleRequest(Client& threadData) {
			threadData.request = this->createRequest();
			threadData.response = this->createResponse();

			auto& req = threadData.request;
			auto& resp = threadData.response;
			auto& timings = threadData.timings;

			timings.readStart = static_cast<time_t>(mainframe::utils::time::getMS());

			auto headerString = readHeaders(threadData);
			if (!readHttpHeader(threadData, headerString)) return false;

			processHeaders(threadData, headerString);
			if (!readHttpBody(threadData)) return false;

			timings.readEnd = timings.processingStart = static_cast<time_t>(mainframe::utils::time::getMS());

			// handle response callback
			std::string headersstr;
			handleCallback(threadData, headersstr);

			timings.processingEnd = timings.writeStart = static_cast<time_t>(mainframe::utils::time::getMS());

			// generate response

			if (!resp->writeHeaders(threadData)) return false;
			if (!resp->writeBody(threadData)) return false;

			timings.writeEnd = static_cast<time_t>(mainframe::utils::time::getMS());

			if (verbose) printVerbose(threadData);
			return true;
		}

		void Server::threadWorker(Client& threadData) {
			static std::atomic<int> id = 1;
			auto getId = [&]() {
				return id++;
			};

			mainframe::utils::thread::setName("HTTP_WORKER " + std::to_string(getId()));

			while (!hitTheBrakes) {
#ifndef DEBUG
				try {
#endif
					threadWorkerImpl(threadData);
#ifndef DEBUG
				} catch (std::exception err) {
					fmt::print("worker error: {}\n", err.what());
				}
#endif
			}
		}

		void Server::threadWorkerImpl(Client& threadData) {
			while (!hitTheBrakes) {
				threadData.sock = nullptr;

				{
					std::lock_guard<std::mutex> lockGuard(lock);
					if (socks.empty()) {
						continue;
					}

					threadData.sock = std::move(socks.front());
					socks.erase(socks.begin());
				}

				if (threadData.sock == nullptr) {
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					continue;
				}

				threadData.sock->setTimeout(5000);

#ifndef DEBUG
				try {
#endif
					while (HandleRequest(threadData)) {
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
#ifndef DEBUG
				} catch (std::exception err) {
					fmt::print("worker error in request: {}\n", err.what());
				}
#endif
			}
		}

		bool Server::onRequest(Client& handler) {
			auto path = mainframe::utils::string::split(handler.request->getPath(), '/');

			for (auto& method : methods) {
				auto ret = method->comparePath(path);
				if (!ret.getResult()) {
					continue;
				}

				ret.copyParams(handler.request.get());
				method->execute(handler.request.get(), handler.response.get());
				return true;
			}

			for (auto& controller : controllers) {
				auto ret = controller->comparePath(path);
				if (!ret.getResult()) {
					continue;
				}

				ret.copyParams(handler.request.get());
				return controller->onRequest(handler, ret.getRemainingPath());
			}

			return false;
		}

		bool Server::host(int port) {
			return sockListener.bind(port) && sockListener.listen();
		}
	}
}
