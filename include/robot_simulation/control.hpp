//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__control__hpp_INCLUDED_
#define _robot_simulation__control__hpp_INCLUDED_

#include <robot_simulation/rope_platform.hpp>
#include <robot_simulation/robot.hpp>

#include <webservice/json_ws_client.hpp>


namespace robot_simulation{


	class control: webservice::json_ws_client{
	public:
		control(std::string host, std::string port, std::string resource);


	protected:
		void on_json(nlohmann::json&& text)override;

		void on_binary(std::vector< std::uint8_t >&&)override;

		void on_open()override;

		void on_close()override;

		void on_error(
			webservice::ws_client_location location,
			boost::system::error_code ec
		)override;

		void on_exception(std::exception_ptr error)noexcept override;


	private:
		rope_platform platform_;
		robot robot_;
	};


}


#endif
