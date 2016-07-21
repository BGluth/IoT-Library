#ifndef IoTLib_USER_SETTINGS_H
#define IoTLib_USER_SETTINGS_H

// Library Settings

#define IoTLib_SENSOR_COUNT 1
#define IoTLib_NUM_SENSORS_WITH_MIN_TEMP 1
#define IoTLib_NUM_SENSORS_WITH_MAX_TEMP 1
#define IoTLib_NUM_POWER_ON_FUNCTIONS 1

#define IoTLib_NON_SENSOR_INIT_FUNCTIONS 1

#define IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES 1

// Helper macros (No touching!)
// **************************************

#define IoTLib_TOTAL_INIT_FUNCTIONS IoTLib_SENSOR_COUNT + IoTLib_NON_SENSOR_INIT_FUNCTIONS









// **************************************

#endif
