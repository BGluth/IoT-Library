#ifndef IoTLib_DEBUG_FUNCTIONS_H
#define IoTLib_DEBUG_FUNCTIONS_H

#include <stddef.h>

void IoTLib_debug_info(const char* message);
void IoTLib_debug_warning(const char* message);
void IoTLib_debug_error(const char* message);

void _IoTLib_debug_call(const char* message, const char* prefix, const size_t prefix_size);
void _IoTLib_halt();

#endif
