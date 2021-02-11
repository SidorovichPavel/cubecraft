#include <iostream>
#include <vector>
#include <array>
#include <iterator>

#include <src/SimpleApp/SimpleApp.h>

int main(int argn, char** argc)
{
	::App::Init(argn, argc);
	
	int app_result = ::App::Run();

	::App::Terminate();
	return app_result;
}