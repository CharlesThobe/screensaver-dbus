#include "screensaver-dbus.h"
#include <stdio.h>
#include <iostream>
int main(){
	char input;
	printf("Enter character then hit enter\nValid inputs are:\n\"i\" for inhibiting\n\"u\" for uninhibiting\n\"s\" for stress testing (for memory leak detection)\nand \"x\" to exit.\n");
	printf("command: ");
	while (true)
	{
		std::cin >> input;
		if (input == 'i')
			{
				if(ChangeScreenSaverStateDBus(true))
					printf("success, inhibition status is %d\n", ScreenSaverStateDBusIsInhibited());
				else
					printf("failure, inhibition status is %d\n", ScreenSaverStateDBusIsInhibited());
				printf("command: ");
			}
		else if (input == 'u')
			{
				if(ChangeScreenSaverStateDBus(false))
					printf("success, inhibition status is %d\n", ScreenSaverStateDBusIsInhibited());
				else
					printf("failure, inhibition status is %d\n", ScreenSaverStateDBusIsInhibited());
				printf("command: ");
			}
		else if (input == 's')
			{
				printf("Stress running\nPress ctl+c to terminate\n");
				while(true)
					{
						ChangeScreenSaverStateDBus(true);
						ChangeScreenSaverStateDBus(false);
					}
			}
		else if (input == 'x')
			{
				return 0;
			}
		else
			{
				printf("Wrong input, Valid inputs are \"i\", \"u\", \"s\" and \"x\"\n");
				printf("command: ");
			}
	}
}
