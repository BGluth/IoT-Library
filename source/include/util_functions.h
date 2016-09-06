/**
 * @defgroup Util_API
 * @{
 */

/** \file util_functions.h
 * \brief Various util funcions that are not directly part of the core API.
 *
 * Provides miscellaneous functions (currently only time related) that are part
 * of the library's API.
 */

#ifndef IoTLib_UTIL_FUNCTIONS_H
#define IoTLib_UTIL_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "user_settings.h"

/** \brief Returns the least amount of time in seconds until the next sensor can
 * be polled.
 *
 * Intended use is for determining how long the device needs to sleep until the
 * next poll.
 *
 * @return The amount of seconds as a double until the next sensor can be polled.
 *
 * @see IoTLib_register_sensor_store_last_polled_time_function(IoTLib_SensorID sensorID, void (*storeSensorLastPolledTimeFunc)(IoTLib_TIME_T lastPollTime))
 */
double IoTLib_calculate_time_in_seconds_until_next_sensor_polling();

/** \brief Returns the time difference between two times in seconds.
 *
 * @param endTime The time that you want to determine how far apart it is from the startTime.
 * @param startTime The time that is your reference point.
 */
double IoTLib_calculate_time_difference_in_seconds(IoTLib_TIME_T endTime, IoTLib_TIME_T startTime);
#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
