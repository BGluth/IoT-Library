#include "user_settings.h"

#include "registration_api.h"
#include "registration_api_local.h"

#include "debug_functions.h"
#include "managed_array_definitions.h"
#include "run_helper_functions.h"
#include "user_settings.h"
#include "vector.h"

IoTLib_initialize_managed_key_value_array(IoTLib_sensorIDsAndNames,
		struct IoTLib_MngdKVArray_SnsrIDString,
		IoTLib_SensorID, const char*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_initFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_pollFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_rawDataToStringFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_powerOnFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_NUM_POWER_ON_FUNCTIONS);

IoTLib_initialize_managed_key_value_array(IoTLib_storeUnsentDataFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_retrieveUnsentDataFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_generateUploadPayloadFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_retrieveSensorLastPolledTimeFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_storeSensorLastPolledTimeFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorPollFrequencies,
		struct IoTLib_MngdKVArray_SnsrIDTime_t,
		IoTLib_SensorID, IoTLib_time_t, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMinTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MIN_TEMP);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMaxTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MAX_TEMP);

struct IoTLib_Vector IoTLib_waitlist_funcs;

struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc = {0, NULL};
void (*IoTLib_uploadFunction)(char* urlUploadString) = NULL;
void (*IoTLib_debugFunction)(char* debugString) = NULL;
void (*IoTLib_storeLastUploadTimeFunc)(IoTLib_time_t lastActiveTime) = NULL;
IoTLib_time_t (*IoTLib_retrieveLastUploadTimeFunc)() = NULL;
struct IoTLib_RawSensorDataAndSensorID* (*IoTLib_retrieveAllUnsentDataFunc)() = NULL;
size_t (*IoTLib_getStoredUnsentDataCountFunc)() = NULL;
IoTLib_time_t (*IoTLib_getCurrentTimeFunction)() = NULL;
double (*IoTLib_convertTimeTypeToSecondsFunc)(IoTLib_time_t rawTime) = NULL;

void IoTLib_run()
{
	IoTLib_debug_info("Entering run function...");
	_IotLib_run_implementation();
}

void IoTLib_sensor_registration_init()
{
	_IoTLib_check_for_unset_functions_if_debugging();
	IoTLib_vector_init(&IoTLib_waitlist_funcs);
}

// sensorName is used for debugging
IoTLib_SensorID IoTLib_register_sensor(const char* sensorName)
{
	size_t id = IoTLib_sensorIDsAndNames.length;

	IoTLib_MKV_insert(&IoTLib_sensorIDsAndNames,
			IoTLib_MngdKVArray_SnsrIDString, id, sensorName);

	return id;
}

void IoTLib_register_sensor_init_function(IoTLib_SensorID sensorID, void (*sensorInitFunc)())
{
	_IoTLib_debug_print_registration_function_call("init function", sensorID);

	IoTLib_MKV_insert(&IoTLib_initFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)sensorInitFunc);
}

void IoTLib_register_sensor_poll_function(IoTLib_SensorID sensorID, void* (*pollSensorFunc)())
{
	_IoTLib_debug_print_registration_function_call("poll sensor function", sensorID);

	IoTLib_MKV_insert(&IoTLib_pollFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)pollSensorFunc);
}

void IoTLib_register_sensor_convert_raw_sensor_data_to_string_function(IoTLib_SensorID sensorID, char* (*dataToStringFunc)(void* rawSensorData))
{
	_IoTLib_debug_print_registration_function_call("raw sensor data to string function", sensorID);

	IoTLib_MKV_insert(&IoTLib_rawDataToStringFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)dataToStringFunc);
}

void IoTLib_register_sensor_power_on_function(IoTLib_SensorID sensorID, void (*powerOnFunc)())
{
	_IoTLib_debug_print_registration_function_call("power on function", sensorID);

	IoTLib_MKV_insert(&IoTLib_powerOnFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)powerOnFunc);
}

void IoTLib_register_sensor_store_unsent_data_function(IoTLib_SensorID sensorID, void (*storeUnsentFunc)(void* rawSensorData))
{
	_IoTLib_debug_print_registration_function_call("store unsent data function", sensorID);

	IoTLib_MKV_insert(&IoTLib_storeUnsentDataFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)storeUnsentFunc);
}

void IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData))
{
	_IoTLib_debug_print_registration_function_call("generate upload payload function", sensorID);

	IoTLib_MKV_insert(&IoTLib_generateUploadPayloadFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)generateUploadPayloadFunc);
}

void IoTLib_register_sensor_retrieve_last_polled_time_function(IoTLib_SensorID sensorID, IoTLib_time_t (*retrieveSensorLastPolledTimeFunc)())
{
	_IoTLib_debug_print_registration_function_call("retrieve last polled time function", sensorID);

	IoTLib_MKV_insert(&IoTLib_retrieveSensorLastPolledTimeFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)retrieveSensorLastPolledTimeFunc);
}

void IoTLib_register_sensor_store_last_polled_time_function(IoTLib_SensorID sensorID, void (*storeSensorLastPolledTimeFunc)(IoTLib_time_t lastPollTime))
{
	_IoTLib_debug_print_registration_function_call("store last polled time function", sensorID);

	IoTLib_MKV_insert(&IoTLib_storeSensorLastPolledTimeFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, (void*)storeSensorLastPolledTimeFunc);
}

void IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, IoTLib_time_t sensorPollFrequency)
{
	_IoTLib_debug_print_registration_function_call("sensor poll frequency", sensorID);

	IoTLib_MKV_insert(&IoTLib_sensorPollFrequencies,
		IoTLib_MngdKVArray_SnsrIDTime_t, sensorID, sensorPollFrequency);
}

void IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp)
{
	_IoTLib_debug_print_registration_function_call("min operating temp", sensorID);

	IoTLib_MKV_insert(&IoTLib_sensorMinTemps,
		IoTLib_MngdKVArray_SnsrIDFloat, sensorID, minTemp);
}

void IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp)
{
	_IoTLib_debug_print_registration_function_call("max operating temp", sensorID);

	IoTLib_MKV_insert(&IoTLib_sensorMaxTemps,
		IoTLib_MngdKVArray_SnsrIDFloat, sensorID, maxTemp);
}


void IoTLib_register_upload_function(void (*uploadFunction)(char* urlUploadString))
{
	IoTLib_uploadFunction = uploadFunction;
}

void IoTLib_register_debug_function(void (*debugFunction)(char* debugString))
{
	IoTLib_debugFunction = debugFunction;
}

void IoTLib_register_temp_sensorid_and_raw_to_temp_function(IoTLib_SensorID tempSensorID, float (*rawToTempFunc)(void* rawSensorData))
{
	struct IoTLib_SnsrIDDataPtr sensorIDWithReadFunc = {.id = tempSensorID, .data = (void*)rawToTempFunc};
	IoTLib_tempSnsrIDAndRawToFloatFunc = sensorIDWithReadFunc;
}

void IoTLib_register_store_last_upload_time_function(void (*storeLastUploadTimeFunc)(IoTLib_time_t lastUploadTime))
{
	IoTLib_storeLastUploadTimeFunc = storeLastUploadTimeFunc;
}

void IoTLib_register_retrieve_last_upload_time_function(IoTLib_time_t (*retireveLastUploadTimeFunc)())
{
	IoTLib_retrieveLastUploadTimeFunc = retireveLastUploadTimeFunc;
}

void IoTLib_register_get_stored_unsent_data_count_function(size_t (*getStoredUnsentDataCountFunc)())
{
	IoTLib_getStoredUnsentDataCountFunc = getStoredUnsentDataCountFunc;
}

void IoTLib_register_retrieve_all_stored_unsent_sensor_data_function(
	struct IoTLib_RawSensorDataAndSensorID* (*retrieveAllUnsentDataFunc)())
{
	IoTLib_retrieveAllUnsentDataFunc = retrieveAllUnsentDataFunc;
}

void IoTLib_register_get_current_time_function(IoTLib_time_t (*getCurrentTimeFunc)())
{
	IoTLib_getCurrentTimeFunction = getCurrentTimeFunc;
}

void IoTLib_register_convert_time_type_to_seconds_func(double (*convertTimeTypeToSecondsFunc)(IoTLib_time_t rawTime))
{
	IoTLib_convertTimeTypeToSecondsFunc = convertTimeTypeToSecondsFunc;
}


void IoTLib_add_task_to_waitlist(bool (*checkAndHandleTaskCompletionFunc)())
{
	IoTLib_debug_info("Adding task to the wait list...");

	IoTLib_vector_add(&IoTLib_waitlist_funcs, (void*)checkAndHandleTaskCompletionFunc);
}


// Local functions:
void _IoTLib_check_for_unset_functions_if_debugging()
{
	if (DEBUG)
	{
		if (IoTLib_tempSnsrIDAndRawToFloatFunc.data == NULL && IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES)
			IoTLib_debug_error("No sensor specified for polling outside temperature!");

		if (IoTLib_uploadFunction == NULL)
			IoTLib_debug_error("No upload function registered!");

		if (IoTLib_storeLastUploadTimeFunc == NULL)
			IoTLib_debug_error("No store last upload time function registered!");

		if (IoTLib_retrieveLastUploadTimeFunc == NULL)
			IoTLib_debug_error("No retrieve last upload time function registered!");

		if (IoTLib_retrieveAllUnsentDataFunc == NULL)
			IoTLib_debug_error("No retrieve all unsent data function registered!");

		if (IoTLib_getStoredUnsentDataCountFunc == NULL)
			IoTLib_debug_error("No get stored unsent data count function registered!");


		if (IoTLib_sensorIDsAndNames.length < IoTLib_SENSOR_COUNT)
		{
			IoTLib_debug_error("Less sensors actually registered than defined in user settings!"
				" (actual: %i settings: %i)", IoTLib_sensorIDsAndNames.length, IoTLib_SENSOR_COUNT);
		}

		if (IoTLib_powerOnFunctions.length < IoTLib_NUM_POWER_ON_FUNCTIONS)
		{
			IoTLib_debug_error("Less power on functions than defined in user settings!"
				" (actual: %i settings: %i)", IoTLib_pollFunctions.length, IoTLib_NUM_POWER_ON_FUNCTIONS);
		}

		if (IoTLib_sensorMaxTemps.length < IoTLib_NUM_SENSORS_WITH_MAX_TEMP && IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES)
		{
			IoTLib_debug_error("Less sensors with max operating temps than defined in user settings!"
				" (actual: %i settings: %i)", IoTLib_sensorMaxTemps.length, IoTLib_NUM_SENSORS_WITH_MAX_TEMP);
		}

		if (IoTLib_sensorMinTemps.length < IoTLib_NUM_SENSORS_WITH_MIN_TEMP && IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES)
		{
			IoTLib_debug_error("Less sensors with min operating temps than defined in user settings!"
				" (actual: %i settings: %i)", IoTLib_sensorMinTemps.length, IoTLib_NUM_SENSORS_WITH_MIN_TEMP);
		}


		if (IoTLib_initFunctions.length < IoTLib_sensorIDsAndNames.length)
		{
			IoTLib_debug_error("Less init functions than sensors!"
				" (actual: %i sensor count: %i)", IoTLib_initFunctions.length, IoTLib_sensorIDsAndNames.length);
		}

		if (IoTLib_pollFunctions.length < IoTLib_sensorIDsAndNames.length)
		{
			IoTLib_debug_error("Less poll functions than sensors!"
				" (actual: %i sensor count: %i)", IoTLib_pollFunctions.length, IoTLib_sensorIDsAndNames.length);
		}

		if (IoTLib_storeUnsentDataFunctions.length < IoTLib_sensorIDsAndNames.length)
		{
			IoTLib_debug_error("Less store unsent data functions than sensors!"
				" (actual: %i sensor count: %i)", IoTLib_storeUnsentDataFunctions.length, IoTLib_sensorIDsAndNames.length);
		}

		if (IoTLib_generateUploadPayloadFunctions.length < IoTLib_sensorIDsAndNames.length)
		{
			IoTLib_debug_error("Less generate upload payload functions than sensors!"
				" (actual: %i sensor count: %i)", IoTLib_generateUploadPayloadFunctions.length, IoTLib_sensorIDsAndNames.length);
		}

		if (IoTLib_retrieveSensorLastPolledTimeFunctions.length < IoTLib_sensorIDsAndNames.length)
		{
			IoTLib_debug_error("Less retrieve last poll time functions than sensors!"
				" (actual: %i sensor count: %i)", IoTLib_retrieveSensorLastPolledTimeFunctions.length, IoTLib_sensorIDsAndNames.length);
		}

		if (IoTLib_storeSensorLastPolledTimeFunctions.length < IoTLib_sensorIDsAndNames.length)
		{
			IoTLib_debug_error("Less store last poll time functions than sensors!"
				" (actual: %i sensor count: %i)", IoTLib_storeSensorLastPolledTimeFunctions.length, IoTLib_sensorIDsAndNames.length);
		}

		if (IoTLib_sensorPollFrequencies.length < IoTLib_sensorIDsAndNames.length)
		{
			IoTLib_debug_error("Less registered sensor poll frequencies than sensors!"
				" (actual: %i sensor count: %i)", IoTLib_sensorPollFrequencies.length, IoTLib_sensorIDsAndNames.length);
		}
	}
}

// Debug functions:
static void _IoTLib_debug_print_registration_function_call (const char* registrationType, IoTLib_SensorID sensorID)
{
	IoTLib_debug_info("Registering %s for sensor ID: %i", registrationType, sensorID);
}
