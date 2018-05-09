//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/http
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <robot_simulation/file_request_handler.hpp>
#include <robot_simulation/ws_service_handler.hpp>
#include <robot_simulation/ws_client_service.hpp>
#include <robot_simulation/printing_error_handler.hpp>

#include <webservice/server.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <csignal>


webservice::server* server = nullptr;

void on_interrupt(int signum){
	std::signal(signum, SIG_DFL);
	std::cout << "Signal: " << signum << '\n';
	server->shutdown();
	std::cout << "Signal ready\n";
}


int main(){
	using namespace robot_simulation;

	try{
		std::uint8_t const thread_count = 10;

		std::string server_root = "gui";
		auto server_address = boost::asio::ip::make_address("127.0.0.1");
		std::uint16_t server_port = 8090;

		std::string const client_host = "127.0.0.1";
		std::string const client_port = "8080";
		std::string const client_resource = "/TCC";

		auto ws_handler_ptr = std::make_unique< ws_service_handler >();
		auto& ws_handler = *ws_handler_ptr;

		webservice::server server(
				std::make_unique< file_request_handler >(server_root),
				std::move(ws_handler_ptr),
				std::make_unique< printing_error_handler >(),
				server_address,
				server_port,
				thread_count
			);

		auto ws_server_service_ptr = std::make_unique< ws_server_service >();
		auto ws_client_service_ptr =
			std::make_unique< ws_client_service >(*ws_server_service_ptr);

		ws_handler.add_service("/", std::move(ws_server_service_ptr));
		ws_handler.add_service(client_resource,
			std::move(ws_client_service_ptr));

		// Connect to the control server
		server.connect(client_host, client_port, client_resource);

		// Allow to shutdown the server with CTRL+C
		::server = &server;
		std::signal(SIGINT, &on_interrupt);

		server.block();

		return 0;
	}catch(std::exception const& e){
		std::cerr << "Exception: " << e.what() << "\n";
		return 1;
	}catch(...){
		std::cerr << "Unknown exception\n";
		return 1;
	}
}
