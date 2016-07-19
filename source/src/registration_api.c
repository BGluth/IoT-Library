#include "registration_api.h"
#include "registration_api_local.h"

#include <stdbool.h>

#include "sensorID.h"
#include "managed_key_value_array_macros.h"
#include "user_settings.h"

IoTLib_define_functions_for_key_type(IoTLib_SensorID, IoTLib_sensorid_get_key_index);

IoTLib_define_SensorID_with_data_struct(IoTLib_SensorIDWithFunctionPointer, void*)
IoTLib_define_managed_array_for_type(IoTLib_ManagedArray_SensorID, IoTLib_SensorID);

IoTLib_define_managed_key_value_array(IoTLib_managedKeyValueArray_SensorIDAndString, IoTLib_SensorID, char*);
IoTLib_define_managed_key_value_array(IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer, IoTLib_SensorID, void*);
IoTLib_define_managed_key_value_array(IoTLib_managedKeyValueArray_SensorIDAndFloat, IoTLib_SensorID, float);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorIDsAndNames,
		struct IoTLib_managedKeyValueArray_SensorIDAndString,
		IoTLib_SensorID, char*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_initFunctions,
		struct IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_readFunctions,
		struct IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_rawDataToStringFunctions,
		struct IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_powerOnFunctions,
		struct IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		IoTLib_SensorID, void*, IoTLib_NUM_POWER_ON_FUNCTIONS);

IoTLib_initialize_managed_key_value_array(IoTLib_writeDataOfflineFunctions,
		struct IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_readOfflineDataFunctions,
		struct IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_generateUploadPayloadFunctions,
		struct IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		IoTLib_SensorID, void*, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMinTemps,
		struct IoTLib_managedKeyValueArray_SensorIDAndFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MIN_TEMP);

IoTLib_initialize_managed_key_value_array(IoTLib_sensorMaxTemps,
		struct IoTLib_managedKeyValueArray_SensorIDAndFloat,
		IoTLib_SensorID, float, IoTLib_NUM_SENSORS_WITH_MAX_TEMP);

struct IoTLib_SensorIDWithFunctionPointer IoTLib_sensorIDWithTemperatureReadingFunction;

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

	IoTLib_managed_key_value_array_insert(IoTLib_sensorIDsAndNames,
			IoTLib_managedKeyValueArray_SensorIDAndString,
			id, sensorName);

	return id;
}

void IoTLib_registerSensorInitFunction(IoTLib_SensorID sensorID, void (*sensorInitFunc)())
{
	IoTLib_managed_key_value_array_insert(IoTLib_initFunctions,
		IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		sensorID, sensorInitFunc);
}

void IoTLib_registerSensorReadFunction(IoTLib_SensorID sensorID, void* (*readSensorFunc)())
{
	IoTLib_managed_key_value_array_insert(IoTLib_readFunctions,
		IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		sensorID, readSensorFunc);
}

void IoTLib_registerConvertRawSensorDataToStringFunction(IoTLib_SensorID sensorID, void (*dataToStringFunc)(void* rawSensorData, char* charBuffer))
{
	IoTLib_managed_key_value_array_insert(IoTLib_rawDataToStringFunctions,
		IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		sensorID, dataToStringFunc);
}

void IoTLib_registerSensorPowerOnFunction(IoTLib_SensorID sensorID, void (*powerOnFunc)())
{
	IoTLib_managed_key_value_array_insert(IoTLib_powerOnFunctions,
		IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		sensorID, powerOnFunc);
}

void IoTLib_registerSensorWriteDataOfflineFunction(IoTLib_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData))
{
	IoTLib_managed_key_value_array_insert(IoTLib_writeDataOfflineFunctions,
		IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		sensorID, writeOfflineFunc);
}

void IoTLib_registerSensorReadOfflineDataFunction(IoTLib_SensorID sensorID, void* (*readOfflineFunc)())
{
	IoTLib_managed_key_value_array_insert(IoTLib_readOfflineDataFunctions,
		IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		sensorID, readOfflineFunc);
}

void IoTLib_registerGenerateUploadPayloadFunction(IoTLib_SensorID sensorID, void (*generateUploadPayloadFunc)(void* rawSensorData, char* charBuffer))
{
	IoTLib_managed_key_value_array_insert(IoTLib_generateUploadPayloadFunctions,
		IoTLib_managedKeyValueArray_SensorIDAndFunctionPointer,
		sensorID, generateUploadPayloadFunc);
}

void IoTLib_registerSensorMinOperatingTemp(IoTLib_SensorID sensorID, float minTemp)
{
	IoTLib_managed_key_value_array_insert(IoTLib_sensorMinTemps,
		IoTLib_managedKeyValueArray_SensorIDAndFloat,
		sensorID, minTemp);
}

void IoTLib_registerSensorMaxOperatingTemp(IoTLib_SensorID sensorID, float maxTemp)
{
	IoTLib_managed_key_value_array_insert(IoTLib_sensorMaxTemps,
		IoTLib_managedKeyValueArray_SensorIDAndFloat,
		sensorID, maxTemp);
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
	struct IoTLib_SensorIDWithFunctionPointer sensorIDWithReadFunc = {.id = tempSensorID, .data = readTempSensorFloat};
	IoTLib_sensorIDWithTemperatureReadingFunction = sensorIDWithReadFunc;
}

bool _IoTLib_checkForUnsetFunctions()
{
	return true;
}
