#include <iostream>

void g() {}

int return_zero() {
	return 0;
}

void print_name(std::string name) {
	std::cout << "Name: " << name << std::endl;
}

int main() {
	int n = 0;
	n++;
	std::cout << "n = " << n << std::endl;
	std::cout << "matan\n";
	std::cout << "hello everyone\n";

	std::cout << "goodbye everyone\n";
	return 0;
}
