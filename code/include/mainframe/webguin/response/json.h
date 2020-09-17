#pragma once

#include <mainframe/webguin/response/base.h>

namespace mainframe {
	namespace webguin {
		class ResponseJson : public ResponseBase {
		public:
			ResponseJson();

			virtual bool writeHeaders(mainframe::networking::Socket& socket) const override;
			virtual bool writeBody(mainframe::networking::Socket& socket) const override;
		};
	}
}