#ifndef IoTLib_MACRO_UTIL_H
#define IoTLib_MACRO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

void IoTLib_perform_managed_array_bounds_check(const size_t managedArrayLength, const size_t managedArrayCapacity);
void IoTLib_managed_key_value_array_getFunc_validate_key_index(const int keyIndex);
bool IoTLib_managed_key_value_array_index_is_valid_index(const int keyIndex);

#ifdef __cplusplus
}
#endif

#endif
