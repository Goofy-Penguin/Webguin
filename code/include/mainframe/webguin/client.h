 #pragma once

#include <thread>
#include <memory>
#include <mainframe/networking/socket.h>
#include <mainframe/webguin/model/timings.h>

namespace mainframe {
	namespace webguin {
		class Request;
		class Response;

		class Client {
		public:
			HttpTimings timings;
			std::unique_ptr<Request> request;
			std::unique_ptr<Response> response;
			std::unique_ptr<mainframe::networking::Socket> sock;
			std::thread* thread = nullptr;

			Client() = default;

			bool send(const std::string& buffer);
		};
	}
}
