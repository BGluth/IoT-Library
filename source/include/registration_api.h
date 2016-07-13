#ifndef IOTLIB_SENSOR_REGISTRAITION_H
#define IOTLIB_SENSOR_REGISTRAITION_H

typedef int IOTLIB_SensorID;

void IoTLib_run();
int IoTLib_registerSensor(char* sensorName);
void IoTLib_registerSensorInitFunction(IOTLIB_SensorID sensorID, void (*sensorInitFunc)());
void IoTLib_registerSensorReadFunction(IOTLIB_SensorID sensorID, void* (*readSensorFunc)());
void IoTLib_registerConvertReadSensorDataToStringFunction(IOTLIB_SensorID sensorID, void (*dataToStringFunc)(void* rawSensorData, char* charBuffer));
void IoTLib_registerSensorPowerOnFunction(IOTLIB_SensorID sensorID, void (*powerOnFunc)());
void IoTLib_registerSensorMinOperatingTemp(IOTLIB_SensorID sensorID, float minTemp);
void IoTLib_registerSensorMaxOperatingTemp(IOTLIB_SensorID sensorID, float maxTemp);
void IoTLib_registerSensorWriteDataOfflineFunction(IOTLIB_SensorID sensorID, void (*writeOfflineFunc)(void* rawSensorData));
void IoTLib_registerSensorReadDataOfflineFunction(IOTLIB_SensorID sensorID, void* (*readOfflineFunc)());
void IoTLib_registerGenerateUploadPayloadFunction(IOTLIB_SensorID sensorID, void (*generateUploadPayloadFunc)(void* rawSensorData, char* charBuffer));

void IoTLib_setUploadFunction(void (*uploadFunction)(char* urlUploadString));
void IoTLib_setDebugFunction(void (*debugFunction)(char* debugString, bool isError));
void IoTLib_setTempSensorIDAndReadingFunction(IOTLIB_SensorID tempSensorID, float (*readTempSensorFloat)(void* rawSensorData));

#endif
