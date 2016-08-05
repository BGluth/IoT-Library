/* This is a "fake" version of the real user_settings.h defined in the IoT_Library source.
 * These settings are used for testing.
 * The build system will provide this header first before the read user_settings,
 * so when building for testing this header will always be used instead of the real one.
 */

#ifndef IoTLib_USER_SETTINGS_H
#define IoTLib_USER_SETTINGS_H

// Library Settings

enum IoTLib_DebugLevel
{
	IoTLib_DEBGLVL_NONE = 0,
	IoTLib_DEBGLVL_ERROR = 1,
	IoTLib_DEBGLVL_ERROR_WARNING = 2,
	IoTLib_DEBGLVL_ERROR_WARNING_INFO = 3
};

#define IoTLib_SENSOR_COUNT 3

#define IoTLib_NUM_SENSORS_WITH_MIN_TEMP 2
#define IoTLib_NUM_SENSORS_WITH_MAX_TEMP 2
#define IoTLib_NUM_POWER_ON_FUNCTIONS 3
#define IoTLib_NUM_NON_SENSOR_INIT_FUNCTIONS 0

#define IoTLib_MIN_SECONDS_BETWEEN_UPLOADS 900

#define IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES 1

#define IoTLib_DEBUG_MESSAGE_LEVEL IoTLib_DEBGLVL_ERROR_WARNING_INFO
#define IoTLib_DEBUG_HALT_ON_ERROR 0

// Helper macros (No touching!)
// **************************************

#define IoTLib_TOTAL_INIT_FUNCTIONS IoTLib_SENSOR_COUNT + IoTLib_NON_SENSOR_INIT_FUNCTIONS










// **************************************

#endif
