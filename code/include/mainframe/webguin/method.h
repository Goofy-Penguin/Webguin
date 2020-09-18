#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <mainframe/webguin/model/pathpart.h>
#include <mainframe/webguin/model/compareresult.h>

namespace mainframe {
	namespace webguin {
		class Request;
		class Response;

		using MethodCallback = std::function<void(std::shared_ptr<const Request>, std::shared_ptr<Response>)>;

		class Method {
			std::string path;
			std::vector<PathPart> parts;
			MethodCallback callback;

		public:
			Method() = default;
			Method(MethodCallback callback_);

			CompareResult comparePath(const std::vector<std::string>& pathParts);

			void setPath(const std::string& path_);
			const std::string& getPath();

			virtual void execute(std::shared_ptr<const Request> request, std::shared_ptr<Response> response);
		};
	}
}
