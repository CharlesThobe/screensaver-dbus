#include "screensaver-dbus.h"
#include <stdio.h>
#include <iostream>

void CallFunctionAndPrintTestMessage(bool inhibit_requested)
{
	const char* state;
	if (ChangeScreenSaverStateDBus(inhibit_requested))
	{
		state = "Success";
	}
	else
	{
		state = "Failure";
	}
	printf("%s, inhibition status is %d\n", state, ScreenSaverStateDBusIsInhibited());
	printf("Command: ");
}

int main()
{
	char input;
	printf("Enter a character then hit enter\nValid inputs are:\n\"i\" for inhibiting\n\"u\" for uninhibiting\n\"s\" for stress testing (for memory leak detection)\nand \"x\" to exit.\n");
	printf("Command: ");
	while (true)
	{
		std::cin >> input;
		switch (input)
		{
			case 'i':
				CallFunctionAndPrintTestMessage(true);
				break;
			case 'u':
				CallFunctionAndPrintTestMessage(false);
				break;
			case 's':
				printf("Stress running\nPress ctl+c to terminate\n");
				while(true)
				{
					ChangeScreenSaverStateDBus(true);
					ChangeScreenSaverStateDBus(false);
				}
			case 'x':
				return 0;
			default:
				printf("Wrong input, Valid inputs are \"i\", \"u\", \"s\" and \"x\"\n");
				printf("Command: ");
		}
	}
}
