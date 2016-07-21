#include "registration_api.h"
#include "registration_api_local.h"

#include <time.h>
#include <stdbool.h>

#include "managed_array_definitions.h"
#include "run_helper_functions.h"
#include "user_settings.h"

IoTLib_initialize_managed_key_value_array(IoTLib_sensorIDsAndNames,
		struct IoTLib_MngdKVArray_SnsrIDString,
		IoTLib_SensorID, char*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_initFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_readFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_rawDataToStringFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_powerOnFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_NUM_POWER_ON_FUNCTIONS);

IoTLib_initialize_managed_key_value_array(IoTLib_writeDataOfflineFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_readOfflineDataFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_generateUploadPayloadFunctions,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorReadFrequencies,
		struct IoTLib_MngdKVArray_SnsrIDTime,
		IoTLib_SensorID, time_t, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMinTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MIN_TEMP);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMaxTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MAX_TEMP);

struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc;

void (*IoTLib_uploadFunction)(char* urlUploadString);
void (*IoTLib_debugFunction)(char* debugString, bool isError);
void (*IoTLib_writeDeviceLastActiveTimeFunc)(time_t lastActiveTime);
time_t (*IoTLib_readDeviceLastActiveTimeFunc)();

void IoTLib_run()
{
	IoTLib_initialize_managed_array(activeSensorIDs, struct IoTLib_MngdArray_SnsrID, IoTLib_SensorID, IoTLib_SENSOR_COUNT);

	_IoTLib_call_sensor_init_functions();
	_IoTLib_call_sensor_power_on_functions();
	_IoTLib_determine_active_sensors(&activeSensorIDs);

	IoTLib_initialize_managed_key_value_array(rawSensorDataBuffer,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_SensorID, void*, activeSensorIDs.length);
	IoTLib_initialize_managed_key_value_array(stringSensorDataBuffer,
		struct IoTLib_MngdKVArray_SnsrIDString, IoTLib_SensorID, char*, rawSensorDataBuffer.length);

	_IoTLib_read_and_store_data_from_sensors(rawSensorDataBuffer, activeSensorIDs);
	_IoTLib_get_string_represenations_of_raw_sensor_data(stringSensorDataBuffer, rawSensorDataBuffer);


}

void IoTLib_sensor_registration_init()
{
	if (DEBUG)
	{
		_IoTLib_check_for_unset_functions();
	}
}

// sensorName is used for debugging
int IoTLib_register_sensor(char* sensorName)
{
	int id = IoTLib_sensorIDsAndNames.length;

	IoTLib_MKV_insert(&IoTLib_sensorIDsAndNames,
			IoTLib_MngdKVArray_SnsrIDString, id, sensorName);

	return id;
}

void IoTLib_register_sensor_init_function(IoTLib_SensorID sensorID, void (*sensorInitFunc)())
{
	IoTLib_MKV_insert(&IoTLib_initFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, sensorInitFunc);
}

void IoTLib_register_sensor_read_function(IoTLib_SensorID sensorID, void* (*readSensorFunc)())
{
	IoTLib_MKV_insert(&IoTLib_readFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, readSensorFunc);
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

void IoTLib_register_sensor_write_data_offline_function(IoTLib_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData))
{
	IoTLib_MKV_insert(&IoTLib_writeDataOfflineFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, writeOfflineFunc);
}

void IoTLib_register_sensor_read_offline_data_function(IoTLib_SensorID sensorID, void* (*readOfflineFunc)())
{
	IoTLib_MKV_insert(&IoTLib_readOfflineDataFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, readOfflineFunc);
}

void IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData))
{
	IoTLib_MKV_insert(&IoTLib_generateUploadPayloadFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, generateUploadPayloadFunc);
}

void IoTLib_register_sensor_read_frequency(IoTLib_SensorID sensorID, time_t sensorReadFrequency)
{
	IoTLib_MKV_insert(&IoTLib_sensorReadFrequencies,
		IoTLib_MngdKVArray_SnsrIDTime, sensorID, sensorReadFrequency);
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

void IoTLib_set_debug_function(void (*debugFunction)(char* debugString, bool isError))
{
	IoTLib_debugFunction = debugFunction;
}

void IoTLib_set_temp_sensorid_and_reading_function(IoTLib_SensorID tempSensorID, float (*readTempSensorFloat)(void* rawSensorData))
{
	struct IoTLib_SnsrIDDataPtr sensorIDWithReadFunc = {.id = tempSensorID, .data = readTempSensorFloat};
	IoTLib_tempSnsrIDAndRawToFloatFunc = sensorIDWithReadFunc;
}

void IoTLib_set_write_device_last_active_time_function(void (*writeDeviceLastActiveTimeFunc)(time_t lastActiveTime))
{
	IoTLib_writeDeviceLastActiveTimeFunc = writeDeviceLastActiveTimeFunc;
}

void IoTLib_set_read_device_last_active_time_function(time_t (*readDeviceLastActiveTimeFunc)())
{
	IoTLib_readDeviceLastActiveTimeFunc = readDeviceLastActiveTimeFunc;
}

bool _IoTLib_check_for_unset_functions()
{
	return true;
}
