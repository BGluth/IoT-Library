#ifndef IoTLib_UTIL_FUNCTIONS_H
#define IoTLib_UTIL_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

double IoTLib_calculate_time_in_seconds_until_next_sensor_polling();
time_t _IoTLib_get_current_time();

#ifdef __cplusplus
}
#endif

#endif
