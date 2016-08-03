#ifndef IoTLib_SENSORID_H
#define IoTLib_SENSORID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "typedefs.h"

size_t IoTLib_sensorid_get_key_index(const IoTLib_SensorID* keyArray, const size_t arrayLength, const IoTLib_SensorID keyToSearchFor);

#ifdef __cplusplus
}
#endif

#endif
