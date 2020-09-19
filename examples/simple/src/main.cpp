#include <chrono>

#include <mainframe/webguin/server.h>
#include <mainframe/webguin/request/base.h>
#include <mainframe/webguin/response/base.h>

using namespace mainframe::webguin;

// http://127.0.0.1:<PORT>
const int SERVER_PORT = 5050;

// a default server object
Server server;
int main() {
	// register http://127.0.0.1:<PORT>/foo
	server.addMethod("foo", [](const Request* request, Response* response) {
		response->setCode(200);
		response->setData("bar");
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
