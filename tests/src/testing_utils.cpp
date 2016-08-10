#include "testing_utils.hpp"

#include <stddef.h>

#include "user_settings.h"
#include "registration_api.h"
#include "managed_key_value_array_macros.h"
#include "managed_array_definitions.h"
#include "vector.h"

extern struct IoTLib_MngdKVArray_SnsrIDString IoTLib_sensorIDsAndNames;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_initFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_pollFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_rawDataToStringFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_powerOnFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_generateUploadPayloadFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_storeUnsentDataFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_retrieveSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_storeSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDTime_t IoTLib_sensorPollFrequencies;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMinTemps;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMaxTemps;
extern struct IoTLib_Vector IoTLib_waitlist_funcs;

extern struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc;
extern void (*IoTLib_uploadFunction)(char* urlUploadString);
extern void (*IoTLib_debugFunction)(char* debugString);
extern void (*IoTLib_storeLastUploadTimeFunc)(IoTLib_time_t lastActiveTime);
extern IoTLib_time_t (*IoTLib_retrieveLastUploadTimeFunc)();
extern struct IoTLib_RawSensorDataAndSensorID* (*IoTLib_retrieveAllUnsentDataFunc)();
extern size_t (*IoTLib_getStoredUnsentDataCountFunc)();

void clear_registered_data()
{
	IoTLib_MKV_clear(&IoTLib_sensorIDsAndNames, IoTLib_MngdKVArray_SnsrIDString);
	IoTLib_MKV_clear(&IoTLib_initFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_powerOnFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_generateUploadPayloadFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_pollFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_rawDataToStringFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_retrieveSensorLastPolledTimeFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_storeSensorLastPolledTimeFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_storeUnsentDataFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr);
	IoTLib_MKV_clear(&IoTLib_sensorMinTemps, IoTLib_MngdKVArray_SnsrIDFloat);
	IoTLib_MKV_clear(&IoTLib_sensorMaxTemps, IoTLib_MngdKVArray_SnsrIDFloat);
	IoTLib_MKV_clear(&IoTLib_sensorPollFrequencies, IoTLib_MngdKVArray_SnsrIDTime_t);
	IoTLib_vector_free(&IoTLib_waitlist_funcs);

	IoTLib_tempSnsrIDAndRawToFloatFunc = (struct IoTLib_SnsrIDDataPtr) {0, NULL};
	IoTLib_uploadFunction = NULL;
	IoTLib_debugFunction = NULL;
	IoTLib_storeLastUploadTimeFunc = NULL;
	IoTLib_retrieveLastUploadTimeFunc = NULL;
	IoTLib_retrieveAllUnsentDataFunc = NULL;
	IoTLib_getStoredUnsentDataCountFunc = NULL;
}

void init_and_run()
{
	IoTLib_sensor_registration_init();
	IoTLib_run();
}
