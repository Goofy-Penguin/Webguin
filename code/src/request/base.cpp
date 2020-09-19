#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/client.h>

namespace mainframe {
	namespace webguin {
		Request::Request() {
		}

		void Request::setData(const std::string& data_) {
			data = data_;
		}

		const std::string& Request::getData() const {
			return data;
		}

		void Request::setPath(const std::string& path_) {
			path = path_;
		}

		const std::string& Request::getPath() const {
			return path;
		}

		const Param& Request::getParam(const std::string& name) const {
			return params.find(name)->second;
		}

		bool Request::hasParam(const std::string& name) const {
			return params.find(name) != params.end();
		}

		void Request::setParam(const std::string& name, const Param& value) {
			params[name] = value;
		}

		void Request::clearParams() {
			params.clear();
		}

		void Request::setMethod(MethodType method_) {
			method = method_;
		}

		MethodType Request::getMethod() const {
			return method;
		}

		void Request::setHeader(const HttpHeader& header_) {
			for (auto& header : headers) {
				if (header.getName() == header_.getName()) {
					header = header_;
					return;
				}
			}

			addHeader(header_);
		}


		void Request::addHeader(const HttpHeader& header_) {
			headers.push_back(header_);
		}

		bool Request::hasHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return true;
				}
			}

			return false;
		}


		const HttpHeader& Request::getHeader(const std::string& name) const {
			for (auto& header : headers) {
				if (header.getName() == name) {
					return header;
				}
			}

			throw std::runtime_error("no header by name " + name);
		}

		void Request::setCookie(const HttpCookie& cookie_) {
			for (auto& cookie : cookies) {
				if (cookie.getName() == cookie_.getName()) {
					cookie = cookie_;
					return;
				}
			}

			addCookie(cookie_);
		}

		void Request::addCookie(const HttpCookie& cookie_) {
			cookies.push_back(cookie_);
		}

		bool Request::hasCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return true;
				}
			}

			return false;
		}

		const HttpCookie& Request::getCookie(const std::string& name) const {
			for (auto& cookie : cookies) {
				if (cookie.getName() == name) {
					return cookie;
				}
			}

			throw std::runtime_error("no cookie by name " + name);
		}
	}
}
