#include <dbus/dbus.h>
static DBusConnection* connection = nullptr;
static dbus_uint32_t inhibit_cookie;
bool Send_ScreenSaver_Inhibit(bool inhibit_requested = true, char* program_name = (char*)"ignored", char* reason = (char*)"ignored")
{
	DBusError error_dbus;
	dbus_error_init(&error_dbus);
	/*Initialize connection.
	If a connection to the bus already exists, then that connection is returned.*/
	connection = dbus_bus_get(DBUS_BUS_SESSION, &error_dbus);
	if (!connection || dbus_error_is_set(&error_dbus))
	{
		dbus_error_free(&error_dbus);
		return false;
	}
	char* dbus_method;
	if (inhibit_requested)
	{
		dbus_method = (char*)"Inhibit";
	}
	else
	{
		dbus_method = (char*)"UnInhibit";
	}
	DBusMessage* message = dbus_message_new_method_call("org.freedesktop.ScreenSaver",
														"/org/freedesktop/ScreenSaver",
														"org.freedesktop.ScreenSaver",
														dbus_method);
	if (!message)
	{
		return false;
	}
	//Initialize an appendable iterator for the message, gets freed with the message.
	DBusMessageIter message_itr;
	dbus_message_iter_init_append(message, &message_itr);
	if (inhibit_requested)
	{
		//Append process/window name.
		if (!dbus_message_iter_append_basic(&message_itr, DBUS_TYPE_STRING, &program_name))
			{
				dbus_message_unref(message);
				return false;
			}
		//Append reason for inhibiting the screensaver.
		if (!dbus_message_iter_append_basic(&message_itr, DBUS_TYPE_STRING, &reason))
			{
				dbus_message_unref(message);
				return false;
			}
	}
	else
	{
		//Handle the cookie.
		if (!dbus_message_iter_append_basic(&message_itr, DBUS_TYPE_UINT32, &inhibit_cookie))
		{
			dbus_message_unref(message);
			return false;
		}
	}
	DBusMessage* response = dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, &error_dbus);
	if (!response || dbus_error_is_set(&error_dbus))
	{
		dbus_error_free(&error_dbus);
		dbus_message_unref(message);
		return false;
	}
	//Get the cookie when inhibiting the screensaver.
	if (inhibit_requested)
	{
		//Initiates an iterator of the response message to get elements out of it.
		DBusMessageIter response_itr;
		dbus_message_iter_init(response, &response_itr);
		if (dbus_message_iter_get_arg_type(&response_itr) != DBUS_TYPE_UINT32)
		{
			dbus_message_unref(message);
			return false;
		}
		dbus_message_iter_get_basic(&response_itr, &inhibit_cookie);
	}
	dbus_message_unref(message);
	/*Reset connection on uninhibit, helpful in case of a dbus crash, reset or reinstallation.
	these case are probably a red herring.*/
	/*
	if (!inhibit_requested)
	{
		dbus_connection_unref(connection);
	}
	*/
	return true;
	
}

#include <mutex>
int main(){
	//while(true){looped();}
	Send_ScreenSaver_Inhibit(true, (char*)"yeah", (char*)"nah");
	printf("Ihibited\nPress enter to uninhibit.\n");
	getchar();
	Send_ScreenSaver_Inhibit(false, (char*)"yeah", (char*)"nah");
	printf("uninhibited\nPress enter to exit.\n");
	getchar();
	printf("checking for leaks");
	while(true)
	{
		Send_ScreenSaver_Inhibit(true, (char*)"yeah", (char*)"nah");
		Send_ScreenSaver_Inhibit(false, (char*)"yeah", (char*)"nah");
	}

	return 0;
}