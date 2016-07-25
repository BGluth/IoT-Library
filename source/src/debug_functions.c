#include "debug_functions.h"

#include <string.h>
#include "user_settings.h"

extern void (*IoTLib_debugFunction)(char* debugString);

static char* _IoTLib_info_prefix = "INFO: ";
static char* _IoTLib_warning_prefix = "WARNING: ";
static char* _IoTLib_error_prefix = "ERROR: ";

const static size_t _IoTLib_info_prefix_size = 6;
const static size_t _IoTLib_warning_prefix_size = 9;
const static size_t _IoTLib_error_prefix_size = 7;

void IoTLib_debug_info(const char* message)
{
	if (IoTLib_DEBUG_MESSAGE_LEVEL == IoTLib_Debg_ERROR_WARNING_INFO)
		_IoTLib_debug_call(message, _IoTLib_info_prefix, _IoTLib_info_prefix_size);
}

void IoTLib_debug_warning(const char* message)
{
	if (IoTLib_DEBUG_MESSAGE_LEVEL >= IoTLib_Debg_ERROR_WARNING)
		_IoTLib_debug_call(message, _IoTLib_warning_prefix, _IoTLib_warning_prefix_size);
}

void IoTLib_debug_error(const char* message)
{
	if (IoTLib_DEBUG_MESSAGE_LEVEL >= IoTLib_Debg_ERROR)
		_IoTLib_debug_call(message, _IoTLib_error_prefix, _IoTLib_error_prefix_size);

	if (IoTLib_DEBUG_HALT_ON_ERROR)
		_IoTLib_halt();
}

void _IoTLib_debug_call(const char* message, const char* prefix, const size_t prefix_size)
{
	char debugMessageBuffer[prefix_size + strlen(message)];
	strcpy(debugMessageBuffer, prefix);
	strcat(debugMessageBuffer, message);

	IoTLib_debugFunction(debugMessageBuffer);
}

void _IoTLib_halt()
{
	while (1) {}
}


