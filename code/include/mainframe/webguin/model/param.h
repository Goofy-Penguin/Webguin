#pragma once

#include <mainframe/webguin/enum/paramtype.h>
#include <string>

namespace mainframe {
	namespace webguin {
		class Param {
		private:
			std::string raw;
			ParamType type = ParamType::none;

		public:
			operator char() const { return raw[0]; }
			operator unsigned char() const { return static_cast<unsigned char>(raw[0]); }

			operator short() const { return static_cast<short>(std::stoi(raw)); }
			operator unsigned short() const { return static_cast<unsigned short>(std::stoul(raw)); }

			operator int() const { return std::stoi(raw); }
			operator unsigned int() const { return static_cast<unsigned int>(std::stoul(raw)); }

			operator long() const { return std::stol(raw); }
			operator unsigned long() const { return static_cast<unsigned int>(std::stoul(raw)); }

			operator long long() const { return std::stoll(raw); }
			operator unsigned long long() const { return std::stoull(raw); }

			operator float() const { return std::stof(raw); }
			operator double() const { return std::stod(raw); }
			operator long double() const { return std::stold(raw); }

			operator std::string () const { return raw; }
			operator const std::string& () const { return raw; }
			operator bool() const { return std::stoll(raw) > 0; }

			template<class T>
			T get() const {
				return *this;
			}

			Param() = default;
			~Param() = default;

			Param(const std::string& value, ParamType _type);
			Param(const Param& value);

			bool isValid() const;
			ParamType getType() const;
			std::string getTypeString() const;
		};
	}
}
