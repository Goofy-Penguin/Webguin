#pragma once

#include <mainframe/webguin/model/http/header.h>
#include <mainframe/webguin/model/http/cookie.h>
#include <mainframe/webguin/model/timings.h>
#include <mainframe/webguin/enum/httpcode.h>

#include <mainframe/utils/event.hpp>
#include <nlohmann/json.hpp>
#include <map>
#include <thread>
#include <memory>
#include <mutex>

namespace mainframe {
	namespace webguin {
		class Client;

		class Response {
			HttpCode code = HttpCode::none;
			std::string data;
			std::string contenttype;
			HttpTimings timings;
			std::vector<HttpHeader> headers;
			std::vector<HttpCookie> cookies;

		public:
			Response();
			virtual ~Response() = default;

			void setCode(HttpCode code_);
			HttpCode getCode() const;

			const std::string& getData() const;

			void clearHeaders();
			void addHeader(const HttpHeader& header_);
			void setHeader(const HttpHeader& header_);
			bool hasHeader(const std::string& name) const;
			const HttpHeader& getHeader(const std::string& name) const;

			void addCookie(const HttpCookie& cookie_);
			void setCookie(const HttpCookie& cookie_);
			bool hasCookie(const std::string& name) const;
			const HttpCookie& getCookie(const std::string& name) const;

			virtual void setData(const std::string& data_);
			virtual bool writeHeaders(Client& client) const;
			virtual bool writeBody(Client& client) const;
		};
	}
}
