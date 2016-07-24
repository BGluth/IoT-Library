#include "sensorID.h"

size_t IoTLib_sensorid_get_key_index(const IoTLib_SensorID* keyArray, const size_t arrayLength, const IoTLib_SensorID keyToSearchFor)
{
	// Linear search for now...
	for (size_t i = 0; i < arrayLength; i++)
	{
		if (keyArray[i] == keyToSearchFor)
			return i;
	}

	return -1;
}
