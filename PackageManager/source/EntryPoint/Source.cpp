#include "Path.h"

#include <iostream>

int main()
{
	Path p("C:\\Stuff\\What");


	std::cout << "\"" << p.toString() << "\"" << std::endl;
	
	p.up();

	std::cout << "\"" << p.toString() << "\"" << std::endl;


	std::cin.get();	
}