#include <mainframe/webguin/response/base.h>
#include <mainframe/webguin/client.h>
#include <fmt/format.h>

namespace mainframe {
	namespace webguin {
		Response::Response() {
			addHeader({"content-type", "text/plain"});
			addHeader({"content-length", std::to_string(data.size())});
			addHeader({"Access-Control-Allow-Origin", "*"});
			addHeader({"server", "webguin"});
			addHeader({"connection", "close"});
		}

		void Response::setCode(HttpCode code_) {
			code = code_;
		}

		HttpCode Response::getCode() const {
			return code;
		}


		void Response::setData(const std::string& data_) {
			data = data_;
			setHeader({"content-length", std::to_string(data.size())});
		}

		const std::string& Response::getData() const {
			return data;
		}

		void Response::addHeader(const HttpHeader& header_) {
			headers.push_back(header_);
		}

		void Response::setHeader(const HttpHeader& header_) {
			for (auto& header : headers) {
				if (header.getName() == header_.getName()) {
					header = header_;
					return;
				}
			}

			addHeader(header_);
		}

		bool Response::hasHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return true;
				}
			}

			return false;
		}


		const HttpHeader& Response::getHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return header;
				}
			}

			throw std::runtime_error("no header by name " + name);
		}

		void Response::addCookie(const HttpCookie& cookie_) {
			cookies.push_back(cookie_);
		}

		void Response::setCookie(const HttpCookie& cookie_) {
			for (auto& cookie : cookies) {
				if (cookie.getName() == cookie_.getName()) {
					cookie = cookie_;
					return;
				}
			}

			addCookie(cookie_);
		}

		bool Response::hasCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return true;
				}
			}

			return false;
		}

		const HttpCookie& Response::getCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return cookie;
				}
			}

			throw std::runtime_error("no cookie by name " + name);
		}


		bool Response::writeHeaders(Client& client) const {
			std::string buffer;
			buffer.reserve(1024);

			buffer += fmt::format("HTTP/1.1 {}\r\n", client.response->getCode());

			for (auto& header : headers) {
				buffer += header.getName() + ":" + header.getValue() + "\r\n";
			}

			if (headers.empty()) buffer += "\r\n";
			buffer += "\r\n";

			return client.send(buffer);
		}

		bool Response::writeBody(Client& client) const {
			return client.send(data);
		}
	}
}
