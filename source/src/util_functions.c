#include "util_functions.h"

#include <stddef.h>
#include <float.h>

#include "managed_array_definitions.h"

extern IoTLib_TIME_T (*IoTLib_getCurrentTimeFunction)();
extern uint32_t (*IoTLib_convertTimeTypeToSecondsFunc)(IoTLib_TIME_T rawTime);
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_retrieveSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDTime_t IoTLib_sensorPollFrequencies;

double IoTLib_calculate_time_in_seconds_until_next_sensor_polling()
{
	IoTLib_TIME_T currentTime = IoTLib_getCurrentTimeFunction();
	double nearestTimeUntilNextSensorPoll = DBL_MAX;

	for (size_t i = 0; i < IoTLib_retrieveSensorLastPolledTimeFunctions.length; i++)
	{
		IoTLib_SensorID currentSensorID = IoTLib_retrieveSensorLastPolledTimeFunctions.keys[i];
		IoTLib_TIME_T (*retrieveCurrentSensorLastPolledTimeFunc)() = (IoTLib_TIME_T (*)())
			IoTLib_retrieveSensorLastPolledTimeFunctions.values[i];

		IoTLib_TIME_T sensorLastPollTime = retrieveCurrentSensorLastPolledTimeFunc();
		IoTLib_TIME_T currentSensorFrequency = IoTLib_MKV_get(&IoTLib_sensorPollFrequencies,
			IoTLib_MngdKVArray_SnsrIDTime_t, currentSensorID);
		IoTLib_TIME_T timeOfCurrentSensorsNextPoll = sensorLastPollTime + currentSensorFrequency;
		double timeUntilCurrentSensorsNextPoll = IoTLib_calculate_time_difference_in_seconds(
			timeOfCurrentSensorsNextPoll, currentTime);

		if (timeUntilCurrentSensorsNextPoll < nearestTimeUntilNextSensorPoll)
			nearestTimeUntilNextSensorPoll = timeUntilCurrentSensorsNextPoll;
	}

	return IoTLib_convertTimeTypeToSecondsFunc(nearestTimeUntilNextSensorPoll);
}

double IoTLib_calculate_time_difference_in_seconds(IoTLib_TIME_T endTime, IoTLib_TIME_T startTime)
{
	double endTimeInSeconds = IoTLib_convertTimeTypeToSecondsFunc(endTime);
	double startTimeInSeconds = IoTLib_convertTimeTypeToSecondsFunc(startTime);

	return endTimeInSeconds - startTimeInSeconds;
}
