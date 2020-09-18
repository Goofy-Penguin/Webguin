#include <chrono>
#include <fmt/format.h>

#include <mainframe/webguin/server.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>
#include <mainframe/webguin/model/param.h>

using namespace mainframe::webguin;

// http://127.0.0.1:<PORT>
const int SERVER_PORT = 5050;

// a default server object
Server server;
int main() {
	// http://127.0.0.1:<PORT>/foo/*/{a}/{b}/[c]/[d]
	server.addMethod("foo/*/{a}/{b}/[c]/[d]", [](std::shared_ptr<const Request> request, std::shared_ptr<Response> response) {
		std::string ret = fmt::format("path: {}\r\n", request->getPath());

		// params
		std::string a = request->getParam("a");
		std::string b = request->getParam("b");

		// optionals, they _might_ not be here, so check first before using them
		std::string c = request->hasParam("c") ? request->getParam("c").get<std::string>() : "";
		std::string d = request->hasParam("d") ? request->getParam("d").get<std::string>() : "";

		ret += fmt::format("a: {}\r\n", a);
		ret += fmt::format("b: {}\r\n", b);
		ret += fmt::format("c: {}\r\n", c);
		ret += fmt::format("d: {}\r\n", d);

		// response
		response->setCode(200);
		response->setData(ret);
	});

	// http://127.0.0.1:<PORT>/math/{a:number}/{operator:string}/{b:number}/[plusValue:number=30]
	server.addMethod("math/{a:number}/{operator:string}/{b:number}/[plusValue:number=30]", [](std::shared_ptr<const Request> request, std::shared_ptr<Response> response) {
		// params
		std::string opStr = request->getParam("operator");
		double a = request->getParam("a");
		double b = request->getParam("b");
		double plusValue = request->getParam("plusValue");

		// calculate our sum
		double output = 0;
		switch (opStr.front()) {
			case '+': output = a + b; break;
			case '-': output = a - b; break;
			case '*': output = a * b; break;
			case '\\': output = a / b; break;

			default:
				throw std::runtime_error("Invalid operator, must be one of [+, -, *, /]");
				break;
		}

		// add our 3th number
		output += plusValue;

		// response
		response->setCode(200);
		response->setData(fmt::format("{} {} {} + {} = {}", a, opStr.front(), b, plusValue, output));
	});

	// try to host on specified port
	if (!server.host(SERVER_PORT)) {
		return 1;
	}

	// wait for requests
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}
