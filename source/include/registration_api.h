#ifndef IoTLib_SENSOR_REGISTRAITION_H
#define IoTLib_SENSOR_REGISTRAITION_H

#include <stdbool.h>
#include <time.h>
#include "typedefs.h"

void IoTLib_run();
int IoTLib_register_sensor(char* sensorName);
void IoTLib_register_sensor_init_function(IoTLib_SensorID sensorID, void (*sensorInitFunc)());
void IoTLib_register_sensor_read_function(IoTLib_SensorID sensorID, void* (*readSensorFunc)());
void IoTLib_register_sensor_convert_raw_sensor_data_to_string_function(IoTLib_SensorID sensorID, char* (*dataToStringFunc)(void* rawSensorData));
void IoTLib_register_sensor_power_on_function(IoTLib_SensorID sensorID, void (*powerOnFunc)());
void IoTLib_register_sensor_write_data_offline_function(IoTLib_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData));
void IoTLib_register_sensor_read_offline_data_function(IoTLib_SensorID sensorID, void* (*readOfflineFunc)());
void IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData));
void IoTLib_register_sensor_read_frequency(IoTLib_SensorID sensorID, time_t sensorReadFrequency);
void IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp);
void IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp);


void IoTLib_set_upload_function(void (*uploadFunction)(char* urlUploadString));
void IoTLib_set_debug_function(void (*debugFunction)(char* debugString, bool isError));
void IoTLib_set_temp_sensorid_and_reading_function(IoTLib_SensorID tempSensorID, float (*readTempSensorFloat)(void* rawSensorData));

#endif
