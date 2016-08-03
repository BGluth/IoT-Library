#ifndef IoTLib_SENSORID_H
#define IoTLib_SENSORID_H

#include <stddef.h>
#include "typedefs.h"

size_t IoTLib_sensorid_get_key_index(const IoTLib_SensorID* keyArray, const size_t arrayLength, const IoTLib_SensorID keyToSearchFor);

#endif
