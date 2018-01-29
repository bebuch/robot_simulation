#include <robot_simulation/rope_platform.hpp>
#include <robot_simulation/robot.hpp>

// #include <boost/beast/core.hpp>
// #include <boost/beast/websocket.hpp>
// #include <boost/asio/connect.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <cstdlib>
// #include <iostream>
// #include <string>
//
// using tcp = boost::asio::ip::tcp;
// namespace ws = boost::beast::websocket;

// Sends a WebSocket message and prints the response
int main(int argc, char** argv){
	robot_simulation::rope_platform
		platform(0.4, 2, {0.8, 0.3, 0.5}, {15, 17, 14});
	platform.move_to({8, 12, 10}).get();

	robot_simulation::robot robot(0.1, 1, {-2, -2, 0}, {2, 2, 4});
	robot.move_to({{1.5, -2, 3.89}, {1, 2, 1.5}}).get();

// 	try{
// 		// Check command line arguments.
// 		if(argc != 4){
// 			std::cerr <<
// 				"Usage: websocket-client-sync <host> <port> <text>\n" <<
// 				"Example:\n" <<
// 				"    websocket-client-sync echo.websocket.org 80 \"Hello, world!\"\n";
// 			return EXIT_FAILURE;
// 		}
// 		auto const host = argv[1];
// 		auto const port = argv[2];
// 		auto const text = argv[3];
//
// 		// The io_context is required for all I/O
// 		boost::asio::io_context ioc;
//
// 		// These objects perform our I/O
// 		tcp::resolver resolver{ioc};
// 		ws::stream< tcp::socket > ws{ioc};
//
// 		// Look up the domain name
// 		auto const results = resolver.resolve(host, port);
//
// 		// Make the connection on the IP address we get from a lookup
// 		boost::asio::connect(ws.next_layer(), results.begin(), results.end());
//
// 		// Perform the websocket handshake
// 		ws.handshake(host, "/");
//
// 		// Send the message
// 		ws.write(boost::asio::buffer(std::string(text)));
//
// 		// This buffer will hold the incoming message
// 		boost::beast::multi_buffer buffer;
//
// 		// Read a message into our buffer
// 		ws.read(buffer);
//
// 		// Close the WebSocket connection
// 		ws.close(ws::close_code::normal);
//
// 		// If we get here then the connection is closed gracefully
//
// 		// The buffers() function helps print a ConstBufferSequence
// 		std::cout << boost::beast::buffers(buffer.data()) << std::endl;
// 	}catch(std::exception const& e){
// 		std::cerr << "Error: " << e.what() << std::endl;
// 		return EXIT_FAILURE;
// 	}
// 	return EXIT_SUCCESS;
}
