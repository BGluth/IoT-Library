#ifndef IoTLib_UTIL_FUNCTIONS_H
#define IoTLib_UTIL_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "user_settings.h"

uint32_t IoTLib_calculate_time_in_seconds_until_next_sensor_polling();
IoTLib_time_t IoTLib_calculate_time_difference(IoTLib_time_t endTime, IoTLib_time_t startTime);
#ifdef __cplusplus
}
#endif

#endif
