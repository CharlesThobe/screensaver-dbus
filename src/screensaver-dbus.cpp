#include <dbus/dbus.h>

#ifdef TEST_BUILD
static dbus_uint32_t s_cookie;
#endif

bool SetScreensaverInhibitDBus(const bool inhibit_requested, const char* program_name, const char* reason)
{
#ifndef TEST_BUILD
	static dbus_uint32_t s_cookie;
#endif
	const char* bus_method = (inhibit_requested) ? "Inhibit" : "UnInhibit";
	DBusError error_dbus;
	DBusConnection* connection = nullptr;
	static DBusConnection* s_comparison_connection;
	DBusMessage* message = nullptr;
	DBusMessage* response = nullptr;
	DBusMessageIter message_itr;

	dbus_error_init(&error_dbus);
	// Calling dbus_bus_get() after the first time returns a pointer to the existing connection.
	connection = dbus_bus_get(DBUS_BUS_SESSION, &error_dbus);
	if (!connection || (dbus_error_is_set(&error_dbus)))
		goto cleanup;
	if (s_comparison_connection != connection)
	{
		dbus_connection_set_exit_on_disconnect(connection, false);
		s_cookie = 0;
		s_comparison_connection = connection;
	}
	message = dbus_message_new_method_call("org.freedesktop.ScreenSaver", "/org/freedesktop/ScreenSaver", "org.freedesktop.ScreenSaver", bus_method);
	if (!message)
		goto cleanup;
	// Initialize an append iterator for the message, gets freed with the message.
	dbus_message_iter_init_append(message, &message_itr);
	if (inhibit_requested)
	{
		// Guard against repeat inhibitions which would add extra inhibitors each generating a different cookie.
		if (s_cookie)
			goto cleanup;
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
		if (!dbus_message_iter_append_basic(&message_itr, DBUS_TYPE_UINT32, &s_cookie))
			goto cleanup;
	}
	// Send message and get response.
	response = dbus_connection_send_with_reply_and_block(connection, message, DBUS_TIMEOUT_USE_DEFAULT, &error_dbus);
	if (!response || dbus_error_is_set(&error_dbus))
		goto cleanup;
	s_cookie = 0;
	if (inhibit_requested)
	{
		// Get the cookie from the response message.
		if (!dbus_message_get_args(response, &error_dbus, DBUS_TYPE_UINT32, &s_cookie, DBUS_TYPE_INVALID) || dbus_error_is_set(&error_dbus))
			goto cleanup;
	}
	dbus_message_unref(message);
	dbus_message_unref(response);
	return true;
cleanup:
	if (dbus_error_is_set(&error_dbus))
		dbus_error_free(&error_dbus);
	if (message)
		dbus_message_unref(message);
	if (response)
		dbus_message_unref(response);
	return false;
}

#ifdef TEST_BUILD
bool ScreensaverInhibitorIsUp()
{
	return s_cookie;
}
#endif
