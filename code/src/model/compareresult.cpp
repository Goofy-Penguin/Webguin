#include <mainframe/webguin/model/compareresult.h>
#include <mainframe/webguin/request/base.h>
#include <fmt/format.h>
#include <stdexcept>

namespace mainframe {
	namespace webguin {
		CompareResult::CompareResult(bool result_, const std::vector<std::string>& remainingPath_, const std::map<std::string, Param>& params_) : result(result_), remainingPath(remainingPath_), params(params_) {

		}

		void CompareResult::copyParams(std::shared_ptr<Request> req) const {
			for (auto& param : params) {
				req->setParam(param.first, param.second);
			}
		}

		bool CompareResult::getResult() const {
			return result;
		}

		void CompareResult::setResult(bool result_) {
			result = result_;
		}

		const std::vector<std::string>& CompareResult::getRemainingPath() const {
			return remainingPath;
		}

		void CompareResult::setRemainingPath(const std::vector<std::string>& remainingPath_) {
			remainingPath = remainingPath_;
		}

		const std::map<std::string, Param>& CompareResult::getParams() const {
			return params;
		}

		void CompareResult::addParam(const std::string& name, const Param& value) {
			if (!value.isValid()) {
				throw std::runtime_error(fmt::format("invalid value for param '{}' = '{}', should be type of {}", name, value.get<std::string>(), value.getTypeString()));
			}

			params[name] = value;
		}
	}
}