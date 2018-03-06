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

#include <mutex>
#include <thread>
#include <cmath>
#include <iostream>
#include <chrono>


namespace robot_simulation{


	template < typename Robot, typename Pos >
	auto move_to_fn(Robot& robot, Pos&& target_pos){
		return [&robot, target_pos = std::forward< Pos >(target_pos)]{
				auto& mutex_ = robot.mutex_;
				auto& pos_ = robot.pos_;
				auto const acceleration_ = robot.acceleration_;
				auto const max_speed_ = robot.max_speed_;

				std::lock_guard lock(mutex_);

				auto const source_pos = pos_;
				auto const direction = target_pos - source_pos;

				auto const distance =
					robot_simulation::distance(pos_, target_pos);

				auto time = 0.0;

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
					pos_ = source_pos + direction * fraction;

					auto const tock = std::chrono::high_resolution_clock::now();

					using namespace std::literals::chrono_literals;
					constexpr auto step = 50ms;
					std::this_thread::sleep_for(step - (tock - tick));
					time += std::chrono::duration_cast<
						std::chrono::duration< double > >(step).count();

					std::cout << pos_
						<< "; time: " << std::setw(6) << time << " s"
						<< "; speed: " << std::setw(6) << speed  << " m/s\n";
				}

				pos_ = target_pos;

				std::cout << pos_
					<< "; time: " << std::setw(6) << end_time << " s"
					<< "; speed: " << std::setw(6) << 0.0 << " m/s\n";
			};
	}


}


#endif
