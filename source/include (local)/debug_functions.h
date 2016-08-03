#ifndef IoTLib_DEBUG_FUNCTIONS_H
#define IoTLib_DEBUG_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdarg.h>

void IoTLib_debug_info(const char* message, ...);
void IoTLib_debug_warning(const char* message, ...);
void IoTLib_debug_error(const char* message, ...);

void _IoTLib_debug_call(const char* message, const char* prefix, const size_t prefix_size, va_list printArgs);
void _IoTLib_halt();

#ifdef __cplusplus
}
#endif

#endif
