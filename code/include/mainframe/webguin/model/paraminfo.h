#pragma once

#include <mainframe/webguin/enum/paramtype.h>
#include <string>

namespace mainframe {
	namespace webguin {
		class ParamInfo {
		public:
			std::string name;
			std::string default_;
			ParamType type;
		};
	}
}
