
#include "pch.h"
#include "Core.h"
#include <ctime>
#include <vld.h>

int main(int argc, char *argv[])
{
	srand(int(time(nullptr)));
	Core core{ Window{ "Project name - Name, first name - 1DAEXX", 1280, 720 } };
	core.Run();
	return 0;
}
