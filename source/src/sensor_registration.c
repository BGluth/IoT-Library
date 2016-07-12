#include "sensor_registration.h"
#include "general_macros.h"
#include "sensor_registration_local.h"

#include "user_settings.h"

define_SensorID_with_type(IOTLIB_SensorIDAndName, char*); // NOTE: Currently relies on allocating sensor name to own memory.
define_SensorID_with_type(IOTLIB_sensorIDWithFunctionPointer, void*);
define_SensorID_with_type(IOTLIB_sensorIDWithFloat, float);

define_array_with_counter(IOTLIB_array_sensorIDWithName, struct IOTLIB_SensorIDAndName);
define_array_with_counter(IOTLIB_array_sensorIDWithFunctionPointer, struct IOTLIB_sensorIDWithFunctionPointer);
define_array_with_counter(IOTLIB_array_sensorIDWithFloat, struct IOTLIB_sensorIDWithFloat);

struct IOTLIB_SensorIDAndName IOTLIB_sensorIDsAndNames[IOTLIB_SENSOR_COUNT];
struct IOTLIB_sensorIDWithFunctionPointer IOTLIB_initFunctions[IOTLIB_SENSOR_COUNT];
struct IOTLIB_sensorIDWithFunctionPointer IOTLIB_readFunctions[IOTLIB_SENSOR_COUNT];
struct IOTLIB_sensorIDWithFunctionPointer IOTLIB_powerOnFunctions[IOTLIB_NUM_POWER_ON_FUNCTIONS];
struct IOTLIB_sensorIDWithFloat IOTLIB_sensorMinTemps[IOTLIB_NUM_SENSORS_WITH_MIN_TEMP];

struct IOTLIB_array_sensorIDWithName IOTLIB_sensorIDsAndNamesArray =
	{.array = IOTLIB_sensorIDsAndNames, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_array_sensorIDWithFunctionPointer IOTLIB_initFunctionsArray =
	{.array = IOTLIB_initFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_array_sensorIDWithFunctionPointer IOTLIB_readFunctionsArray =
	{.array = IOTLIB_readFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_array_sensorIDWithFunctionPointer IOTLIB_powerOnFunctionsArray =
	{.array = IOTLIB_powerOnFunctions, .length = 0, .capacity = IOTLIB_SENSOR_COUNT};
struct IOTLIB_array_sensorIDWithFloat IOTLIB_sensorMinTempsArray =
	{.array = IOTLIB_sensorMinTemps, .length = 0, .capacity = IOTLIB_NUM_SENSORS_WITH_MIN_TEMP};


void IoTLib_sensorRegistrationInit()
{

}

// sensorName is used for debugging
int IoTLib_registerSensor(char* sensorName)
{
	int id = IOTLIB_sensorIDsAndNamesArray.length;
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_sensorIDsAndNamesArray, IOTLIB_SensorIDAndName, id, sensorName);
	return id;
}

void IoTLib_registerSensorInitFunction(SensorID sensorID, void (*sensorInitFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_initFunctionsArray, IOTLIB_sensorIDWithFunctionPointer, sensorID, sensorInitFunc);
}

void IoTLib_registerSensorReadFunction(SensorID sensorID, char* (*readSensorFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_readFunctionsArray, IOTLIB_sensorIDWithFunctionPointer, sensorID, readSensorFunc);
}

void IoTLib_registerSensorPowerOnFunction(SensorID sensorID, void (*powerOnFunc)())
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_powerOnFunctionsArray, IOTLIB_sensorIDWithFunctionPointer, sensorID, powerOnFunc);
}

void IoTLib_registerSensorMinOperatingTemp(SensorID sensorID, float minTemp)
{
	initialize_sensorIDAndData_struct_and_add_to_array
		(IOTLIB_sensorMinTempsArray, IOTLIB_sensorIDWithFloat, sensorID, minTemp);
}
