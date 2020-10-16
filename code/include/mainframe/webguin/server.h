#pragma once

#include <mainframe/webguin/controller.h>
#include <mainframe/webguin/method.h>
#include <mainframe/webguin/enum/methodtype.h>

#include <mainframe/networking/socket.h>
#include <nlohmann/json.hpp>
#include <map>
#include <thread>
#include <memory>
#include <mutex>

namespace mainframe {
	namespace webguin {
		class Client;
		class Request;
		class Response;

		class Server {
		public:

		private:
			std::thread* worker;
			mainframe::networking::Socket sockListener;
			std::mutex lock;
			std::vector<std::unique_ptr<mainframe::networking::Socket>> socks;
			std::map<std::thread*, std::unique_ptr<Client>> workerThreads;
			std::vector<std::unique_ptr<Method>> methods;
			std::vector<std::unique_ptr<Controller>> controllers;
			bool hitTheBrakes = false;
			bool verbose = false;

			void threadListener();
			void threadWorker(Client& threadData);
			void threadWorkerImpl(Client& threadData);
			std::string readHeaders(Client& threadData);
			bool readHttpHeader(Client& threadData, const std::string& rawheaders);
			void processHeaders(Client& threadData, const std::string& rawheaders);
			bool readHttpBody(Client& threadData);
			void handleCallback(Client& threadData, std::string& headersstr);
			bool writeResponse(Client& threadData, const std::string& headersstr);
			bool HandleRequest(Client& threadData);

			MethodType createMethodFromString(const std::string& method);

		public:
			Server();
			~Server();

			void setVerbose(bool mode);

			template<typename T = Method, typename... TArgs, std::enable_if_t<std::is_base_of<Method, T>::value>* = nullptr>
			T& addMethod(const std::string& path, TArgs... args) {
				auto method = std::make_unique<T>(args...);
				method->setPath(path);

				auto& ptr = *method;
				methods.push_back(std::move(method));
				return ptr;
			}

			template<typename T = Controller, typename... TArgs, std::enable_if_t<std::is_base_of<Controller, T>::value>* = nullptr>
			T& addController(const std::string& path, TArgs... args) {
				auto controller = std::make_unique<T>(args...);
				controller->setPath(path);

				auto& ptr = *controller;
				controllers.push_back(std::move(controller));
				return ptr;
			}

			std::vector<Controller*> getControllers();
			std::vector<Method*> getMethods();

			virtual std::unique_ptr<Request> createRequest() const;
			virtual std::unique_ptr<Response> createResponse() const;

			virtual void printVerbose(const Client& threadData) const;
			virtual bool host(int port);
			virtual bool onRequest(Client& handler);
		};
	}
}
