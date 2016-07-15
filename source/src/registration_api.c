#include <stdbool.h>

#include "general_macros.h"
#include "registration_api.h"

#include "registration_api_local.h"
#include "user_settings.h"

IoTLib_define_SensorID_with_type(IoTLib_SensorIDAndString, char*); // NOTE: Currently relies on allocating sensor name to own memory.
IoTLib_define_SensorID_with_type(IoTLib_SensorIDWithFunctionPointer, void*);
IoTLib_define_SensorID_with_type(IoTLib_SensorIDWithFloat, float);

IoTLib_define_array_with_counter(IoTLib_ManagedArray_SensorIDWithString, struct IoTLib_SensorIDAndString);
IoTLib_define_array_with_counter(IoTLib_ManagedArray_SensorIDWithFunctionPointer, struct IoTLib_SensorIDWithFunctionPointer);
IoTLib_define_array_with_counter(IoTLib_ManagedArray_SensorIDWithFloat, struct IoTLib_SensorIDWithFloat);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithString,
	IoTLib_sensorIDsAndNamesManagedArray, struct IoTLib_SensorIDAndString, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFunctionPointer,
	IoTLib_initFunctionsManagedArray, struct IoTLib_SensorIDWithFunctionPointer, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFunctionPointer,
	IoTLib_readFunctionsManagedArray, struct IoTLib_SensorIDWithFunctionPointer, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFunctionPointer,
	IoTLib_sensorDataToStringFunctionsManagedArray, struct IoTLib_SensorIDWithFunctionPointer, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFunctionPointer,
	IoTLib_powerOnFunctionsManagedArray, struct IoTLib_SensorIDWithFunctionPointer, IoTLib_NUM_POWER_ON_FUNCTIONS);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFunctionPointer,
	IoTLib_writeDataOfflineFunctionsManagedArray, struct IoTLib_SensorIDWithFunctionPointer, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFunctionPointer,
	IoTLib_readOfflineDataFunctionsManagedArray, struct IoTLib_SensorIDWithFunctionPointer, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFunctionPointer,
	IoTLib_generateUploadPayloadFunctionsManagedArray, struct IoTLib_SensorIDWithFunctionPointer, IoTLib_SENSOR_COUNT);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFloat,
	IoTLib_sensorMinTempsManagedArray, struct IoTLib_SensorIDWithFloat, IoTLib_NUM_SENSORS_WITH_MIN_TEMP);

IoTLib_initialize_managed_array(
	struct IoTLib_ManagedArray_SensorIDWithFloat,
	IoTLib_sensorMaxTempsManagedArray, struct IoTLib_SensorIDWithFloat, IoTLib_NUM_SENSORS_WITH_MAX_TEMP);


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
	int id = IoTLib_sensorIDsAndNamesManagedArray.length;
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorIDsAndNamesManagedArray, IoTLib_SensorIDAndString, id, sensorName);
	return id;
}

void IoTLib_registerSensorInitFunction(IoTLib_SensorID sensorID, void (*sensorInitFunc)())
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_initFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, sensorInitFunc);
}

void IoTLib_registerSensorReadFunction(IoTLib_SensorID sensorID, void* (*readSensorFunc)())
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_readFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, readSensorFunc);
}

void IoTLib_registerConvertReadSensorDataToStringFunction(IoTLib_SensorID sensorID, void (*dataToStringFunc)(void* rawSensorData, char* charBuffer))
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorDataToStringFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, dataToStringFunc);
}

void IoTLib_registerSensorPowerOnFunction(IoTLib_SensorID sensorID, void (*powerOnFunc)())
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_powerOnFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, powerOnFunc);
}

void IoTLib_registerSensorMinOperatingTemp(IoTLib_SensorID sensorID, float minTemp)
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorMinTempsManagedArray, IoTLib_SensorIDWithFloat, sensorID, minTemp);
}

void IoTLib_registerSensorMaxOperatingTemp(IoTLib_SensorID sensorID, float maxTemp)
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorMaxTempsManagedArray, IoTLib_SensorIDWithFloat, sensorID, maxTemp);
}

void IoTLib_registerSensorWriteDataOfflineFunction(IoTLib_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData))
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_writeDataOfflineFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, writeOfflineFunc);
}

void IoTLib_registerSensorReadOfflineDataFunction(IoTLib_SensorID sensorID, void* (*readOfflineFunc)())
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_readOfflineDataFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, readOfflineFunc);
}

void IoTLib_registerGenerateUploadPayloadFunction(IoTLib_SensorID sensorID, void (*generateUploadPayloadFunc)(void* rawSensorData, char* charBuffer))
{
	IoTLib_initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_generateUploadPayloadFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, generateUploadPayloadFunc);
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
