#include <robot_simulation/control.hpp>


int main(int argc, char** argv){
	try{
		robot_simulation::control cl("127.0.0.1", "8080", "/robot");
		(void)cl;
	}catch(std::exception const& e){
		std::cout << "\033[1;31mException: " << e.what() << "\033[0m\n";
	}catch(...){
		std::cout << "\033[1;31mUnknown exception\033[0m\n";
	}

// 	robot_simulation::rope_platform
// 		platform(0.4, 2, {0.8, 0.3, 0.5}, {15, 17, 14});
// 	std::cin.get();
// 	std::cout << "-> " << robot_simulation::position{8, 12, 10} << std::endl;
// 	std::cin.get();
// 	platform.move_to({8, 12, 10}).get();
// 	std::cin.get();
//
//
// 	robot_simulation::robot robot(0.1, 1, {-2, -2, 0}, {2, 2, 4});
// 	std::cin.get();
// 	std::cout << "-> " << robot_simulation::robot_position{{-2, -2, 0}, {2, 2, 4}}
// 		<< std::endl;
// 	std::cin.get();
// 	robot.move_to({{1.5, -2, 3.89}, {1, 2, 1.5}}).get();
// 	std::cin.get();
}
