﻿#include <src/SimpleApp/SimpleApp.h>
#include <src/Event/Event.h>

int main(int argn, char** argc)
{
	::App::Init(argn, argc);

	int app_result = ::App::Run();

	::App::Terminate();
	return app_result;
}