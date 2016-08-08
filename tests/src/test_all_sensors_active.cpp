#include "test_all_sensors_active.hpp"

#include <string.h>
#include <time.h>

#include "catch.hpp"
#include "fff.h"

#include "testing_utils.hpp"
#include "user_settings.h"
#include "registration_api.h"

static int fake_sensor_poll_return_value = 42;
static char fake_generate_upload_payload_return_value[] = "THIS IS A FAKE UPLOAD PAYLOAD!!! :D";
static float normalEnvironmentTempValue = 25;
static float hotEnvironmentTempValue = 40;
static float coldEnvironemtTempValue = -40;
static const char* sensorNames[3] = {"Sensor1", "Sensor2", "Sensor3"};
static IoTLib_SensorID sensorIDs[3];
static const IoTLib_SensorID tempSensorID = 1;
static const int defaultSensorPollTime = 30;


FAKE_VOID_FUNC(init_function);
FAKE_VOID_FUNC(power_on_function);
FAKE_VALUE_FUNC(void*, sensor_poll_function);
FAKE_VOID_FUNC(store_unsent_data_function, void*);
FAKE_VALUE_FUNC(char*, generate_upload_payload_function, void*);
FAKE_VALUE_FUNC(time_t, retrieve_last_polled_time_function);
FAKE_VOID_FUNC(store_last_polled_time_function, time_t);

FAKE_VOID_FUNC(upload_function, char*);
FAKE_VALUE_FUNC(float, raw_to_temp_function, void*);
FAKE_VALUE_FUNC(time_t, retrieve_last_upload_time_function);
FAKE_VOID_FUNC(store_last_upload_time_function, time_t);
FAKE_VALUE_FUNC(struct IoTLib_RawSensorDataAndSensorID*, retrieve_all_stored_unsent_sensor_data_function);
FAKE_VALUE_FUNC(size_t, get_stored_unsent_data_count_function);

FAKE_VALUE_FUNC(time_t, time, time_t*);


static void debug_function(char* debugString)
{
	printf(debugString);
	printf("\n");
}

static void init_fakes()
{
	sensor_poll_function_fake.return_val = &fake_sensor_poll_return_value;
	generate_upload_payload_function_fake.return_val = fake_generate_upload_payload_return_value;

	retrieve_last_upload_time_function_fake.return_val = 0;
	set_environment_temp(normalEnvironmentTempValue);
}

static void reset()
{
	printf("Resetting...\n");
	clear_registered_data();
	reset_fakes();
	init_fakes();
}

static void reset_fakes()
{
	RESET_FAKE(init_function);
	RESET_FAKE(power_on_function);
	RESET_FAKE(sensor_poll_function);
	RESET_FAKE(store_unsent_data_function);
	RESET_FAKE(generate_upload_payload_function);
	RESET_FAKE(retrieve_last_polled_time_function);
	RESET_FAKE(store_last_polled_time_function);

	RESET_FAKE(upload_function);
	RESET_FAKE(raw_to_temp_function);
	RESET_FAKE(retrieve_last_upload_time_function);
	RESET_FAKE(store_last_upload_time_function);
	RESET_FAKE(retrieve_all_stored_unsent_sensor_data_function);
	RESET_FAKE(get_stored_unsent_data_count_function);

	//RESET_FAKE()
}

static void force_update_function_to_run()
{
	retrieve_last_upload_time_function_fake.return_val = IoTLib_MIN_SECONDS_BETWEEN_UPLOADS + 1;
}

static void register_fake_functions(size_t numSensors)
{
	IoTLib_register_upload_function(upload_function);
	IoTLib_register_debug_function(debug_function);
	IoTLib_register_temp_sensorid_and_raw_to_temp_function(tempSensorID, raw_to_temp_function);
	IoTLib_register_store_last_upload_time_function(store_last_upload_time_function);
	IoTLib_register_retrieve_last_upload_time_function(retrieve_last_upload_time_function);
	IoTLib_register_retrieve_all_stored_unsent_sensor_data_function(retrieve_all_stored_unsent_sensor_data_function);
	IoTLib_register_get_stored_unsent_data_count_function(get_stored_unsent_data_count_function);

	for (int i = 0; i < numSensors; i++)
	{
		IoTLib_SensorID sensorID = IoTLib_register_sensor(sensorNames[i]);
		IoTLib_register_sensor_init_function(sensorID, init_function);
		IoTLib_register_sensor_poll_function(sensorID, sensor_poll_function);
		IoTLib_register_sensor_power_on_function(sensorID, power_on_function);
		IoTLib_register_sensor_store_unsent_data_function(sensorID, store_unsent_data_function);
		IoTLib_register_sensor_generate_upload_payload_function(sensorID, generate_upload_payload_function);
		IoTLib_register_sensor_retrieve_last_polled_time_function(sensorID, retrieve_last_polled_time_function);
		IoTLib_register_sensor_store_last_polled_time_function(sensorID, store_last_polled_time_function);

		sensorIDs[i] = sensorID;
	}

	set_two_sensors_max_temp_below_hot_env_temp();
	set_two_sensors_min_temps_above_cold_env_temp();
	set_third_sensor_min_max_temp_within_boundaries_of_hot_and_cold_temp();

	set_all_sensors_to_have_same_poll_frequency(numSensors, defaultSensorPollTime);
}

static void set_all_sensors_to_have_same_poll_frequency(size_t numSensors, int frequency)
{
	for (size_t i = 0; i < numSensors; i++)
	{
		IoTLib_register_sensor_poll_frequency(sensorIDs[i], frequency);
	}
}

static void set_environment_temp(float temp)
{
	raw_to_temp_function_fake.return_val = temp;
}



static void set_two_sensors_max_temp_below_hot_env_temp()
{
	float maxOperatingTempBelowHotEnvironmentTemp = hotEnvironmentTempValue - 10;
	set_two_sensors_min_or_max_temp_to_value(IoTLib_register_sensor_max_operating_temp, maxOperatingTempBelowHotEnvironmentTemp);
}

static void set_two_sensors_min_temps_above_cold_env_temp()
{
	float minOperatingTempAboveColdEnvironmentTemp = coldEnvironemtTempValue + 10;
	set_two_sensors_min_or_max_temp_to_value(IoTLib_register_sensor_min_operating_temp, minOperatingTempAboveColdEnvironmentTemp);
}

static void set_third_sensor_min_max_temp_within_boundaries_of_hot_and_cold_temp()
{
	IoTLib_register_sensor_max_operating_temp(sensorIDs[2], 100);
	IoTLib_register_sensor_min_operating_temp(sensorIDs[2], -100);
}

static void set_two_sensors_min_or_max_temp_to_value(
	void (*setOperatingTempFunc)(IoTLib_SensorID sensorID, float minOrMaxOperTemp),
	float sensorOperatingTemp)
{
	for (size_t i = 0; i < 2; i++)
	{
		setOperatingTempFunc(sensorIDs[i], sensorOperatingTemp);
	}
}

static void set_current_time_so_default_poll_time_sensors_poll_and_device_uploads()
{
	set_current_time(std::max(defaultSensorPollTime, IoTLib_MIN_SECONDS_BETWEEN_UPLOADS) + 1);
}

static void set_current_time(int currentTime)
{
	time_fake.return_val = currentTime;
}

////////////////////////
// ACTUAL TESTS!!!! YAY!
////////////////////////

SCENARIO("Run function calls registered functions appropriately")
{
	reset();

	GIVEN("All needed functions have been registered for 3 sensors")
	{
		register_fake_functions(3);
		set_current_time_so_default_poll_time_sensors_poll_and_device_uploads();

		GIVEN("Two sensors have max operating temperatures below the current environment temperature")
		{
			set_environment_temp(hotEnvironmentTempValue);
			init_and_run();

			THEN("Only 2 sensors should be polled.")
			{
				// Two sensors since we need to poll the temp sensor to get the current env temp.
				REQUIRE(sensor_poll_function_fake.call_count == 2);
			}
		}

		GIVEN("Two sensors have min operating temperatures above the current environment temperature")
		{
			set_environment_temp(coldEnvironemtTempValue);
			init_and_run();

			THEN("Only 2 sensors should be polled.")
			{
				REQUIRE(sensor_poll_function_fake.call_count == 2);
			}
		}

		GIVEN("Not enough time has passed for any sensors to be polled")
		{
			set_current_time(0);
			init_and_run();
			THEN("No sensors should be polled")
			{
				REQUIRE(sensor_poll_function_fake.call_count == 0);
			}
		}
	}


}
