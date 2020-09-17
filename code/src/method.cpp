#include <mainframe/webguin/method.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>
#include <mainframe/utils/string.h>

namespace mainframe {
	namespace webguin {
		Method::Method(const std::string& path_) : path(path_) {
		}

		Method::Method(const std::string& path_, MethodCallback callback_) : path(path_), callback(callback_) {
		}

		void Method::setPath(const std::string& path_) {
			path = path_;
		}

		const std::string& Method::getPath() {
			return path;
		}

		int Method::comparePath(const std::string& path_) {
			auto& filter = getPath();

			auto wildcardPos = filter.find('*');
			if (wildcardPos == std::string::npos) {
				return path_ == filter ? 0 : -1;
			}

			if (path_.size() < filter.size()) {
				return -1;
			}

			size_t curpos = 0;
			size_t retpos = 0;
			bool wildcardFound = false;

			auto parts = mainframe::utils::string::split(filter, '/');
			for (auto& part : parts) {
				if (!wildcardFound) {
					retpos = curpos;
				}

				if (!wildcardFound && part.find('*') != std::string::npos) {
					wildcardFound = true;
				}

				if (curpos >= path_.size()) {
					return -1;
				}

				auto nextpos = path_.find('/', curpos + 1);
				if (nextpos == std::string::npos) {
					nextpos = path_.size();
				}

				if (part == "*") {
					curpos = nextpos;
					continue;
				}

				if (path_.substr(curpos, nextpos) != part) {
					return -1;
				}

				curpos = nextpos;
			}

			return retpos + 1;
		}

		void Method::execute(std::shared_ptr<RequestBase> request, std::shared_ptr<ResponseBase> response) {
			if (callback == nullptr) {
				throw std::runtime_error("callback is nullptr");
			}

			callback(request, response);
		}
	}
}