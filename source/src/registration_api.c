#include "registration_api.h"
#include "registration_api_local.h"

#include "managed_array_definitions.h"
#include "run_helper_functions.h"
#include "user_settings.h"
#include "vector.h"

IoTLib_initialize_managed_key_value_array(IoTLib_sensorIDsAndNames,
		struct IoTLib_MngdKVArray_SnsrIDString,
		IoTLib_SensorID, char*, IoTLib_SENSOR_COUNT);

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

IoTLib_initialize_managed_key_value_array(IoTLib_storeDataUnsentFunctions,
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
		struct IoTLib_MngdKVArray_SnsrIDInt,
		IoTLib_SensorID, int, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMinTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MIN_TEMP);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMaxTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MAX_TEMP);

struct IoTLib_Vector IoTLib_waitlist_funcs;


struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc;
void (*IoTLib_uploadFunction)(char* urlUploadString);
void (*IoTLib_debugFunction)(char* debugString);
void (*IoTLib_storeLastUploadTimeFunc)(time_t lastActiveTime);
time_t (*IoTLib_retrieveLastUploadTimeFunc)();
struct IoTLib_RawSensorDataAndSensorID* (*IoTLib_retrieveAllUnsentDataFunc)();
size_t (*IoTLib_getStoredUnsentDataCountFunc)();

void IoTLib_run()
{
	IoTLib_initialize_managed_array(activeSensorIDs, struct IoTLib_MngdArray_SnsrID, IoTLib_SensorID, IoTLib_SENSOR_COUNT);

	_IoTLib_call_sensor_init_functions();
	_IoTLib_call_sensor_power_on_functions();
	_IoTLib_determine_active_sensors(activeSensorIDs);

	IoTLib_initialize_managed_key_value_array(rawSensorDataBuffer,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_SensorID, void*, activeSensorIDs.length);

	_IoTLib_poll_data_from_sensors(rawSensorDataBuffer, activeSensorIDs);
	_IoTLib_set_last_poll_time_for_active_sensors(activeSensorIDs);

	_IoTLib_upload_all_pending_sensor_data_or_store_new_data_locally(rawSensorDataBuffer, activeSensorIDs);

	_IoTLib_wait_for_tasks_to_complete();

	_IoTLib_store_current_time_as_upload_time();
}

void IoTLib_sensor_registration_init()
{
	if (DEBUG)
	{
		_IoTLib_check_for_unset_functions();
	}

	IoTLib_vector_init(&IoTLib_waitlist_funcs);
}

// sensorName is used for debugging
IoTLib_SensorID IoTLib_register_sensor(char* sensorName)
{
	size_t id = IoTLib_sensorIDsAndNames.length;

	IoTLib_MKV_insert(&IoTLib_sensorIDsAndNames,
			IoTLib_MngdKVArray_SnsrIDString, id, sensorName);

	return id;
}

void IoTLib_register_sensor_init_function(IoTLib_SensorID sensorID, void (*sensorInitFunc)())
{
	IoTLib_MKV_insert(&IoTLib_initFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, sensorInitFunc);
}

void IoTLib_register_sensor_poll_function(IoTLib_SensorID sensorID, void* (*pollSensorFunc)())
{
	IoTLib_MKV_insert(&IoTLib_pollFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, pollSensorFunc);
}

void IoTLib_register_sensor_convert_raw_sensor_data_to_string_function(IoTLib_SensorID sensorID, char* (*dataToStringFunc)(void* rawSensorData))
{
	IoTLib_MKV_insert(&IoTLib_rawDataToStringFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, dataToStringFunc);
}

void IoTLib_register_sensor_power_on_function(IoTLib_SensorID sensorID, void (*powerOnFunc)())
{
	IoTLib_MKV_insert(&IoTLib_powerOnFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, powerOnFunc);
}

void IoTLib_register_sensor_store_unsent_data_function(IoTLib_SensorID sensorID, void (*storeUnsentFunc)(void* rawSensorData))
{
	IoTLib_MKV_insert(&IoTLib_storeDataUnsentFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, storeUnsentFunc);
}

void IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData))
{
	IoTLib_MKV_insert(&IoTLib_generateUploadPayloadFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, generateUploadPayloadFunc);
}

void IoTLib_register_sensor_retrieve_last_polled_time_function(IoTLib_SensorID sensorID, time_t (*retrieveSensorLastPolledTimeFunc)())
{
	IoTLib_MKV_insert(&IoTLib_retrieveSensorLastPolledTimeFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, retrieveSensorLastPolledTimeFunc);
}

void IoTLib_register_sensor_store_last_polled_time_function(IoTLib_SensorID sensorID, void (*storeSensorLastPolledTimeFunc)(time_t lastPollTime))
{
	IoTLib_MKV_insert(&IoTLib_storeSensorLastPolledTimeFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, storeSensorLastPolledTimeFunc);
}

void IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, int sensorPollFrequency)
{
	IoTLib_MKV_insert(&IoTLib_sensorPollFrequencies,
		IoTLib_MngdKVArray_SnsrIDInt, sensorID, sensorPollFrequency);
}

void IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp)
{
	IoTLib_MKV_insert(&IoTLib_sensorMinTemps,
		IoTLib_MngdKVArray_SnsrIDFloat, sensorID, minTemp);
}

void IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp)
{
	IoTLib_MKV_insert(&IoTLib_sensorMaxTemps,
		IoTLib_MngdKVArray_SnsrIDFloat, sensorID, maxTemp);
}


void IoTLib_set_upload_function(void (*uploadFunction)(char* urlUploadString))
{
	IoTLib_uploadFunction = uploadFunction;
}

void IoTLib_set_debug_function(void (*debugFunction)(char* debugString))
{
	IoTLib_debugFunction = debugFunction;
}

void IoTLib_set_temp_sensorid_and_poll_temp_function(IoTLib_SensorID tempSensorID, float (*pollTempSensorFloat)(void* rawSensorData))
{
	struct IoTLib_SnsrIDDataPtr sensorIDWithReadFunc = {.id = tempSensorID, .data = pollTempSensorFloat};
	IoTLib_tempSnsrIDAndRawToFloatFunc = sensorIDWithReadFunc;
}

void IoTLib_set_store_last_upload_time_function(void (*storeLastUploadTimeFunc)(time_t lastUploadTime))
{
	IoTLib_storeLastUploadTimeFunc = storeLastUploadTimeFunc;
}

void IoTLib_set_retrieve_last_upload_time_function(time_t (*retireveLastUploadTimeFunc)())
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


void IoTLib_add_task_to_waitlist(bool (*checkAndHandleTaskCompletionFunc)())
{
	IoTLib_vector_add(&IoTLib_waitlist_funcs, checkAndHandleTaskCompletionFunc);
}


bool _IoTLib_check_for_unset_functions()
{
	return true;
}
