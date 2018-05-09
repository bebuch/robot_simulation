//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__ws_server_service__hpp_INCLUDED_
#define _robot_simulation__ws_server_service__hpp_INCLUDED_

#include <webservice/json_ws_service.hpp>


namespace robot_simulation{


	class ws_server_service: public webservice::json_ws_service{
	public:
		using webservice::json_ws_service::json_ws_service;

	private:
		void on_open(webservice::ws_identifier)override{
			std::cout << "opened gui session\n";
		}

		void on_close(webservice::ws_identifier)override{
			std::cout << "closed gui session\n";
		}

		void on_exception(std::exception_ptr error)noexcept override{
			try{
				std::rethrow_exception(error);
			}catch(std::exception const& e){
				std::cout
					<< "\033[1;31mfail ws_server_service: "
					<< "unexpected exception: " << e.what() << "\033[0m\n";
			}catch(...){
				std::cout <<
					"\033[1;31mfail ws_server_service: unexpected unknown "
					"exception\033[0m\n";
			}
		}
	};


}


#endif
