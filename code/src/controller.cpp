#include <mainframe/webguin/controller.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

namespace mainframe {
	namespace webguin {
		void Controller::execute(const Request* request, Response* response) {

		}

		bool Controller::onRequest(Client& handler, const std::vector<std::string>& path) {
			for (auto& method : methods) {
				auto ret = method->comparePath(path);
				if (!ret.getResult() || !method->check(handler.request.get())) {
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

			execute(handler.request.get(), handler.response.get());
			return true;
		}

		std::vector<Controller*> Controller::getControllers() {
			std::vector<Controller*> ret;

			for (auto& c : controllers) {
				ret.push_back(c.get());
			}

			return ret;
		}

		std::vector<Method*> Controller::getMethods() {
			std::vector<Method*> ret;

			for (auto& m : methods) {
				ret.push_back(m.get());
			}

			return ret;
		}
	}
}
