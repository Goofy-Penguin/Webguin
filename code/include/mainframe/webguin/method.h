#pragma once

#include <string>
#include <memory>
#include <functional>

namespace mainframe {
	namespace webguin {
		class RequestBase;
		class ResponseBase;

		using MethodCallback = std::function<void(std::shared_ptr<RequestBase>, std::shared_ptr<ResponseBase>)>;

		class Method {
			std::string path;
			MethodCallback callback;

		public:
			Method(const std::string& path_);
			Method(const std::string& path_, MethodCallback callback_);

			int comparePath(const std::string& path_);

			void setPath(const std::string& path_);
			const std::string& getPath();

			virtual void execute(std::shared_ptr<RequestBase> request, std::shared_ptr<ResponseBase> response);
		};
	}
}
