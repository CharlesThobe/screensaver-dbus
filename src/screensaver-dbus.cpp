#include <dbus/dbus.h>
#define BUS_NAME "org.freedesktop.ScreenSaver"
#define BUS_PATH "/org/freedesktop/ScreenSaver"
#define BUS_INTERFACE "org.freedesktop.ScreenSaver"
static dbus_uint32_t s_screensaver_dbus_cookie;
bool ChangeScreenSaverStateDBus(const bool inhibit_requested, const char* program_name, const char* reason)
{
	// "error_dbus" doesn't need to be cleared in the end with "dbus_message_unref" at least if there is
	// no error set, since calling "dbus_error_free" reinitializes it like "dbus_error_init" after freeing.
	DBusError error_dbus;
	dbus_error_init(&error_dbus);
	DBusConnection* connection = nullptr;
	DBusMessage* message = nullptr;
	DBusMessage* response = nullptr;
	// Initialized here because initializations should be before "goto" statements.
	const char* bus_method = (inhibit_requested) ? "Inhibit" : "UnInhibit";
	// "dbus_bus_get" gets a pointer to the same connection in libdbus, if exists, without creating a new connection.
	// this doesn't need to be deleted, except if there's an error then calling "dbus_connection_unref", to free it,
	// might be better so a new connection is established on the next try.
	if (!(connection = dbus_bus_get(DBUS_BUS_SESSION, &error_dbus)) || (dbus_error_is_set(&error_dbus)))
		goto cleanup;
	if (!(message = dbus_message_new_method_call(BUS_NAME, BUS_PATH, BUS_INTERFACE, bus_method)))
		goto cleanup;
	// Initialize an append iterator for the message, gets freed with the message.
	DBusMessageIter message_itr;
	dbus_message_iter_init_append(message, &message_itr);
	if (inhibit_requested)
	{
		// Append process/window name.
		if (!dbus_message_iter_append_basic(&message_itr, DBUS_TYPE_STRING, &program_name))
			goto cleanup;
		// Append reason for inhibiting the screensaver.
		if (!dbus_message_iter_append_basic(&message_itr, DBUS_TYPE_STRING, &reason))
			goto cleanup;
	}
	else
	{
		// Only Append the cookie.
		if (!dbus_message_iter_append_basic(&message_itr, DBUS_TYPE_UINT32, &s_screensaver_dbus_cookie))
			goto cleanup;
		s_screensaver_dbus_cookie = 0;
	}
	// Send message and get response.
	if (!(response = dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, &error_dbus)) 
		|| dbus_error_is_set(&error_dbus))
		goto cleanup;
	if (inhibit_requested)
	{
		// Get the cookie from the response message.
		if (!dbus_message_get_args(response, &error_dbus, DBUS_TYPE_UINT32, &s_screensaver_dbus_cookie, DBUS_TYPE_INVALID))
			goto cleanup;
	}
	dbus_message_unref(message);
	dbus_message_unref(response);
	return true;
	cleanup:
			if (dbus_error_is_set(&error_dbus))
				dbus_error_free(&error_dbus);
			if (connection)
				dbus_connection_unref(connection);
			if (message)
				dbus_message_unref(message);
			if (response)
				dbus_message_unref(response);
			return false;
}

bool ScreenSaverStateDBusIsInhibited()
{
	return s_screensaver_dbus_cookie;
}
