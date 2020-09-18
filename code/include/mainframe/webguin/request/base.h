#pragma once

#include <mainframe/webguin/enum/methodtype.h>
#include <mainframe/webguin/model/http/header.h>
#include <mainframe/webguin/model/http/cookie.h>
#include <mainframe/webguin/model/param.h>

#include <mainframe/utils/event.hpp>
#include <nlohmann/json.hpp>
#include <map>
#include <thread>
#include <memory>
#include <mutex>

namespace mainframe {
	namespace webguin {
		class Client;

		class Request {
			std::string data;
			std::string path;
			std::string contenttype;
			std::vector<HttpHeader> headers;
			std::vector<HttpCookie> cookies;
			std::map<std::string, Param> params;
			MethodType method;

		public:
			Request();

			void setData(const std::string& data_);
			const std::string& getData() const;

			void setMethod(MethodType method_);
			MethodType getMethod() const;

			void setPath(const std::string& path_);
			const std::string& getPath() const;

			bool hasParam(const std::string& name) const;
			const Param& getParam(const std::string& name) const;

			Param getParam(const std::string& name, Param default_) const;
			void setParam(const std::string& name, const Param& value);
			void clearParams();

			void addHeader(const HttpHeader& header_);
			void setHeader(const HttpHeader& header_);
			bool hasHeader(const std::string& name) const;
			const HttpHeader& getHeader(const std::string& name) const;

			void addCookie(const HttpCookie& cookie_);
			void setCookie(const HttpCookie& cookie_);
			bool hasCookie(const std::string& name) const;
			const HttpCookie& getCookie(const std::string& name) const;
		};
	}
}