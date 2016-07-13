#include <stdbool.h>

#include "general_macros.h"
#include "registration_api.h"

#include "registration_api_local.h"
#include "user_settings.h"

define_SensorID_with_type(IoTLib_SensorIDAndName, char*); // NOTE: Currently relies on allocating sensor name to own memory.
define_SensorID_with_type(IoTLib_SensorIDWithFunctionPointer, void*);
define_SensorID_with_type(IoTLib_SensorIDWithFloat, float);

define_array_with_counter(IoTLib_ManagedArray_SensorIDWithName, struct IoTLib_SensorIDAndName);
define_array_with_counter(IoTLib_ManagedArray_SensorIDWithFunctionPointer, struct IoTLib_SensorIDWithFunctionPointer);
define_array_with_counter(IoTLib_ManagedArray_SensorIDWithFloat, struct IoTLib_SensorIDWithFloat);

struct IoTLib_SensorIDAndName IoTLib_sensorIDsAndNames[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFunctionPointer IoTLib_initFunctions[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFunctionPointer IoTLib_readFunctions[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFunctionPointer IoTLib_sensorDataToStringFunctions[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFunctionPointer IoTLib_powerOnFunctions[IoTLib_NUM_POWER_ON_FUNCTIONS];
struct IoTLib_SensorIDWithFloat IoTLib_sensorMinTemps[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFloat IoTLib_sensorMaxTemps[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFunctionPointer IoTLib_writeDataOfflineFunctions[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFunctionPointer IoTLib_readDataOfflineFunctions[IoTLib_SENSOR_COUNT];
struct IoTLib_SensorIDWithFunctionPointer IoTLib_generateUploadPayloadFunctions[IoTLib_SENSOR_COUNT];


struct IoTLib_ManagedArray_SensorIDWithName IoTLib_sensorIDsAndNamesManagedArray =
	{.array = IoTLib_sensorIDsAndNames, .length = 0, .capacity = IoTLib_SENSOR_COUNT};
struct IoTLib_ManagedArray_SensorIDWithFunctionPointer IoTLib_initFunctionsManagedArray =
	{.array = IoTLib_initFunctions, .length = 0, .capacity = IoTLib_SENSOR_COUNT};
struct IoTLib_ManagedArray_SensorIDWithFunctionPointer IoTLib_readFunctionsManagedArray =
	{.array = IoTLib_readFunctions, .length = 0, .capacity = IoTLib_SENSOR_COUNT};
struct IoTLib_ManagedArray_SensorIDWithFunctionPointer IoTLib_sensorDataToStringFunctionsManagedArray =
	{.array = IoTLib_sensorDataToStringFunctions, .length = 0, .capacity = IoTLib_SENSOR_COUNT};
struct IoTLib_ManagedArray_SensorIDWithFunctionPointer IoTLib_powerOnFunctionsManagedArray =
	{.array = IoTLib_powerOnFunctions, .length = 0, .capacity = IoTLib_SENSOR_COUNT};
struct IoTLib_ManagedArray_SensorIDWithFloat IoTLib_sensorMinTempsManagedArray =
	{.array = IoTLib_sensorMinTemps, .length = 0, .capacity = IoTLib_NUM_SENSORS_WITH_MIN_TEMP};
struct IoTLib_ManagedArray_SensorIDWithFloat IoTLib_sensorMaxTempsManagedArray =
	{.array = IoTLib_sensorMaxTemps, .length = 0, .capacity = IoTLib_NUM_SENSORS_WITH_MAX_TEMP};
struct IoTLib_ManagedArray_SensorIDWithFunctionPointer IoTLib_writeDataOfflineFunctionsManagedArray =
	{.array = IoTLib_writeDataOfflineFunctions, .length = 0, .capacity = IoTLib_SENSOR_COUNT};
struct IoTLib_ManagedArray_SensorIDWithFunctionPointer IoTLib_readDataOfflineFunctionsManagedArray =
	{.array = IoTLib_readDataOfflineFunctions, .length = 0, .capacity = IoTLib_SENSOR_COUNT};
struct IoTLib_ManagedArray_SensorIDWithFunctionPointer IoTLib_genereateuploadPayloadFunctionsManagedArray =
	{.array = IoTLib_generateUploadPayloadFunctions, .length = 0, .capacity = IoTLib_SENSOR_COUNT};

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
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorIDsAndNamesManagedArray, IoTLib_SensorIDAndName, id, sensorName);
	return id;
}

void IoTLib_registerSensorInitFunction(IoTLib_SensorID sensorID, void (*sensorInitFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_initFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, sensorInitFunc);
}

void IoTLib_registerSensorReadFunction(IoTLib_SensorID sensorID, void* (*readSensorFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_readFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, readSensorFunc);
}

void IoTLib_registerConvertReadSensorDataToStringFunction(IoTLib_SensorID sensorID, void (*dataToStringFunc)(void* rawSensorData, char* charBuffer))
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorDataToStringFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, dataToStringFunc);
}

void IoTLib_registerSensorPowerOnFunction(IoTLib_SensorID sensorID, void (*powerOnFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_powerOnFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, powerOnFunc);
}

void IoTLib_registerSensorMinOperatingTemp(IoTLib_SensorID sensorID, float minTemp)
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorMinTempsManagedArray, IoTLib_SensorIDWithFloat, sensorID, minTemp);
}

void IoTLib_registerSensorMaxOperatingTemp(IoTLib_SensorID sensorID, float maxTemp)
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_sensorMaxTempsManagedArray, IoTLib_SensorIDWithFloat, sensorID, maxTemp);
}

void IoTLib_registerSensorWriteDataOfflineFunction(IoTLib_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData))
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_writeDataOfflineFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, writeOfflineFunc);
}

void IoTLib_registerSensorReadDataOfflineFunction(IoTLib_SensorID sensorID, void* (*readOfflineFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_readDataOfflineFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, readOfflineFunc);
}

void IoTLib_registerGenerateUploadPayloadFunction(IoTLib_SensorID sensorID, void (*generateUploadPayloadFunc)(void* rawSensorData, char* charBuffer))
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IoTLib_genereateuploadPayloadFunctionsManagedArray, IoTLib_SensorIDWithFunctionPointer, sensorID, generateUploadPayloadFunc);
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
