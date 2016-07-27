#ifndef IoTLib_SENSOR_REGISTRATION_LOCAL_H
#define IoTLib_SENSOR_REGISTRATION_LOCAL_H

#include <stdbool.h>
#include "general_macros.h"

bool _IoTLib_check_for_unset_functions();

// Debug functions
static void _IoTLib_debug_print_registration_function_call (char* registrationType, IoTLib_SensorID sensorID);

#endif
