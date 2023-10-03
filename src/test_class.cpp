#include <iostream>
#include <array>

template<std::size_t Nwalkers>
class TestArray {
	private:
		std::array<int, Nwalkers> test;
	public:
		TestArray(std::array<int, Nwalkers> x0s) : test(x0s) {};
		std::array<int, Nwalkers> getArray() {return test;}
};

int main() {

	std::array<int, 2> x0s = {1,4};
	TestArray<2> test(x0s);
	std::array<int, 2> tester = test.getArray();
	std::cout << tester.at(0) << ", " << tester.at(1) << std::endl;
	
	return 0;

}
