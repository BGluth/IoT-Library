#include "macro_util.h"

#include "user_settings.h"
#include "debug_functions.h"

void IoTLib_perform_managed_array_bounds_check(const size_t managedArrayLength, const size_t managedArrayCapacity)
{
	#ifdef DEBUG
		if (managedArrayLength == managedArrayCapacity)
		{
			IoTLib_debug_error("Tried adding an item to an array that was at capacity! (cap and len == %i)", managedArrayLength);
		}
	#endif

}

void IoTLib_managed_key_value_array_getFunc_validate_key_index(const int keyIndex)
{
	#ifdef DEBUG
		if (!IoTLib_managed_key_value_array_index_is_valid_index(keyIndex))
		{
			IoTLib_debug_error("Tried getting a key that does not exist! (index: %i)", keyIndex);
		}
	#endif
}

bool IoTLib_managed_key_value_array_index_is_valid_index(const int keyIndex)
{
	if (keyIndex == -1)
		return false;

	return true;
}
