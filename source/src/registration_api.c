#include <stdbool.h>

#include "general_macros.h"
#include "registration_api.h"

#include "registration_api_local.h"
#include "user_settings.h"

define_SensorID_with_type(IOTLIB_SensorIDAndName, char*); // NOTE: Currently relies on allocating sensor name to own memory.
define_SensorID_with_type(IOTLIB_SensorIDWithFunctionPointer, void*);
define_SensorID_with_type(IOTLIB_SensorIDWithFloat, float);

define_array_with_counter(IOTLIB_ManagedArray_SensorIDWithName, struct IOTLIB_SensorIDAndName);
define_array_with_counter(IOTLIB_ManagedArray_SensorIDWithFunctionPointer, struct IOTLIB_SensorIDWithFunctionPointer);
define_array_with_counter(IOTLIB_ManagedArray_SensorIDWithFloat, struct IOTLIB_SensorIDWithFloat);

struct IOTLIB_SensorIDAndName IOTLIB_sensorIDsAndNames[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_initFunctions[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_readFunctions[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_sensorDataToStringFunctions[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_powerOnFunctions[IOTLIB_NUM_POWER_ON_FUNCTIONS];
struct IOTLIB_SensorIDWithFloat IOTLIB_sensorMinTemps[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFloat IOTLIB_sensorMaxTemps[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_writeDataOfflineFunctions[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_readDataOfflineFunctions[IOTLIB_SENSOR_COUNT];
struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_generateUploadPayloadFunctions[IOTLIB_SENSOR_COUNT];


struct IOTLIB_ManagedArray_SensorIDWithName IOTLIB_sensorIDsAndNamesManagedArray =
	{.array = IOTLIB_sensorIDsAndNames, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_ManagedArray_SensorIDWithFunctionPointer IOTLIB_initFunctionsManagedArray =
	{.array = IOTLIB_initFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_ManagedArray_SensorIDWithFunctionPointer IOTLIB_readFunctionsManagedArray =
	{.array = IOTLIB_readFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_ManagedArray_SensorIDWithFunctionPointer IOTLIB_sensorDataToStringFunctionsManagedArray =
	{.array = IOTLIB_sensorDataToStringFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_ManagedArray_SensorIDWithFunctionPointer IOTLIB_powerOnFunctionsManagedArray =
	{.array = IOTLIB_powerOnFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_ManagedArray_SensorIDWithFloat IOTLIB_sensorMinTempsManagedArray =
	{.array = IOTLIB_sensorMinTemps, .length = 0, .capacity = IOTLIB_NUM_SENSORS_WITH_MIN_TEMP};
struct IOTLIB_ManagedArray_SensorIDWithFloat IOTLIB_sensorMaxTempsManagedArray =
	{.array = IOTLIB_sensorMaxTemps, .length = 0, .capacity = IOTLIB_NUM_SENSORS_WITH_MAX_TEMP};
struct IOTLIB_ManagedArray_SensorIDWithFunctionPointer IOTLIB_writeDataOfflineFunctionsManagedArray =
	{.array = IOTLIB_writeDataOfflineFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_ManagedArray_SensorIDWithFunctionPointer IOTLIB_readDataOfflineFunctionsManagedArray =
	{.array = IOTLIB_readDataOfflineFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_ManagedArray_SensorIDWithFunctionPointer IOTLIB_genereateuploadPayloadFunctionsManagedArray =
	{.array = IOTLIB_generateUploadPayloadFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};

struct IOTLIB_SensorIDWithFunctionPointer IOTLIB_sensorIDWithTemperatureReadingFunction;

void (*IOTLIB_uploadFunction)(char* urlUploadString);
void (*IOTLIB_debugFunction)(char* debugString, bool isError);
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
	int id = IOTLIB_sensorIDsAndNamesManagedArray.length;
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_sensorIDsAndNamesManagedArray, IOTLIB_SensorIDAndName, id, sensorName);
	return id;
}

void IoTLib_registerSensorInitFunction(IOTLIB_SensorID sensorID, void (*sensorInitFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_initFunctionsManagedArray, IOTLIB_SensorIDWithFunctionPointer, sensorID, sensorInitFunc);
}

void IoTLib_registerSensorReadFunction(IOTLIB_SensorID sensorID, void* (*readSensorFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_readFunctionsManagedArray, IOTLIB_SensorIDWithFunctionPointer, sensorID, readSensorFunc);
}

void IoTLib_registerConvertReadSensorDataToStringFunction(IOTLIB_SensorID sensorID, void (*dataToStringFunc)(void* rawSensorData, char* charBuffer))
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_sensorDataToStringFunctionsManagedArray, IOTLIB_SensorIDWithFunctionPointer, sensorID, dataToStringFunc);
}

void IoTLib_registerSensorPowerOnFunction(IOTLIB_SensorID sensorID, void (*powerOnFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_powerOnFunctionsManagedArray, IOTLIB_SensorIDWithFunctionPointer, sensorID, powerOnFunc);
}

void IoTLib_registerSensorMinOperatingTemp(IOTLIB_SensorID sensorID, float minTemp)
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_sensorMinTempsManagedArray, IOTLIB_SensorIDWithFloat, sensorID, minTemp);
}

void IoTLib_registerSensorMaxOperatingTemp(IOTLIB_SensorID sensorID, float maxTemp)
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_sensorMaxTempsManagedArray, IOTLIB_SensorIDWithFloat, sensorID, maxTemp);
}

void IoTLib_registerSensorWriteDataOfflineFunction(IOTLIB_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData))
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_writeDataOfflineFunctionsManagedArray, IOTLIB_SensorIDWithFunctionPointer, sensorID, writeOfflineFunc);
}

void IoTLib_registerSensorReadDataOfflineFunction(IOTLIB_SensorID sensorID, void* (*readOfflineFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_readDataOfflineFunctionsManagedArray, IOTLIB_SensorIDWithFunctionPointer, sensorID, readOfflineFunc);
}

void IoTLib_registerGenerateUploadPayloadFunction(IOTLIB_SensorID sensorID, void (*generateUploadPayloadFunc)(void* rawSensorData, char* charBuffer))
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_genereateuploadPayloadFunctionsManagedArray, IOTLIB_SensorIDWithFunctionPointer, sensorID, generateUploadPayloadFunc);
}

void IoTLib_setUploadFunction(void (*uploadFunction)(char* urlUploadString))
{
	IOTLIB_uploadFunction = uploadFunction;
}

void IoTLib_setDebugFunction(void (*debugFunction)(char* debugString, bool isError))
{
	IOTLIB_debugFunction = debugFunction;
}

void IoTLib_setTempSensorIDAndReadingFunction(IOTLIB_SensorID tempSensorID, float (*readTempSensorFloat)(void* rawSensorData))
{
	struct IOTLIB_SensorIDWithFunctionPointer sensorIDWithReadFunc = {.id = tempSensorID, .data = readTempSensorFloat};
	IOTLIB_sensorIDWithTemperatureReadingFunction = sensorIDWithReadFunc;
}

bool _IoTLib_checkForUnsetFunctions()
{
	return true;
}
