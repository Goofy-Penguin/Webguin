#include <mainframe/webguin/controller.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

namespace mainframe {
	namespace webguin {
		void Controller::execute(std::shared_ptr<const Request> request, std::shared_ptr<Response> response) {

		}

		bool Controller::onRequest(Client& handler, const std::vector<std::string>& path) {
			for (auto& method : methods) {
				auto ret = method->comparePath(path);
				if (!ret.getResult()) {
					continue;
				}

				ret.copyParams(handler.request);
				method->execute(handler.request, handler.response);
				return true;
			}

			for (auto& controller : controllers) {
				auto ret = controller->comparePath(path);
				if (!ret.getResult()) {
					continue;
				}

				ret.copyParams(handler.request);
				return controller->onRequest(handler, ret.getRemainingPath());
			}

			execute(handler.request, handler.response);
			return true;
		}
	}
}
