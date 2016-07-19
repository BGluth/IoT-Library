#ifndef IoTLib_SENSOR_REGISTRAITION_H
#define IoTLib_SENSOR_REGISTRAITION_H

#include <stdbool.h>
#include "typedefs.h"

void IoTLib_run();
int IoTLib_registerSensor(char* sensorName);
void IoTLib_registerSensorInitFunction(IoTLib_SensorID sensorID, void (*sensorInitFunc)());
void IoTLib_registerSensorReadFunction(IoTLib_SensorID sensorID, void* (*readSensorFunc)());
void IoTLib_registerConvertRawSensorDataToStringFunction(IoTLib_SensorID sensorID, void (*dataToStringFunc)(void* rawSensorData, char* charBuffer));
void IoTLib_registerSensorPowerOnFunction(IoTLib_SensorID sensorID, void (*powerOnFunc)());
void IoTLib_registerSensorWriteDataOfflineFunction(IoTLib_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData));
void IoTLib_registerSensorReadOfflineDataFunction(IoTLib_SensorID sensorID, void* (*readOfflineFunc)());
void IoTLib_registerGenerateUploadPayloadFunction(IoTLib_SensorID sensorID, void (*generateUploadPayloadFunc)(void* rawSensorData, char* charBuffer));
void IoTLib_registerSensorMinOperatingTemp(IoTLib_SensorID sensorID, float minTemp);
void IoTLib_registerSensorMaxOperatingTemp(IoTLib_SensorID sensorID, float maxTemp);


void IoTLib_setUploadFunction(void (*uploadFunction)(char* urlUploadString));
void IoTLib_setDebugFunction(void (*debugFunction)(char* debugString, bool isError));
void IoTLib_setTempSensorIDAndReadingFunction(IoTLib_SensorID tempSensorID, float (*readTempSensorFloat)(void* rawSensorData));

#endif
