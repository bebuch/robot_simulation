//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/robot_simulation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _robot_simulation__printing_error_handler__hpp_INCLUDED_
#define _robot_simulation__printing_error_handler__hpp_INCLUDED_

#include <webservice/error_handler.hpp>


namespace robot_simulation{


	class printing_error_handler: public webservice::error_handler{
		void on_exception(std::exception_ptr error)noexcept override{
			try{
				std::rethrow_exception(error);
			}catch(std::exception const& e){
				std::cout << "\033[1;31mfail error_handler: unexpected "
					"exception: " << e.what() << "\033[0m\n";
			}catch(...){
				std::cout << "\033[1;31mfail error_handler: unexpected "
					"unknown exception\033[0m\n";
			}
			std::exit(1);
		}
	};


}


#endif
