#include <src/SimpleApp/SimpleApp.h>

int main(int argn, char** argc)
{
	auto v = la::vec3{ 1,2,3 }*la::vec3{ 0,-1,0 };

	::App::Init(argn, argc);

	int app_result = ::App::Run();

	::App::Terminate();
	return app_result;
}