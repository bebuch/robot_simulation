//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/http
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <robot_simulation/rope_platform.hpp>
#include <robot_simulation/robot.hpp>

#include <webservice/json_ws_service.hpp>
#include <webservice/client.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <csignal>


struct ws_service: webservice::json_ws_service{
	using webservice::json_ws_service::json_ws_service;

	void on_open(webservice::ws_identifier)override{
		std::cout << "opened session\n";
	}

	void on_close(webservice::ws_identifier)override{
		std::cout << "closed session\n";
	}

	void on_text(
		webservice::ws_identifier identifier,
		nlohmann::json&& data
	)override{
		std::cout << "received text message: " << data << "\n";
		auto type = data.at("type").get< std::string >();
		std::cout << "type: '" << type << "'\n";
		if(type == "get_carrier_minmax"){
			executor().get_executor().defer(
				[this, identifier]{
					std::cout << "exec: get_carrier_minmax\n";

					send_text(identifier, nlohmann::json::object({
							{"type", "carrier_minmax"},
							{"min", nlohmann::json::object({
								{"x", 0.8},
								{"y", 0.3},
								{"z", 0.5}
							})},
							{"max", nlohmann::json::object({
								{"x", 15.0},
								{"y", 17.0},
								{"z", 14.0}
							})}
					}));
				}, std::allocator< void >());
		}else if(type == "carrier_move_to"){
			executor().get_executor().defer(
				[this, identifier, data = std::move(data)]{
					std::cout << "exec: carrier_move_to\n";

					auto x = data.at("x").get< double >();
					auto y = data.at("y").get< double >();
					auto z = data.at("z").get< double >();

					std::cout << "x: " << x << "\n";
					std::cout << "y: " << y << "\n";
					std::cout << "z: " << z << "\n";

					platform.move_to({x, y, z}).get();

					auto pos = platform.pos();
					send_text(identifier, nlohmann::json{
							{"type", "carrier_ready"},
							{"x", pos.x},
							{"y", pos.y},
							{"z", pos.z}
						});
				}, std::allocator< void >());
		}else if(type == "robot_move_to"){
			executor().get_executor().defer(
				[this, identifier, data = std::move(data)]{
					std::cout << "exec: robot_move_to\n";

					auto x = data.at("x").get< double >();
					auto y = data.at("y").get< double >();
					auto z = data.at("z").get< double >();
					auto roll = data.at("roll").get< double >();
					auto pitch = data.at("pitch").get< double >();
					auto yaw = data.at("yaw").get< double >();

					std::cout << "x: " << x << "\n";
					std::cout << "y: " << y << "\n";
					std::cout << "z: " << z << "\n";
					std::cout << "roll: " << roll << "\n";
					std::cout << "pitch: " << pitch << "\n";
					std::cout << "yaw: " << yaw << "\n";

					robot.move_to({{x, y, z}, {roll, pitch, yaw}}).get();

					auto pos = robot.pos();
					send_text(identifier, nlohmann::json{
							{"type", "robot_ready"},
							{"x", pos.position.x},
							{"y", pos.position.y},
							{"z", pos.position.z},
							{"roll", pos.orientation.roll},
							{"pitch", pos.orientation.pitch},
							{"yaw", pos.orientation.yaw}
						});
				}, std::allocator< void >());
		}else if(type == "weld_to"){
			executor().get_executor().defer(
				[this, identifier, data = std::move(data)]{
					std::cout << "exec: weld_to\n";


					auto x = data.at("x").get< double >();
					auto y = data.at("y").get< double >();
					auto z = data.at("z").get< double >();
					auto roll = data.at("roll").get< double >();
					auto pitch = data.at("pitch").get< double >();
					auto yaw = data.at("yaw").get< double >();
					auto add = data.at("add").get< double >();

					std::cout << "x: " << x << "\n";
					std::cout << "y: " << y << "\n";
					std::cout << "z: " << z << "\n";
					std::cout << "roll: " << roll << "\n";
					std::cout << "pitch: " << pitch << "\n";
					std::cout << "yaw: " << yaw << "\n";
					std::cout << "add: " << add << "\n";

					(void)add; // TODO: Implement robot weld_to
					robot.move_to({{x, y, z}, {roll, pitch, yaw}}).get();

					send_text(identifier, nlohmann::json{
							{"type", "weld_ready"}
						});
				}, std::allocator< void >());
		}else{
			throw std::logic_error("unknown type: " + type);
		}
	}

	void on_binary(
		webservice::ws_identifier identifier,
		std::vector< std::uint8_t >&& /*data*/
	)override{
		std::cout << "received binary message\n";
		executor().get_executor().defer(
			[this, identifier]{
				std::cout << "exec: 3d_data\n";

				send_text(identifier, nlohmann::json{
						{"type", "3d_ready"}
					});
			}, std::allocator< void >());
}


	robot_simulation::rope_platform platform{
			0.4, 2, {0.8, 0.3, 0.5}, {15, 17, 14}
		};

	robot_simulation::robot robot{
			0.1, 1, {-2, -2, 0}, {2, 2, 4}
		};
};


webservice::client* client = nullptr;

void on_interrupt(int signum){
	std::signal(signum, SIG_DFL);
	std::cout << "Signal: " << signum << '\n';
	client->shutdown();
	std::cout << "Signal ready\n";
}


int main(){
	try{
		std::string const host = "127.0.0.1";
		std::string const port = "8080";
		std::uint8_t const thread_count = 10;
		std::string const resource = "/TCC";

		webservice::client client(
				std::make_unique< ws_service >(),
				nullptr, // ignore errors and exceptions
				thread_count
			);
		client.connect(host, port, resource);

		// Allow to shutdown the client with CTRL+C
		::client = &client;
		std::signal(SIGINT, &on_interrupt);

		client.block();

		return 0;
	}catch(std::exception const& e){
		std::cerr << "Exception: " << e.what() << "\n";
		return 1;
	}catch(...){
		std::cerr << "Unknown exception\n";
		return 1;
	}
}
