//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__rope_platform__hpp_INCLUDED_
#define _robot_simulation__rope_platform__hpp_INCLUDED_

#include "types.hpp"
#include "move_to_fn.hpp"

#include <future>


namespace robot_simulation{


	class rope_platform{
	public:
		rope_platform(
			ws_server_service& gui,
			double acceleration,
			double max_speed,
			position min,
			position max);

		std::future< void > move_to(position target_pos);

		position pos()const{ return pos_; }

		double const acceleration_;
		double const max_speed_;

		position const min_;
		position const max_;


	private:
		std::mutex mutex_;

		ws_server_service& gui_;

		position pos_;

		friend auto
		move_to_fn< rope_platform, position >(ws_server_service&,
			rope_platform&, position&&, std::unique_lock< std::mutex >&&);
	};


}


#endif
