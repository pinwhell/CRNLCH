#include <iostream>

int main(int argc, const char** argv)
{
	std::cout << "The Test Say Hi!" << std::endl;
	
	for (int i = 1; i < argc; i++)
		printf("argv[%d] => %s\n", i, argv[i]);
}
