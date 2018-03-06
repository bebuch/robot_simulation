//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <robot_simulation/rope_platform.hpp>


namespace robot_simulation{


	rope_platform::rope_platform(
		double acceleration,
		double max_speed,
		position min,
		position max
	)
		: acceleration_(acceleration)
		, max_speed_(max_speed)
		, min_(min)
		, max_(max)
		, pos_{min.x, min.y, max.z}
	{
		std::cout << "   " << pos_
			<< "; max speed: " << std::setw(6) << max_speed_ << " m/s\n";
	}



	std::future< void > rope_platform::move_to(position target_pos){
		if(is_out_of_range(target_pos, min_, max_)){
			throw std::out_of_range(
				"rope_platform out of range target position");
		}

		return std::async(move_to_fn(*this, std::move(target_pos)));
	}


}
