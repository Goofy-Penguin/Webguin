#include <chrono>

#include <mainframe/webguin/server.h>
#include <mainframe/webguin/controller.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

using namespace mainframe::webguin;

// http://127.0.0.1:<PORT>
const int SERVER_PORT = 5050;

void testMethod(std::shared_ptr<const Request> request, std::shared_ptr<Response> response) {
	response->setCode(200);
	response->setData("Hello World!");
}

// TestControllerSub
// http://127.0.0.1:<PORT>/foo/bar/hello METHOD
// http://127.0.0.1:<PORT>/foo/bar/*/* METHOD
// http://127.0.0.1:<PORT>/foo/bar/* EXECUTE
class TestControllerSub : public Controller {
public:
	TestControllerSub() {
		addMethod("hello", testMethod);
		addMethod("*/*", testMethod);
	}

	virtual void execute(std::shared_ptr<const Request> request, std::shared_ptr<Response> response) override {
		testMethod(request, response);
	}
};


// TestController
// http://127.0.0.1:<PORT>/foo/hello METHOD
// http://127.0.0.1:<PORT>/foo/bar/* CONTROLLER
class TestController : public Controller {
public:
	TestController() {
		addMethod("hello", testMethod);
		addController<TestControllerSub>("bar/*");
	}
};

// TestServer
// http://127.0.0.1:<PORT>/hello METHOD
// http://127.0.0.1:<PORT>/foo/* CONTROLLER
class TestServer : public Server {
public:
	TestServer() {
		addMethod("hello", testMethod);
		addController<TestController>("foo/*");
	}
};

// create our own server object and host
TestServer server;
int main() {
	if (!server.host(SERVER_PORT)) {
		return 1;
	}

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}