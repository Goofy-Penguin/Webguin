 #pragma once

#include <thread>
#include <memory>
#include <mainframe/networking/socket.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>
#include <mainframe/webguin/timings.h>

namespace mainframe {
	namespace webguin {
		class RequestBase;
		class ResponseBase;

		class Client {
		public:
			HttpTimings timings;
			std::shared_ptr<RequestBase> request;
			std::shared_ptr<ResponseBase> response;
			std::shared_ptr<mainframe::networking::Socket> sock;
			std::thread* thread = nullptr;

			Client() = default;

			bool send(const std::string& buffer);
		};
	}
}
