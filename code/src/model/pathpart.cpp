#include <mainframe/webguin/model/pathpart.h>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace mainframe {
	namespace webguin {
		std::vector<std::string> split(const std::string& haystack, const std::vector<std::string>& needles) {
			std::vector<std::string> ret;

			size_t oldpos = 0;
			while (true) {
				size_t closestpos = std::string::npos;
				size_t needlesize = 0;
				for (auto& needle : needles) {
					auto nextpos = haystack.find(needle, oldpos);

					if (nextpos == std::string::npos) continue;
					if (closestpos < nextpos) continue;

					closestpos = nextpos;
					needlesize = needle.size();
				}

				if (closestpos == std::string::npos) break;

				ret.emplace_back(haystack.begin() + oldpos, haystack.begin() + closestpos);
				closestpos += needlesize;
				oldpos = closestpos;
			}

			if (oldpos != haystack.size()) {
				ret.emplace_back(haystack.begin() + oldpos, haystack.end());
			}

			return ret;
		}

		void PathPart::extractInfo(const std::string& info) {
			auto pieces = split(info, {":", "="});

			std::string typeStr = "string";

			if (pieces.size() > 0) value = pieces[0];
			if (pieces.size() > 1) typeStr = pieces[1];
			for (size_t i = 2; i < pieces.size(); i++) {
				if (!default_.empty()) default_ += "=";
				default_ += pieces[i];
				defaultSet = true;
			}

			if (typeStr == "int") valueType = ParamType::int_;
			else if (typeStr == "long") valueType = ParamType::long_;
			else if (typeStr == "float") valueType = ParamType::float_;
			else if (typeStr == "number") valueType = ParamType::number;
			else if (typeStr == "string") valueType = ParamType::string;
			else if (typeStr == "double") valueType = ParamType::double_;
			else throw std::runtime_error("invalid type for param");
		}

		PathPart::PathPart(const std::string& part) {
			value = part;

			if (!checkForParam() && !checkForOptional() && !checkForWildcard()) {
				setType(PathPartType::match);
			}

			extractInfo(value);
		}

		bool PathPart::checkForWildcard() {
			if (value != "*") {
				return false;
			}

			setType(PathPartType::wildcard);
			return true;
		}

		bool PathPart::checkForOptional() {
			if (value.size() < 3 || value.front() != '[' || value.back() != ']') {
				return false;
			}

			setType(PathPartType::optional);
			setValue(value.substr(1, value.size() - 2));
			return true;
		}

		bool PathPart::checkForParam() {
			if (value.size() < 3 || value.front() != '{' || value.back() != '}') {
				return false;
			}

			setType(PathPartType::param);
			setValue(value.substr(1, value.size() - 2));
			return true;
		}

		void PathPart::setValue(const std::string& value_) {
			value = value_;
		}

		const std::string& PathPart::getValue() const {
			return value;
		}

		void PathPart::setDefault(const std::string& value_) {
			default_ = value_;
			defaultSet = true;
		}

		const std::string& PathPart::getDefault() const {
			return default_;
		}

		bool PathPart::hasDefault() const {
			return defaultSet;
		}

		void PathPart::setValueType(ParamType type_) {
			valueType = type_;
		}

		ParamType PathPart::getValueType() const {
			return valueType;
		}

		void PathPart::setType(PathPartType type_) {
			type = type_;
		}

		PathPartType PathPart::getType() const {
			return type;
		}

		std::string PathPart::getValueTypeStr() const {
			switch (valueType) {
				case ParamType::bool_: return "bool";
				case ParamType::double_: return "double";
				case ParamType::float_: return "float";
				case ParamType::int_: return "int";
				case ParamType::long_: return "long";
				case ParamType::number: return "number";
				case ParamType::string: return "string";

				default:
					return "";
			}
		}


		bool PathPart::compare(const std::string& path) const {
			switch (type) {
				case PathPartType::param:
				case PathPartType::optional:
				case PathPartType::wildcard:
					return true;

				case PathPartType::match: {
					std::string lowerpath = path;
					std::transform(lowerpath.begin(), lowerpath.end(), lowerpath.begin(), ::tolower);

					return lowerpath == value;
				}

				default:
					return false;
			}
		}
	}
}
