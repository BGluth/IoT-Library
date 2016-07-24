#ifndef IoTLib_SENSOR_REGISTRAITION_H
#define IoTLib_SENSOR_REGISTRAITION_H

#include <stddef.h>
#include <stdbool.h>
#include <time.h>

#include "typedefs.h"
#include "public_structs.h"

void IoTLib_run();

IoTLib_SensorID IoTLib_register_sensor(char* sensorName);
void IoTLib_register_sensor_init_function(IoTLib_SensorID sensorID, void (*sensorInitFunc)());
void IoTLib_register_sensor_poll_function(IoTLib_SensorID sensorID, void* (*readSensorFunc)());
void IoTLib_register_sensor_convert_raw_sensor_data_to_string_function(IoTLib_SensorID sensorID, char* (*dataToStringFunc)(void* rawSensorData));
void IoTLib_register_sensor_power_on_function(IoTLib_SensorID sensorID, void (*powerOnFunc)());
void IoTLib_register_sensor_store_unsent_data_function(IoTLib_SensorID sensorID, void (*storeUnsentFunc)(void* rawSensorData));
void IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData));
void IoTLib_register_sensor_retrieve_last_polled_time_function(IoTLib_SensorID sensorID, time_t (*retrieveSensorLastPolledTimeFunc)());
void IoTLib_register_sensor_store_last_polled_time_function(IoTLib_SensorID sensorID, void (*storeSensorLastPolledTimeFunc)(time_t lastPollTime));
void IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, int sensorReadFrequency);
void IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp);
void IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp);

void IoTLib_set_upload_function(void (*uploadFunction)(char* urlUploadString));
void IoTLib_set_debug_function(void (*debugFunction)(char* debugString, bool isError));
void IoTLib_set_temp_sensorid_and_poll_temp_function(IoTLib_SensorID tempSensorID, float (*pollTempSensorFloat)(void* rawSensorData));
void IoTLib_set_store_last_upload_time_function(void (*storeLastUploadTimeFunc)(time_t lastUploadTime));
void IoTLib_set_retrieve_last_upload_time_function(time_t (*retireveLastUploadTimeFunc)());
void IoTLib_register_retrieve_all_stored_unsent_sensor_data_function(struct IoTLib_RawSensorDataAndSensorID* (*retrieveAllUnsentDataFunc)());
void IoTLib_register_get_stored_unsent_data_count_function(size_t (*getStoredUnsentDataCountFunc)());

#endif
