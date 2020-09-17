#pragma once

#include <mainframe/webguin/data.h>
#include <mainframe/webguin/timings.h>
#include <vector>

namespace mainframe {
	namespace webguin {
		class HttpResponse : public HttpData {
		public:
			int code = 400;
			std::string data;
			std::string contenttype;
			HttpTimings timings;
		};
	}
}
