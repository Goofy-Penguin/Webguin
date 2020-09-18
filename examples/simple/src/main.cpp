#include <chrono>

#include <mainframe/webguin/server.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

using namespace mainframe::webguin;

// http://127.0.0.1:<PORT>/foo
const int SERVER_PORT = 5050;

Server server;
int main() {
	server.addMethod("foo", [](std::shared_ptr<const Request> request, std::shared_ptr<Response> response) {
		response->setCode(200);
		response->setData("bar");
	});

	if (!server.host(SERVER_PORT)) {
		return 1;
	}

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}
