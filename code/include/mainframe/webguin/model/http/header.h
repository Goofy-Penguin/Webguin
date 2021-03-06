﻿#pragma once

#include <string>

namespace mainframe {
	namespace webguin {
		class HttpHeader {
			std::string name;
			std::string value;

		public:
			HttpHeader(const std::string& name_, const std::string& value_);

			void setName(const std::string& name_);
			void setValue(const std::string& value_);

			const std::string& getName() const;
			const std::string& getValue() const;
		};
	}
}
