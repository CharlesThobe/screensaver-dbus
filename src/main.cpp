#include "screensaver-dbus.h"
#include <stdio.h>
int main(){
	ChangeScreenSaverStateDBus(true, (char*)"Test ProgName", (char*)"Test Reason");
	printf("Ihibited\nPress enter to uninhibit.\n");
	getchar();
	ChangeScreenSaverStateDBus(false, (char*)"Test ProgName", (char*)"Test Reason");
	printf("uninhibited\nPress enter to test for memory leaks.\n");
	getchar();
	printf("Stress running\nPress ctl+c to terminate\n");
	while(true)
	{
		ChangeScreenSaverStateDBus(true, (char*)"Test ProgName", (char*)"Test Reason");
		ChangeScreenSaverStateDBus(false, (char*)"Test ProgName", (char*)"Test Reason");
	}

	return 0;
}