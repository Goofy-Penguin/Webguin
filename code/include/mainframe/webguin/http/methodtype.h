#pragma once

#include <string>

namespace mainframe {
	namespace webguin {
		enum class MethodType {
			none,
			get,
			post,
			put,
			delete_,
			options
		};
	}
}