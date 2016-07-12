#ifndef IOTLIB_SENSOR_REGISTRAITION_H
#define IOTLIB_SENSOR_REGISTRAITION_H

int IoTLib_registerSensor(char* sensorName);
void IoTLib_registerSensorInitFunction(int sensorID, void (*sensorInitFunc)());
void IoTLib_registerSensorReadFunction(int sensorID, char* (*readSensorFunc)());
void IoTLib_registerSensorMinOperatingTemp(int sensorID, float minTemp);
void IoTLib_registerSensorPowerOnFunction(int sensorID, void (*powerOnFunc)());

#endif
