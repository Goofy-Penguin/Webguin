#include <mainframe/webguin/response/base.h>
#include <fmt/format.h>

namespace mainframe {
	namespace webguin {
		ResponseBase::ResponseBase() {
			addHeader({"content-type", "text/plain"});
			addHeader({"content-length", std::to_string(data.size())});
			addHeader({"Access-Control-Allow-Origin", "*"});
			addHeader({"server", "webguin"});
			addHeader({"connection", "close"});
		}

		void ResponseBase::setCode(int code_) {
			code = code_;
		}

		int ResponseBase::getCode() const {
			return code;
		}


		void ResponseBase::setData(const std::string& data_) {
			data = data_;
			setHeader({"content-length", std::to_string(data.size())});
		}

		const std::string& ResponseBase::getData() const {
			return data;
		}

		void ResponseBase::addHeader(const HttpHeader& header_) {
			headers.push_back(header_);
		}

		void ResponseBase::setHeader(const HttpHeader& header_) {
			for (auto& header : headers) {
				if (header.getName() == header_.getName()) {
					header = header_;
					return;
				}
			}

			addHeader(header_);
		}

		bool ResponseBase::hasHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return true;
				}
			}

			return false;
		}


		const HttpHeader& ResponseBase::getHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return header;
				}
			}

			throw std::runtime_error("no header by name " + name);
		}

		void ResponseBase::addCookie(const HttpCookie& cookie_) {
			cookies.push_back(cookie_);
		}

		void ResponseBase::setCookie(const HttpCookie& cookie_) {
			for (auto& cookie : cookies) {
				if (cookie.getName() == cookie_.getName()) {
					cookie = cookie_;
					return;
				}
			}

			addCookie(cookie_);
		}

		bool ResponseBase::hasCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return true;
				}
			}

			return false;
		}

		const HttpCookie& ResponseBase::getCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return cookie;
				}
			}

			throw std::runtime_error("no cookie by name " + name);
		}


		bool ResponseBase::writeHeaders(Client& client) const {
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

		bool ResponseBase::writeBody(Client& client) const {
			return client.send(data);
		}
	}
}