#ifndef IOTLIB_SENSOR_REGISTRATION_LOCAL_H
#define IOTLIB_SENSOR_REGISTRATION_LOCAL_H

#include <stdbool.h>
#include "general_macros.h"

bool _IoTLib_checkForUnsetFunctions();


// Macros
#define define_SensorID_with_type(type_name, type) \
struct type_name \
{ \
	IOTLIB_SensorID id; \
	type data; \
};

#define initialize_sensorIDAndData_struct_and_add_to_array(array, struct_type, sensorID, actualData) \
	struct struct_type idAndData = {.id = sensorID, .data = actualData}; \
	array_add(array, idAndData);

#endif
