#ifndef IoTLib_MACRO_UTIL_H
#define IoTLib_MACRO_UTIL_H

#include <stdbool.h>

void IoTLib_perform_managed_array_bounds_check(const int managedArrayLength, const int managedArrayCapacity);
void IoTLib_managed_key_value_array_getFunc_validate_key_index(const int keyIndex);
bool IoTLib_managed_key_value_array_index_is_valid_index(const int keyIndex);

#endif
