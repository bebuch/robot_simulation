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
#include <list>


namespace robot_simulation{


	class robot{
	public:
		robot(
			ws_server_service& gui,
			double acceleration,
			double max_speed,
			double max_current,
			double max_voltage,
			double variation_current,
			double variation_voltage,
			position min,
			position max);

		std::future< void > move_to(robot_position target_pos);

		std::future< void > weld_to(robot_weld_target target_pos);

		robot_position pos()const noexcept{ return pos_; }

		std::list< weld_params > get_weld_params();


		double const acceleration_;
		double const max_speed_;
		double const max_current_;
		double const max_voltage_;
		double const variation_current_;
		double const variation_voltage_;

		position const min_;
		position const max_;


	private:
		std::mutex mutex_;
		std::mutex weld_params_mutex_;

		ws_server_service& gui_;

		robot_position pos_;

		std::list< weld_params > weld_params_;

		friend auto
		move_to_fn< robot, robot_position >(ws_server_service&,
			robot&, robot_position&&, std::unique_lock< std::mutex >&&);

		friend auto
		move_to_fn< robot, robot_weld_target >(ws_server_service&,
			robot&, robot_weld_target&&, std::unique_lock< std::mutex >&&);
	};


}


#endif
