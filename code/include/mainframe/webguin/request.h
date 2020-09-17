#pragma once

#include <mainframe/webguin/http/cookie.h>
#include <mainframe/webguin/data.h>
#include <vector>

namespace mainframe {
	namespace webguin {
		class HttpRequest : public HttpData {
		public:
			std::vector<HttpCookie> cookies;
			std::string raw;
			std::string type;
		};
	}
}
