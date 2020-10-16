#pragma once

#include <string>
#include <mainframe/webguin/enum/pathparttype.h>
#include <mainframe/webguin/enum/paramtype.h>

namespace mainframe {
	namespace webguin {
		class PathPart {
			ParamType valueType = ParamType::none;
			PathPartType type = PathPartType::none;
			std::string value;
			std::string default_;
			bool defaultSet = false;

			bool checkForParam();
			bool checkForOptional();
			bool checkForWildcard();

			void extractInfo(const std::string& info);

		public:
			PathPart(const std::string& part);

			bool hasDefault() const;

			void setValue(const std::string& value_);
			const std::string& getValue() const;

			void setDefault(const std::string& value_);
			const std::string& getDefault() const;

			void setType(PathPartType type_);
			PathPartType getType() const;

			void setValueType(ParamType valueType);
			ParamType getValueType() const;
			std::string getValueTypeStr() const;

			bool compare(const std::string& path) const;
		};
	}
}
