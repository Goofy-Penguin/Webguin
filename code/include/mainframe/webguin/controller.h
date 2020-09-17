#pragma once

#include <string>
#include <mainframe/webguin/method.h>
#include <mainframe/webguin/client.h>

namespace mainframe {
	namespace webguin {
		class Controller : public Method {
			std::vector<std::shared_ptr<Method>> methods;
			std::vector<std::shared_ptr<Controller>> controllers;

		public:
			Controller(const std::string& path);

			void addMethod(std::shared_ptr<Method> method);
			void addController(std::shared_ptr<Controller> controller);

			virtual void execute(std::shared_ptr<RequestBase> request, std::shared_ptr<ResponseBase> response) override;
			virtual bool onRequest(Client& handler, const std::string& remainerPath);
		};
	}
}
