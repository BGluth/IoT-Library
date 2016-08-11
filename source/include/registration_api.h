#ifndef IoTLib_SENSOR_REGISTRAITION_H
#define IoTLib_SENSOR_REGISTRAITION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "user_settings.h"
#include "typedefs.h"
#include "public_structs.h"

void IoTLib_run();
void IoTLib_sensor_registration_init();

IoTLib_SensorID IoTLib_register_sensor(const char* sensorName);
void IoTLib_register_sensor_init_function(IoTLib_SensorID sensorID, void (*sensorInitFunc)());
void IoTLib_register_sensor_poll_function(IoTLib_SensorID sensorID, void* (*readSensorFunc)());
void IoTLib_register_sensor_convert_raw_sensor_data_to_string_function(IoTLib_SensorID sensorID, char* (*dataToStringFunc)(void* rawSensorData));
void IoTLib_register_sensor_power_on_function(IoTLib_SensorID sensorID, void (*powerOnFunc)());
void IoTLib_register_sensor_store_unsent_data_function(IoTLib_SensorID sensorID, void (*storeUnsentFunc)(void* rawSensorData));
void IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData));
void IoTLib_register_sensor_retrieve_last_polled_time_function(IoTLib_SensorID sensorID, IoTLib_time_t (*retrieveSensorLastPolledTimeFunc)());
void IoTLib_register_sensor_store_last_polled_time_function(IoTLib_SensorID sensorID, void (*storeSensorLastPolledTimeFunc)(IoTLib_time_t lastPollTime));
void IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, IoTLib_time_t sensorReadFrequency);
void IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp);
void IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp);

void IoTLib_register_upload_function(void (*uploadFunction)(char* urlUploadString));
void IoTLib_register_debug_function(void (*debugFunction)(char* debugString));
void IoTLib_register_temp_sensorid_and_raw_to_temp_function(IoTLib_SensorID tempSensorID, float (*rawToTempFunc)(void* rawSensorData));
void IoTLib_register_store_last_upload_time_function(void (*storeLastUploadTimeFunc)(IoTLib_time_t lastUploadTime));
void IoTLib_register_retrieve_last_upload_time_function(IoTLib_time_t (*retireveLastUploadTimeFunc)());
void IoTLib_register_retrieve_all_stored_unsent_sensor_data_function(struct IoTLib_RawSensorDataAndSensorID* (*retrieveAllUnsentDataFunc)());
void IoTLib_register_get_stored_unsent_data_count_function(size_t (*getStoredUnsentDataCountFunc)());
void IoTLib_register_get_current_time_function(IoTLib_time_t (*getCurrentTimeFunc)());
void IoTLib_register_convert_time_type_to_seconds_func(double (*convertTimeTypeToSecondsFunc)(IoTLib_time_t rawTime));

#ifdef __cplusplus
}
#endif

#endif
