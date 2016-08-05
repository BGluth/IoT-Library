#include <stddef.h>

#include "typedefs.h"

static void init_fakes();
static void reset();
static void reset_global_lists();
static void reset_fakes();
static void force_update_function_to_run();
static void register_fake_functions(size_t numSensors);
static void set_all_functions_to_have_same_poll_frequency(size_t numSensors);
static void set_two_sensors_max_temp_below_current_temp(int numSensors);
static void set_two_sensors_min_temp_below_current_temp(int numSensors);
static void set_two_sensors_min_or_max_temp_to_value(void (*setOperatingTempFunc)(IoTLib_SensorID sensorID, float minOrMaxOperTemp), float sensorOperatingTemp);
