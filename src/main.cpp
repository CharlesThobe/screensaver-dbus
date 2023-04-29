#include "screensaver-dbus.h"
#include <stdio.h>
int main(){
	ChangeScreenSaverStateDBus(true, "Test ProgName", "Test Reason");
	printf("Ihibited\nPress enter to uninhibit.\n");
	getchar();
	ChangeScreenSaverStateDBus(false, "Test ProgName", "Test Reason");
	printf("uninhibited\nPress enter to test for memory leaks.\n");
	getchar();
	printf("Stress running\nPress ctl+c to terminate\n");
	while(true)
	{
		ChangeScreenSaverStateDBus(true, "Test ProgName", "Test Reason");
		ChangeScreenSaverStateDBus(false, "Test ProgName", "Test Reason");
	}

	return 0;
}