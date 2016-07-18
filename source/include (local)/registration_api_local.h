#ifndef IoTLib_SENSOR_REGISTRATION_LOCAL_H
#define IoTLib_SENSOR_REGISTRATION_LOCAL_H

#include <stdbool.h>
#include "general_macros.h"

bool _IoTLib_checkForUnsetFunctions();

// Local macros
#define IoTLib_define_SensorID_with_data_struct(structName, dataType) \
	struct structName \
	{ \
		IoTLib_SensorID id; \
		dataType data; \
	};

#endif
