//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__move_to_fn__hpp_INCLUDED_
#define _robot_simulation__move_to_fn__hpp_INCLUDED_

#include "ws_server_service.hpp"

#include <mutex>
#include <thread>
#include <cmath>
#include <iostream>
#include <chrono>
#include <random>


namespace robot_simulation{


	struct robot_weld_target;

	template < typename Robot, typename Pos >
	auto move_to_fn(
		ws_server_service& gui,
		Robot& robot,
		Pos&& target_to,
		std::unique_lock< std::mutex >&& lock
	){
		return [
				&gui,
				&robot,
				target_to = std::forward< Pos >(target_to),
				lock = std::move(lock)
			]{
				auto& pos = robot.pos_;
				auto const acceleration_ = robot.acceleration_;
				auto const max_speed_ = robot.max_speed_;

				auto const variation_current_ = [&robot]{
						if constexpr(!std::is_same_v< Pos, position >){
							return robot.variation_current_;
						}else{
							(void)robot; // Silance GCC
							return 0.0;
						}
					}();
				auto const variation_voltage_ = [&robot]{
						if constexpr(!std::is_same_v< Pos, position >){
							return robot.variation_voltage_;
						}else{
							(void)robot; // Silance GCC
							return 0.0;
						}
					}();

				auto const source_pos = pos;
				auto const target_pos = [&pos, &target_to]{
					if constexpr(std::is_same_v< Pos, robot_weld_target >){
						return to_robot_target(pos, target_to);
					}else{
						return target_to;
					}
				}();
				auto const direction = target_pos - source_pos;

				auto const distance =
					robot_simulation::distance(pos, target_pos);

				std::size_t start_time_ms =
					std::chrono::time_point_cast< std::chrono::milliseconds >(
						std::chrono::high_resolution_clock::now()
					).time_since_epoch().count();;
				auto time = 0.0;

				auto epoch_time_ms = [start_time_ms, &time]{
						return std::ceil(start_time_ms + time * 1000);
					};

				using std::sqrt;
				using std::min;
				auto const half_way_speed =
					acceleration_ * sqrt(distance / acceleration_);
				auto const full_speed = min(max_speed_, half_way_speed);
				auto const acceleration_time = full_speed / acceleration_;
				auto const acceleration_way
					= acceleration_ * sqr(acceleration_time) / 2;
				auto const full_speed_way = distance - acceleration_way * 2;
				auto const full_speed_time = full_speed_way / full_speed;

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<>
					current_dis_on(-variation_current_, variation_current_);
				std::uniform_real_distribution<>
					voltage_dis_on(-variation_voltage_, variation_voltage_);
				std::uniform_real_distribution<>
					current_dis_off(0, variation_current_ / 10);
				std::uniform_real_distribution<>
					voltage_dis_off(0, variation_voltage_ / 10);
				auto const weld_distance =
					robot_simulation::distance(pos, target_to);


				auto const calc =
					[=](double time){
						auto acc_time = min(time, acceleration_time);
						auto way = acceleration_ * sqr(acc_time) / 2;
						if(time < acceleration_time){
							return std::pair(way, time * acceleration_);
						}

						time -= acceleration_time;
						way += full_speed * min(time, full_speed_time);
						if(time < full_speed_time){
							return std::pair(way, full_speed);
						}

						time -= full_speed_time;
						way += full_speed * time
							- acceleration_ * sqr(time) / 2;
						return std::pair(way,
							(acceleration_time - time) * acceleration_);
					};

				auto const end_time = full_speed_time + 2 * acceleration_time;

				while(time < end_time){
					auto const tick = std::chrono::high_resolution_clock::now();

					auto const [distance_done, speed] = calc(time);

					auto const fraction = distance_done / distance;
					pos = source_pos + direction * fraction;

					if constexpr(std::is_same_v< Pos, robot_weld_target >){
						auto current = robot.max_current_;
						auto voltage = robot.max_voltage_;

						if(distance_done < weld_distance){
							current += current_dis_on(gen);
							voltage += voltage_dis_on(gen);
						}else{
							current = current_dis_off(gen);
							voltage = voltage_dis_off(gen);
						}

						std::cout
							<< "current: " << std::setw(9) << current << " A; "
							<< "voltage: " << std::setw(9) << voltage << " V; ";

						robot.weld_params_.push_back({
								{pos},
								epoch_time_ms(),
								current,
								voltage
							});

						gui.send_text(nlohmann::json{
								{"type", "robot"},
								{"time", epoch_time_ms()},
								{"x", pos.position.x},
								{"y", pos.position.y},
								{"z", pos.position.z},
								{"roll", pos.orientation.roll},
								{"yaw", pos.orientation.yaw},
								{"pitch", pos.orientation.pitch},
								{"current", current},
								{"voltage", voltage}
							});
					}else{
						(void)gen;
						(void)current_dis_on;
						(void)voltage_dis_on;
						(void)current_dis_off;
						(void)voltage_dis_off;
						(void)weld_distance;

						if constexpr(std::is_same_v< Pos, robot_position >){
							gui.send_text(nlohmann::json{
									{"type", "robot"},
									{"time", epoch_time_ms()},
									{"x", pos.position.x},
									{"y", pos.position.y},
									{"z", pos.position.z},
									{"roll", pos.orientation.roll},
									{"yaw", pos.orientation.yaw},
									{"pitch", pos.orientation.pitch},
									{"current", 0},
									{"voltage", 0}
								});
						}else{
							(void) epoch_time_ms;
							gui.send_text(nlohmann::json{
									{"type", "carrier"},
									{"x", pos.x},
									{"y", pos.y},
									{"z", pos.z}
								});
						}
					}

					auto const tock = std::chrono::high_resolution_clock::now();

					using namespace std::literals::chrono_literals;
					constexpr auto step = 50ms;
					std::this_thread::sleep_for(step - (tock - tick));
					time += std::chrono::duration_cast<
						std::chrono::duration< double > >(step).count();

					std::cout << pos
						<< "; time: " << std::setw(6) << time << " s"
						<< "; speed: " << std::setw(6) << speed  << " m/s\n";
				}

				pos = target_pos;

				std::cout << pos
					<< "; time: " << std::setw(6) << end_time << " s"
					<< "; speed: " << std::setw(6) << 0.0 << " m/s\n";
			};
	}


}


#endif
