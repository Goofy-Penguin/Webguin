#include <mainframe/webguin/http/header.h>

namespace mainframe {
	namespace webguin {
		HttpHeader::HttpHeader(const std::string& name_, const std::string& value_) : name(name_), value(value_) {

		}

		void HttpHeader::setName(const std::string& name_) {
			name = name_;
		}

		void HttpHeader::setValue(const std::string& value_) {
			value = value_;
		}

		const std::string& HttpHeader::getName() const {
			return name;
		}

		const std::string& HttpHeader::getValue() const {
			return value;
		}
	}
}
