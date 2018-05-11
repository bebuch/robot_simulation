//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__types__hpp_INCLUDED_
#define _robot_simulation__types__hpp_INCLUDED_

#include <cmath>
#include <iostream>
#include <iomanip>


namespace robot_simulation{


	struct position{
		// in meter
		double x;
		double y;
		double z;
	};

	constexpr position operator-(
		position const& p1,
		position const& p2
	){
		return {
				p1.x - p2.x,
				p1.y - p2.y,
				p1.z - p2.z
			};
	}

	constexpr position operator+(
		position const& p1,
		position const& p2
	){
		return {
				p1.x + p2.x,
				p1.y + p2.y,
				p1.z + p2.z
			};
	}

	constexpr position operator*(
		position const& p1,
		double factor
	){
		return {
				p1.x * factor,
				p1.y * factor,
				p1.z * factor
			};
	}

	constexpr position operator/(
		position const& p1,
		double factor
	){
		return {
				p1.x / factor,
				p1.y / factor,
				p1.z / factor
			};
	}

	constexpr double sqr(double v){ return v * v; }

	inline double distance(position const& p1, position const& p2){
		using std::sqrt;
		return sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y) + sqr(p1.z - p2.z));
	}

	constexpr position normalize(position const& p){
		return p / std::sqrt(sqr(p.x) + sqr(p.y) + sqr(p.z));
	}

	constexpr bool is_out_of_range(
		position const& p,
		position const& min,
		position const& max
	){
		return
			p.x < min.x || p.x > max.x ||
			p.y < min.y || p.y > max.y ||
			p.z < min.z || p.z > max.z;
	}

	inline std::ostream& operator<<(std::ostream& os, position const& p){
		return os << std::fixed << std::setprecision(3)
			<< "{" << std::setw(6) << p.x << " m"
			<< ", " << std::setw(6) << p.y << " m"
			<< ", " << std::setw(6) << p.z << " m}";
	}

	struct orientation{
		// angles in radiant
		double roll;
		double pitch;
		double yaw;
	};

	constexpr orientation operator-(
		orientation const& p1,
		orientation const& p2
	){
		return {
				p1.roll - p2.roll,
				p1.pitch - p2.pitch,
				p1.yaw - p2.yaw
			};
	}

	constexpr orientation operator+(
		orientation const& p1,
		orientation const& p2
	){
		return {
				p1.roll + p2.roll,
				p1.pitch + p2.pitch,
				p1.yaw + p2.yaw
			};
	}

	constexpr orientation operator*(
		orientation const& p1,
		double factor
	){
		return {
				p1.roll * factor,
				p1.pitch * factor,
				p1.yaw * factor
			};
	}

	inline std::ostream& operator<<(std::ostream& os, orientation const& o){
		return os << std::fixed << std::setprecision(3)
			<< "{" << std::setw(6) << o.roll << " rad
			<< ", " << std::setw(6) << o.pitch << " rad"
			<< ", " << std::setw(6) << o.yaw << " rad"}";
	}


	struct robot_position{
		struct position position;
		struct orientation orientation;
	};

	constexpr robot_position operator-(
		robot_position const& p1,
		robot_position const& p2
	){
		return {
				p1.position - p2.position,
				p1.orientation - p2.orientation
			};
	}

	constexpr robot_position operator+(
		robot_position const& p1,
		robot_position const& p2
	){
		return {
				p1.position + p2.position,
				p1.orientation + p2.orientation
			};
	}

	constexpr robot_position operator*(
		robot_position const& p1,
		double factor
	){
		return {
				p1.position * factor,
				p1.orientation * factor
			};
	}

	inline std::ostream& operator<<(std::ostream& os, robot_position const& p){
		return os << "{" << p.position << ", " << p.orientation << "}";
	}


	inline double distance(robot_position const& p1, robot_position const& p2){
		return distance(p1.position, p2.position);
	}

	struct robot_weld_target: robot_position{
		double add;
	};

	constexpr robot_position to_robot_target(
		robot_position from,
		robot_weld_target to
	){
		// ignore for now orientation is wrong, it's just a simulation ...
		return robot_position{
				to.position + normalize(to.position - from.position) * to.add,
				to.orientation
			};
	}

	struct weld_params: robot_position{
		double time;
		double current;
		double voltage;
	};


}


#endif
