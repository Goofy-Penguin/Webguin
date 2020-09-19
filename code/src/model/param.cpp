#include <mainframe/webguin/model/param.h>

namespace mainframe {
	namespace webguin {
		Param::Param(const std::string& value, ParamType _type) : raw(value), type(_type) {}
		Param::Param(const Param& value) : raw(value.raw), type(value.type) {}

		ParamType Param::getType() const {
			return type;
		}

		std::string Param::getTypeString() const {
			switch (type) {
				case ParamType::int_: return "int";
				case ParamType::bool_: return "bool";
				case ParamType::long_: return "long";
				case ParamType::float_: return "float";
				case ParamType::number: return "number";
				case ParamType::string: return "string";
				case ParamType::double_: return "double";
				default: return "none";
			}
		}

		bool Param::isValid() const {
			switch (type) {
				case ParamType::bool_: return "bool";
				case ParamType::long_: return "long";
				case ParamType::string: return true;

				case ParamType::int_:
					return !raw.empty() && std::find_if(raw.begin(), raw.end(), [](unsigned char c) { return !std::isdigit(c); }) == raw.end();

				case ParamType::number:
				case ParamType::float_:
				case ParamType::double_:
					return !raw.empty() && std::find_if(raw.begin(), raw.end(), [](unsigned char c) { return !std::isdigit(c) && c != '.'; }) == raw.end();

				default: return false;
			}
		}
	}
}
