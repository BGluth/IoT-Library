#include "sensorID.h"

int IoTLib_sensorid_get_key_index(const IoTLib_SensorID* keyArray, const int arrayLength, const IoTLib_SensorID keyToSearchFor)
{
	// Linear search for now...
	for (int i = 0; i < arrayLength; i++)
	{
		if (keyArray[i] == keyToSearchFor)
			return i;
	}

	return -1;
}
