#include "SystemClass.h"

int main(void)
{
	SystemClass system = SystemClass();

	if (system.Initialize())
	{
		system.Run();
	}

	system.Shutdown();

	return 0;
}