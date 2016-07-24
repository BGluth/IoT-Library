#include "macro_util.h"

void IoTLib_perform_managed_array_bounds_check(const size_t managedArrayLength, const size_t managedArrayCapacity)
{
	if (DEBUG)
	{
		if (managedArrayLength == managedArrayCapacity)
		{
			// TODO: IoTLib_Debug("Tried adding an item to an array that was at capacity!");
			// TODO: Halt
		}
	}
}

void IoTLib_managed_key_value_array_getFunc_validate_key_index(const size_t keyIndex)
{
	if (DEBUG)
	{
		if (!IoTLib_managed_key_value_array_index_is_valid_index(keyIndex))
		{
			// TODO: IoTLib_Debug("Tried getting a key that does not exist!");
			// TODO: Halt
		}
	}
}

bool IoTLib_managed_key_value_array_index_is_valid_index(const size_t keyIndex)
{
	if (keyIndex == -1)
		return false;

	return true;
}
