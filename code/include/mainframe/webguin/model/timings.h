#pragma once

#include <ctime>

namespace mainframe {
	namespace webguin {
		class HttpTimings {
		public:
			time_t readStart = 0;
			time_t writeStart = 0;
			time_t processingStart = 0;

			time_t readEnd = 0;
			time_t writeEnd = 0;
			time_t processingEnd = 0;
		};
	}
}
