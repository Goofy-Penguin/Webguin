#pragma once

namespace mainframe {
	namespace webguin {
		enum class PathPartType {
			none,
			match,
			param,
			optional,
			wildcard
		};
	}
}