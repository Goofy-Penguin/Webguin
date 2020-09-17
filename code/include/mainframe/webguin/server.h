#pragma once

#include <mainframe/webguin/http/methodtype.h>
#include <mainframe/networking/socket.h>
#include <nlohmann/json.hpp>
#include <map>
#include <thread>
#include <memory>
#include <mutex>

namespace mainframe {
	namespace webguin {
		class Client;
		class Method;
		class Controller;
		class RequestBase;
		class ResponseBase;

		class Server {
		public:

		private:
			std::thread* worker;
			mainframe::networking::Socket sockListener;
			std::mutex lock;
			std::vector<std::shared_ptr<mainframe::networking::Socket>> socks;
			std::map<std::thread*, std::shared_ptr<Client>> workerThreads;
			std::vector<std::shared_ptr<Method>> methods;
			std::vector<std::shared_ptr<Controller>> controllers;
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

			void addMethod(std::shared_ptr<Method> method);
			void addController(std::shared_ptr<Controller> controller);

			virtual std::shared_ptr<RequestBase> createRequest() const;
			virtual std::shared_ptr<ResponseBase> createResponse() const;

			virtual void printVerbose(const Client& threadData) const;
			virtual bool host(int port);
			virtual bool onRequest(Client& handler);
		};
	}
}