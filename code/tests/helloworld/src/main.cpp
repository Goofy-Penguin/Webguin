#include <chrono>
#include <memory>
#include <string>
#include <fmt/printf.h>

#include <mainframe/webguin/server.h>
#include <mainframe/webguin/controller.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

using namespace mainframe::webguin;
using namespace mainframe::networking;

const std::string TEST_CONTENT = "Hello World!";

// TestServer
// http://127.0.0.1:<PORT>/hello METHOD
// http://127.0.0.1:<PORT>/test/* CONTROLLER

// TestController
// http://127.0.0.1:<PORT>/test/hello METHOD
// http://127.0.0.1:<PORT>/test/sub/* CONTROLLER

// TestControllerSub
// http://127.0.0.1:<PORT>/test/sub/hello METHOD
// http://127.0.0.1:<PORT>/test/sub/*/* METHOD
// http://127.0.0.1:<PORT>/test/sub/* EXECUTE

void testMethod(std::shared_ptr<RequestBase> request, std::shared_ptr<ResponseBase> response) {
	response->setCode(200);
	response->setData(TEST_CONTENT);
}

class TestControllerSub : public Controller {
public:
	TestControllerSub(const std::string& path) : Controller(path) {
		addMethod(std::make_shared<Method>("hello", testMethod));
		addMethod(std::make_shared<Method>("*/*", testMethod));
	}

	virtual void execute(std::shared_ptr<RequestBase> request, std::shared_ptr<ResponseBase> response) override {
		testMethod(request, response);
	}
};

class TestController : public Controller {
public:
	TestController(const std::string& path) : Controller(path) {
		addMethod(std::make_shared<Method>("hello", testMethod));
		addController(std::make_shared<TestControllerSub>("sub/*"));
	}
};

class TestServer : public Server {
public:
	TestServer() {
		addMethod(std::make_shared<Method>("hello", testMethod));
		addController(std::make_shared<TestController>("test/*"));
	}
};

std::unique_ptr<TestServer> server;

int main() {
	server = std::make_unique<TestServer>();

	if (!server->host(5050)) {
		fmt::print("Test failed at server host :(\n");
		return 1;
	}

	auto timeoutTimer = time(nullptr) + 5;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	fmt::print("Test successfull :)\n");
	return 0;
}