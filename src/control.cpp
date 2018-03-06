//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <robot_simulation/control.hpp>


namespace robot_simulation{


	control::control(std::string host, std::string port, std::string resource)
		: json_ws_client(std::move(host), std::move(port),std::move(resource))
		, platform_(0.4, 2, {0.8, 0.3, 0.5}, {15, 17, 14})
		, robot_(0.1, 1, {-2, -2, 0}, {2, 2, 4}) {}

	void control::on_json(nlohmann::json&& text){
		//
	}

	void control::on_binary(std::vector< std::uint8_t >&&){
		throw std::logic_error("unexpected binary message");
	}

	void control::on_open(){
		std::cout << "opened WebSocket session\n";
	}

	void control::on_close(){
		std::cout << "closed WebSocket session\n";
	}

	void control::on_error(
		webservice::ws_client_location location,
		boost::system::error_code ec
	){
		std::cout << "\033[1;31mWebSocket error in " << to_string_view(location)
			<< ": " << ec.message() << "\033[0m\n";
	}

	void control::on_exception(std::exception_ptr error)noexcept{
		try{
			std::rethrow_exception(error);
		}catch(std::exception const& e){
			std::cout << "\033[1;31mWebSocket exception: "
				<< e.what() << "\033[0m\n";
		}catch(...){
			std::cout << "\033[1;31mUnknown WebSocket exception\033[0m\n";
		}
	}

}
