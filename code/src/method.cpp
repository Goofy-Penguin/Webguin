#include <mainframe/webguin/method.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>
#include <mainframe/utils/string.h>

namespace mainframe {
	namespace webguin {
		Method::Method(MethodCallback callback_) : callback(callback_) {
		}

		void Method::setPath(const std::string& path_) {
			path = path_;

			parts.clear();
			for (auto& partStr : mainframe::utils::string::split(path, '/')) {
				parts.emplace_back(partStr);
			}
		}

		const std::string& Method::getPath() {
			return path;
		}

		CompareResult Method::comparePath(const std::vector<std::string>& pathParts) {
			CompareResult ret;

			size_t retIndex = 0;
			size_t maxparts = parts.size();

			for (size_t i = 0; i < maxparts; i++) {
				const auto& filter = parts[i];

				// if our path is longer than the request path and we're not using wildcard or optionals, then it's wrong
				auto filterType = filter.getType();
				if (pathParts.size() <= i) {
					if (filterType == PathPartType::optional) {
						if (filter.hasDefault()) {
							ret.addParam(filter.getValue(), {filter.getDefault(), filter.getValueType()});
						}
						break;
					} else {
						return ret;
					}
				}

				// run a check if our filter matches with the match
				const auto& match = pathParts[i];
				if (!filter.compare(match)) {
					return ret;
				}

				// populate params if needed and set the return position
				switch (filter.getType()) {
					case PathPartType::param:
					case PathPartType::optional:
						ret.addParam(filter.getValue(), {match, filter.getValueType()});
						[[fallthrough]];

					case PathPartType::match:
						retIndex = i + 1;
						break;

					default:
						break;
				}

				// if we're at the end of our filters, and there's still some part left. the remainer must be a wildcard
				if (i == maxparts - 1 && maxparts < pathParts.size() && filterType != PathPartType::wildcard) {
					return ret;
				}
			}

			// remove conditional parts of the path, to reduce duplicate mapping
			auto remainingPath = pathParts;
			remainingPath.erase(remainingPath.begin(), remainingPath.begin() + retIndex);

			ret.setRemainingPath(remainingPath);
			ret.setResult(true);
			return ret;
		}

		void Method::execute(const Request* request, Response* response) {
			if (callback == nullptr) {
				throw std::runtime_error("callback is nullptr");
			}

			callback(request, response);
		}
	}
}
