#include <mainframe/webguin/client.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

#include <fmt/printf.h>
#include <fmt/format.h>

namespace mainframe {
	namespace webguin {
		bool Client::send(const std::string& buffer) {
			auto dataptr = reinterpret_cast<const unsigned char*>(buffer.c_str());
			int tosend = static_cast<int>(buffer.size());
			int curpos = 0;

			while (curpos != tosend) {
				int ret = sock->send(dataptr + curpos, tosend - curpos);

				if (ret <= 0) break;
				curpos += ret;
			}

			return curpos == tosend;
		}
	}
}