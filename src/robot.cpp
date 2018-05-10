//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <robot_simulation/robot.hpp>

#include <stdexcept>


namespace robot_simulation{


	robot::robot(
		ws_server_service& gui,
		double acceleration,
		double max_speed,
		double max_current,
		double max_voltage,
		double variation_current,
		double variation_voltage,
		position min,
		position max
	)
		: acceleration_(acceleration)
		, max_speed_(max_speed)
		, max_current_(max_current)
		, max_voltage_(max_voltage)
		, variation_current_(variation_current)
		, variation_voltage_(variation_voltage)
		, min_(min)
		, max_(max)
		, gui_(gui)
		, pos_{{0, 0, 0}, {0, 0, 0}}
	{
		std::cout << "   " << pos_
			<< "; max speed: " << std::setw(6) << max_speed_ << " m/s\n";
	}


	std::future< void > robot::move_to(robot_position target_pos){
		std::unique_lock lock(mutex_);

		if(is_out_of_range(target_pos.position, min_, max_)){
			throw std::out_of_range(
				"robot_position out of range target position");
		}

		return std::async(move_to_fn(gui_,
			*this, std::move(target_pos), std::move(lock)));
	}

	std::future< void > robot::weld_to(robot_weld_target to){
		std::unique_lock lock(mutex_);

		auto target_pos = to_robot_target(pos_, to);

		if(is_out_of_range(target_pos.position, min_, max_)){
			throw std::out_of_range(
				"robot_position out of range target position");
		}

		return std::async(move_to_fn(gui_,
			*this, std::move(to), std::move(lock)));
	}

	std::list< weld_params > robot::get_weld_params(){
		std::lock_guard lock(weld_params_mutex_);
		std::list< weld_params > result;
		result.splice(begin(result), std::move(weld_params_));
		return result;
	}


}
