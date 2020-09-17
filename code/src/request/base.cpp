#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/client.h>

namespace mainframe {
	namespace webguin {
		RequestBase::RequestBase() {
		}

		void RequestBase::setData(const std::string& data_) {
			data = data_;
		}

		const std::string& RequestBase::getData() const {
			return data;
		}

		void RequestBase::setPath(const std::string& path_) {
			path = path_;
		}

		const std::string& RequestBase::getPath() const {
			return path;
		}

		void RequestBase::setMethod(MethodType method_) {
			method = method_;
		}

		MethodType RequestBase::getMethod() const {
			return method;
		}

		void RequestBase::setHeader(const HttpHeader& header_) {
			for (auto& header : headers) {
				if (header.getName() == header_.getName()) {
					header = header_;
					return;
				}
			}

			addHeader(header_);
		}


		void RequestBase::addHeader(const HttpHeader& header_) {
			headers.push_back(header_);
		}

		bool RequestBase::hasHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return true;
				}
			}

			return false;
		}


		const HttpHeader& RequestBase::getHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return header;
				}
			}

			throw std::runtime_error("no header by name " + name);
		}

		void RequestBase::setCookie(const HttpCookie& cookie_) {
			for (auto& cookie : cookies) {
				if (cookie.getName() == cookie_.getName()) {
					cookie = cookie_;
					return;
				}
			}

			addCookie(cookie_);
		}

		void RequestBase::addCookie(const HttpCookie& cookie_) {
			cookies.push_back(cookie_);
		}

		bool RequestBase::hasCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return true;
				}
			}

			return false;
		}

		const HttpCookie& RequestBase::getCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return cookie;
				}
			}

			throw std::runtime_error("no cookie by name " + name);
		}
	}
}
