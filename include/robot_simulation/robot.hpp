//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__robot__hpp_INCLUDED_
#define _robot_simulation__robot__hpp_INCLUDED_

#include "types.hpp"
#include "move_to_fn.hpp"

#include <future>


namespace robot_simulation{


	class robot{
	public:
		robot(
			double acceleration,
			double max_speed,
			position min,
			position max);

		std::future< void > move_to(robot_position target_pos);

		robot_position pos()const noexcept{ return pos_; }


	private:
		std::mutex mutex_;

		double acceleration_;
		double max_speed_;

		position min_;
		position max_;

		robot_position pos_;

		friend auto
		move_to_fn< robot, robot_position >(robot&, robot_position&&);
	};


}


#endif
