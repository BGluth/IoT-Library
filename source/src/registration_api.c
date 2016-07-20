#include "registration_api.h"
#include "registration_api_local.h"

#include <stdbool.h>

#include "sensorID.h"
#include "managed_key_value_array_macros.h"
#include "user_settings.h"

IoTLib_define_functions_for_key_type(IoTLib_SensorID, IoTLib_sensorid_get_key_index);

IoTLib_define_SensorID_with_data_struct(IoTLib_SnsrIDDataPtr, void*);
IoTLib_define_managed_array_for_type(IoTLib_MngdArray_SnsrID, IoTLib_SensorID);

IoTLib_define_managed_key_value_array(IoTLib_MngdKVArray_SnsrIDString, IoTLib_SensorID, char*);
IoTLib_define_managed_key_value_array(IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_SensorID, void*);
IoTLib_define_managed_key_value_array(IoTLib_MngdKVArray_SnsrIDFloat, IoTLib_SensorID, float);

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

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMinTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MIN_TEMP);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMaxTemps,
		struct IoTLib_MngdKVArray_SnsrIDFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MAX_TEMP);

struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc;

void (*IoTLib_uploadFunction)(char* urlUploadString);
void (*IoTLib_debugFunction)(char* debugString, bool isError);
//void ()

void IoTLib_run()
{

}

void IoTLib_sensorRegistrationInit()
{
	if (DEBUG)
	{
		_IoTLib_checkForUnsetFunctions();
	}
}

// sensorName is used for debugging
int IoTLib_registerSensor(char* sensorName)
{
	int id = IoTLib_sensorIDsAndNames.length;

	IoTLib_MKV_insert(&IoTLib_sensorIDsAndNames,
			IoTLib_MngdKVArray_SnsrIDString, id, sensorName);

	return id;
}

void IoTLib_registerSensorInitFunction(IoTLib_SensorID sensorID, void (*sensorInitFunc)())
{
	IoTLib_MKV_insert(&IoTLib_initFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, sensorInitFunc);
}

void IoTLib_registerSensorReadFunction(IoTLib_SensorID sensorID, void* (*readSensorFunc)())
{
	IoTLib_MKV_insert(&IoTLib_readFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, readSensorFunc);
}

void IoTLib_registerConvertRawSensorDataToStringFunction(IoTLib_SensorID sensorID, void (*dataToStringFunc)(void* rawSensorData, char* charBuffer))
{
	IoTLib_MKV_insert(&IoTLib_rawDataToStringFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, dataToStringFunc);
}

void IoTLib_registerSensorPowerOnFunction(IoTLib_SensorID sensorID, void (*powerOnFunc)())
{
	IoTLib_MKV_insert(&IoTLib_powerOnFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, powerOnFunc);
}

void IoTLib_registerSensorWriteDataOfflineFunction(IoTLib_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData))
{
	IoTLib_MKV_insert(&IoTLib_writeDataOfflineFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, writeOfflineFunc);
}

void IoTLib_registerSensorReadOfflineDataFunction(IoTLib_SensorID sensorID, void* (*readOfflineFunc)())
{
	IoTLib_MKV_insert(&IoTLib_readOfflineDataFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, readOfflineFunc);
}

void IoTLib_registerGenerateUploadPayloadFunction(IoTLib_SensorID sensorID, void (*generateUploadPayloadFunc)(void* rawSensorData, char* charBuffer))
{
	IoTLib_MKV_insert(&IoTLib_generateUploadPayloadFunctions,
		IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, generateUploadPayloadFunc);
}

void IoTLib_registerSensorMinOperatingTemp(IoTLib_SensorID sensorID, float minTemp)
{
	IoTLib_MKV_insert(&IoTLib_sensorMinTemps,
		IoTLib_MngdKVArray_SnsrIDFloat, sensorID, minTemp);
}

void IoTLib_registerSensorMaxOperatingTemp(IoTLib_SensorID sensorID, float maxTemp)
{
	IoTLib_MKV_insert(&IoTLib_sensorMaxTemps,
		IoTLib_MngdKVArray_SnsrIDFloat, sensorID, maxTemp);
}

void IoTLib_setUploadFunction(void (*uploadFunction)(char* urlUploadString))
{
	IoTLib_uploadFunction = uploadFunction;
}

void IoTLib_setDebugFunction(void (*debugFunction)(char* debugString, bool isError))
{
	IoTLib_debugFunction = debugFunction;
}

void IoTLib_setTempSensorIDAndReadingFunction(IoTLib_SensorID tempSensorID, float (*readTempSensorFloat)(void* rawSensorData))
{
	struct IoTLib_SnsrIDDataPtr sensorIDWithReadFunc = {.id = tempSensorID, .data = readTempSensorFloat};
	IoTLib_tempSnsrIDAndRawToFloatFunc = sensorIDWithReadFunc;
}

bool _IoTLib_checkForUnsetFunctions()
{
	return true;
}
