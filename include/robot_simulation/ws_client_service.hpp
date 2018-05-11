//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__ws_client_service__hpp_INCLUDED_
#define _robot_simulation__ws_client_service__hpp_INCLUDED_

#include "rope_platform.hpp"
#include "robot.hpp"


namespace robot_simulation{


	class ws_client_service: public webservice::json_ws_service{
	public:
		ws_client_service(ws_server_service& gui)
			: gui_(gui)
			, platform_(gui, 0.4, 2, {0.8, 0.3, 0.5}, {15, 17, 14})
			, robot_(gui, 0.1, 1, 100, 20, 2, 0.5, {-2, -2, 0}, {2, 2, 4})
		{
			set_init();
		}

		void set_init(){
			auto platform_pos = platform_.pos();
			auto robot_pos = robot_.pos();
			gui_.set_init(nlohmann::json::object({
					{"type", "init"},
					{"carrier_minmax", nlohmann::json::object({
						{"min", nlohmann::json::object({
							{"x", platform_.min_.x},
							{"y", platform_.min_.y},
							{"z", platform_.min_.z}})},
						{"max", nlohmann::json::object({
							{"x", platform_.max_.x},
							{"y", platform_.max_.y},
							{"z", platform_.max_.z}})}})},
					{"robot_minmax", nlohmann::json::object({
						{"min", nlohmann::json::object({
							{"x", robot_.min_.x},
							{"y", robot_.min_.y},
							{"z", robot_.min_.z}})},
						{"max", nlohmann::json::object({
							{"x", robot_.max_.x},
							{"y", robot_.max_.y},
							{"z", robot_.max_.z}})}})},
					{"current_max", robot_.max_current_},
					{"voltage_max", robot_.max_voltage_},
					{"current_variation", robot_.variation_current_},
					{"voltage_variation", robot_.variation_voltage_},
					{"carrier", nlohmann::json::object({
							{"x", platform_pos.x},
							{"y", platform_pos.y},
							{"z", platform_pos.z}
						})},
					{"robot", nlohmann::json::object({
							{"x", robot_pos.position.x},
							{"y", robot_pos.position.y},
							{"z", robot_pos.position.z},
							{"roll", robot_pos.orientation.roll},
							{"pitch", robot_pos.orientation.pitch},
							{"yaw", robot_pos.orientation.yaw},
							{"current", 0},
							{"voltage", 0}
						})}
				}));
		}

	private:
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

						platform_.move_to({x, y, z}).get();

						auto pos = platform_.pos();
						send_text(identifier, nlohmann::json{
								{"type", "carrier_ready"},
								{"x", pos.x},
								{"y", pos.y},
								{"z", pos.z}
							});

						set_init();
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

						robot_.move_to({{x, y, z}, {roll, pitch, yaw}}).get();

						auto pos = robot_.pos();
						send_text(identifier, nlohmann::json{
								{"type", "robot_ready"},
								{"x", pos.position.x},
								{"y", pos.position.y},
								{"z", pos.position.z},
								{"roll", pos.orientation.roll},
								{"pitch", pos.orientation.pitch},
								{"yaw", pos.orientation.yaw}
							});

						set_init();
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

						robot_.weld_to(
							{{{x, y, z}, {roll, pitch, yaw}}, add}).get();

						send_text(identifier, nlohmann::json{
								{"type", "weld_ready"}
							});

						set_init();
					}, std::allocator< void >());
			}else if(type == "get_weld_parameters"){
				executor().get_executor().defer(
					[this, identifier]{
						std::cout << "exec: get_weld_parameters\n";

						auto params = robot_.get_weld_params();

						auto message = nlohmann::json::object({
								{"type", "weld_parameters"},
								{"list", nlohmann::json::array()}
							});

						for(auto& param: params){
							message["list"].push_back(nlohmann::json::object({
									{"time", param.time},
									{"x", param.position.x},
									{"y", param.position.y},
									{"z", param.position.z},
									{"roll", param.orientation.roll},
									{"pitch", param.orientation.pitch},
									{"yaw", param.orientation.yaw},
									{"current", param.current},
									{"voltage", param.voltage}
								}));
						}

						send_text(identifier, message);
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


		void on_exception(std::exception_ptr error)noexcept override{
			try{
				std::rethrow_exception(error);
			}catch(std::exception const& e){
				std::cout << "\033[1;31mfail ws_client_service: "
					<< "unexpected exception: " << e.what() << "\033[0m\n";
			}catch(...){
				std::cout << "\033[1;31mfail ws_client_service: "
					"unexpected unknown exception\033[0m\n";
			}
		}

		void on_exception(
			webservice::ws_identifier,
			std::exception_ptr error
		)noexcept override{
			on_exception(error);
		}


		ws_server_service& gui_;

		rope_platform platform_;

		robot robot_;
	};


}


#endif
