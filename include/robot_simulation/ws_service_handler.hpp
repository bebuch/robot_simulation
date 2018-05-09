//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__ws_service_handler__hpp_INCLUDED_
#define _robot_simulation__ws_service_handler__hpp_INCLUDED_

#include <webservice/ws_service_handler.hpp>


namespace robot_simulation{


	class ws_service_handler: public webservice::ws_service_handler{
		void on_exception(std::exception_ptr error)noexcept override{
			try{
				std::rethrow_exception(error);
			}catch(std::exception const& e){
				std::cout
					<< "\033[1;31mfail ws_service_handler: "
					<< "unexpected exception: " << e.what() << "\033[0m\n";
			}catch(...){
				std::cout <<
					"\033[1;31mfail ws_service_handler: unexpected unknown "
					"exception\033[0m\n";
			}
		}
	};



}


#endif
