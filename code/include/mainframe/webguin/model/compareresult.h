#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mainframe/webguin/model/param.h>

namespace mainframe {
	namespace webguin {
		class Request;

		class CompareResult {
			bool result = false;
			std::vector<std::string> remainingPath;
			std::map<std::string, Param> params;

		public:
			CompareResult() = default;
			CompareResult(bool result_, const std::vector<std::string>& remainingPath_, const std::map<std::string, Param>& params_);

			bool getResult() const;
			void setResult(bool result_);

			const std::vector<std::string>& getRemainingPath() const;
			void setRemainingPath(const std::vector<std::string>& remainingPath_);

			const std::map<std::string, Param>& getParams() const;
			void addParam(const std::string& name, const Param& value);

			void CompareResult::copyParams(std::shared_ptr<Request> req) const;
		};
	}
}
