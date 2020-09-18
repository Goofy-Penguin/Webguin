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
			template<typename T = Method, typename... TArgs, std::enable_if_t<std::is_base_of<Method, T>::value>* = nullptr>
			std::shared_ptr<T> addMethod(const std::string& path, TArgs... args) {
				auto method = std::make_shared<T>(args...);
				method->setPath(path);

				methods.push_back(method);
				return method;
			}

			template<typename T = Controller, typename... TArgs, std::enable_if_t<std::is_base_of<Controller, T>::value>* = nullptr>
			std::shared_ptr<T> addController(const std::string& path, TArgs... args) {
				auto controller = std::make_shared<T>(args...);
				controller->setPath(path);

				controllers.push_back(controller);
				return controller;
			}

			virtual void execute(std::shared_ptr<const Request> request, std::shared_ptr<Response> response) override;
			virtual bool onRequest(Client& handler, const std::vector<std::string>& path);
		};
	}
}
