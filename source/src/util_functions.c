#include "util_functions.h"

#include <stddef.h>
#include <float.h>

#include "managed_array_definitions.h"

extern IoTLib_time_t (*IoTLib_getCurrentTimeFunction)();
extern uint32_t (*IoTLib_convertTimeTypeToSecondsFunc)(IoTLib_time_t rawTime);
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_retrieveSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDTime_t IoTLib_sensorPollFrequencies;

double IoTLib_calculate_time_in_seconds_until_next_sensor_polling()
{
	IoTLib_time_t currentTime = IoTLib_getCurrentTimeFunction();
	double nearestTimeUntilNextSensorPoll = DBL_MAX;

	for (size_t i = 0; i < IoTLib_retrieveSensorLastPolledTimeFunctions.length; i++)
	{
		IoTLib_SensorID currentSensorID = IoTLib_retrieveSensorLastPolledTimeFunctions.keys[i];
		IoTLib_time_t (*retrieveCurrentSensorLastPolledTimeFunc)() = (IoTLib_time_t (*)())
			IoTLib_retrieveSensorLastPolledTimeFunctions.values[i];

		IoTLib_time_t sensorLastPollTime = retrieveCurrentSensorLastPolledTimeFunc();
		IoTLib_time_t currentSensorFrequency = IoTLib_MKV_get(&IoTLib_sensorPollFrequencies,
			IoTLib_MngdKVArray_SnsrIDTime_t, currentSensorID);
		IoTLib_time_t timeOfCurrentSensorsNextPoll = sensorLastPollTime + currentSensorFrequency;
		double timeUntilCurrentSensorsNextPoll = IoTLib_calculate_time_difference(
			timeOfCurrentSensorsNextPoll, currentTime);

		if (timeUntilCurrentSensorsNextPoll < nearestTimeUntilNextSensorPoll)
			nearestTimeUntilNextSensorPoll = timeUntilCurrentSensorsNextPoll;
	}

	return IoTLib_convertTimeTypeToSecondsFunc(nearestTimeUntilNextSensorPoll);
}

double IoTLib_calculate_time_difference(IoTLib_time_t endTime, IoTLib_time_t startTime)
{
	double endTimeInSeconds = IoTLib_convertTimeTypeToSecondsFunc(endTime);
	double startTimeInSeconds = IoTLib_convertTimeTypeToSecondsFunc(startTime);

	return endTime - startTime;
}
