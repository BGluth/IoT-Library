#include "macro_util.h"

void IoTLib_perform_managed_array_bounds_check(const int* managedArrayLength, const int* managedArrayCapacity)
{
	if (DEBUG)
	{
		if (managedArrayLength == managedArrayCapacity)
		{
			//IoTLib_Debug("Tried adding an item to an array that was at capacity!");
			// TODO: Halt
		}
	}
}
