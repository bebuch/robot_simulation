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
		double acceleration,
		double max_speed,
		position min,
		position max
	)
		: acceleration_(acceleration)
		, max_speed_(max_speed)
		, min_(min)
		, max_(max)
		, pos_{{0, 0, 0}, {0, 0, 0}} {}


	std::future< void > robot::move_to(robot_position target_pos){
		if(is_out_of_range(target_pos.position, min_, max_)){
			throw std::out_of_range(
				"robot_position out of range target position");
		}

		return std::async(move_to_fn(*this, std::move(target_pos)));
	}


}
