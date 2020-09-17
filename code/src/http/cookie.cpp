#include <mainframe/webguin/http/cookie.h>

namespace mainframe {
	namespace webguin {
		HttpCookie::HttpCookie(const std::string& name_, const std::string& value_) : name(name_), value(value_) {

		}

		void HttpCookie::setName(const std::string& name_) {
			name = name_;
		}

		void HttpCookie::setValue(const std::string& value_) {
			value = value_;
		}

		const std::string& HttpCookie::getName() const {
			return name;
		}

		const std::string& HttpCookie::getValue() const {
			return value;
		}
	}
}
