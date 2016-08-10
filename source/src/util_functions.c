#include "util_functions.h"

#include <stdint.h>
#include <limits.h>
#include <stddef.h>

#include "managed_array_definitions.h"

extern IoTLib_time_t (*IoTLib_getCurrentTimeFunction)();
extern uint32_t (*IoTLib_convertTimeTypeToSecondsFunc)(IoTLib_time_t rawTime);
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_retrieveSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDTime_t IoTLib_sensorPollFrequencies;

uint32_t IoTLib_calculate_time_in_seconds_until_next_sensor_polling()
{
	IoTLib_time_t currentTime = IoTLib_getCurrentTimeFunction();
	uint32_t nearestTimeUntilNextSensorPoll = INT_MAX;

	for (size_t i = 0; i < IoTLib_retrieveSensorLastPolledTimeFunctions.length; i++)
	{
		IoTLib_SensorID currentSensorID = IoTLib_retrieveSensorLastPolledTimeFunctions.keys[i];
		IoTLib_time_t (*retrieveCurrentSensorLastPolledTimeFunc)() = (IoTLib_time_t (*)())
			IoTLib_retrieveSensorLastPolledTimeFunctions.values[i];

		IoTLib_time_t sensorLastPollTime = retrieveCurrentSensorLastPolledTimeFunc();
		IoTLib_time_t currentSensorFrequency = IoTLib_MKV_get(&IoTLib_sensorPollFrequencies,
			IoTLib_MngdKVArray_SnsrIDTime_t, currentSensorID);
		IoTLib_time_t timeOfCurrentSensorsNextPoll = sensorLastPollTime + currentSensorFrequency;
		IoTLib_time_t timeUntilCurrentSensorsNextPoll = IoTLib_calculate_time_difference(
			timeOfCurrentSensorsNextPoll, currentTime);

		if (timeUntilCurrentSensorsNextPoll < nearestTimeUntilNextSensorPoll)
			nearestTimeUntilNextSensorPoll = timeUntilCurrentSensorsNextPoll;
	}

	if (nearestTimeUntilNextSensorPoll < 0)
		nearestTimeUntilNextSensorPoll = 0;
	return IoTLib_convertTimeTypeToSecondsFunc(nearestTimeUntilNextSensorPoll);
}

IoTLib_time_t IoTLib_calculate_time_difference(IoTLib_time_t endTime, IoTLib_time_t startTime)
{
	return endTime - startTime;
}
