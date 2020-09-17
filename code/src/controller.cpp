#include <mainframe/webguin/controller.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

namespace mainframe {
	namespace webguin {
		Controller::Controller(const std::string& path) : Method(path) {
		}

		void Controller::execute(std::shared_ptr<RequestBase> request, std::shared_ptr<ResponseBase> response) {

		}

		bool Controller::onRequest(Client& handler, const std::string& remainerPath) {
			for (auto& method : methods) {
				int lastFound = method->comparePath(remainerPath);
				if (lastFound == -1) {
					continue;
				}

				method->execute(handler.request, handler.response);
				return true;
			}

			for (auto& controller : controllers) {
				int lastFound = controller->comparePath(remainerPath);
				if (lastFound == -1) {
					continue;
				}

				return controller->onRequest(handler, remainerPath.substr(lastFound + 1));
			}

			execute(handler.request, handler.response);
			return true;
		}

		void Controller::addMethod(std::shared_ptr<Method> method) {
			methods.push_back(method);
		}

		void Controller::addController(std::shared_ptr<Controller> controller) {
			controllers.push_back(controller);
		}
	}
}