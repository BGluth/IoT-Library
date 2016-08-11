#include <stddef.h>
#include <stdint.h>

#include "user_settings.h"
#include "typedefs.h"

static void init_fakes();
static void reset();
static void reset_fakes();
static void force_update_function_to_run();
static void register_fake_functions(size_t numSensors);
static double convert_time_type_to_seconds(IoTLib_time_t rawTime);
static void set_all_functions_to_have_same_poll_frequency(size_t numSensors);
static void set_environment_temp(float temp);
static void set_two_sensors_max_temp_below_hot_env_temp();
static void set_two_sensors_min_temps_above_cold_env_temp();
static void set_two_sensors_min_or_max_temp_to_value(void (*setOperatingTempFunc)(IoTLib_SensorID sensorID, float minOrMaxOperTemp), float sensorOperatingTemp);
static void set_third_sensor_min_max_temp_within_boundaries_of_hot_and_cold_temp();
static void set_all_sensors_to_have_same_poll_frequency(size_t numSensors, IoTLib_time_t frequency);
static void set_current_time_so_default_poll_time_sensors_poll_and_device_uploads();
static void set_current_time_so_default_poll_time_sensors_poll_but_device_does_not_upload();
static void set_current_time(IoTLib_time_t currentTime);
