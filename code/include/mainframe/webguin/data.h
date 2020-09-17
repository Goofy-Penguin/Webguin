#pragma once

#include <map>
#include <string>

namespace mainframe {
	namespace webguin {
		class HttpData {
		public:
			std::string path;
			std::string data;
			std::string connectionIp;
			std::map<std::string, std::string> headers;
		};
	}
}
