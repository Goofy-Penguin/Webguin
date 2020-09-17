#pragma once

#include <mainframe/webguin/http/header.h>
#include <mainframe/webguin/http/cookie.h>
#include <mainframe/webguin/server.h>
#include <mainframe/webguin/client.h>
#include <mainframe/webguin/timings.h>

#include <mainframe/utils/event.hpp>
#include <nlohmann/json.hpp>
#include <map>
#include <thread>
#include <memory>
#include <mutex>

namespace mainframe {
	namespace webguin {
		class Client;

		class ResponseBase {
			int code = 400;
			std::string data;
			std::string contenttype;
			HttpTimings timings;
			std::vector<HttpHeader> headers;
			std::vector<HttpCookie> cookies;

		public:
			ResponseBase();

			void setCode(int code_);
			int getCode() const;

			const std::string& getData() const;

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