#include <src/SimpleApp/SimpleApp.h>

int main(int argn, char** argc)
{
	la::vec3 v({ 1,0,0 });
	la::Quaternion q(v, la::rad(90));
	auto r = v * q;

	::App::Init(argn, argc);

	int app_result = ::App::Run();

	::App::Terminate();
	return app_result;
}