#pragma once

#include <string>
#include <mainframe/webguin/method.h>
#include <mainframe/webguin/client.h>

namespace mainframe {
	namespace webguin {
		class Controller : public Method {
			std::vector<std::unique_ptr<Method>> methods;
			std::vector<std::unique_ptr<Controller>> controllers;

		public:
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

			virtual void execute(const Request* request, Response* response) override;
			virtual bool onRequest(Client& handler, const std::vector<std::string>& path);
		};
	}
}
