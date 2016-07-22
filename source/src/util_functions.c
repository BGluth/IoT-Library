#include "util_functions.h"

#include <stdint.h>
#include <stddef.h>
#include <time.h>

#include "managed_array_definitions.h"

extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_getSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDInt IoTLib_sensorPollFrequencies;

double IoTLib_calculate_time_in_seconds_until_next_sensor_polling()
{
	time_t rawCurrentTime = _IoTLib_get_current_time();
	struct tm* currentTime = localtime(&rawCurrentTime);
	double nearestTimeUntilNextSensorPoll = INTMAX_MAX;

	for (int i = 0; i < IoTLib_getSensorLastPolledTimeFunctions.length; i++)
	{
		int currentSensorFrequency = IoTLib_MKV_get(&IoTLib_sensorPollFrequencies,
			IoTLib_MngdKVArray_SnsrIDInt, IoTLib_getSensorLastPolledTimeFunctions.keys[i]);

		struct tm timeOfCurrentSensorsNextPoll = *currentTime;
		timeOfCurrentSensorsNextPoll.tm_sec += currentSensorFrequency;
		double timeUntilCurrentSensorsNextPoll = difftime(mktime(&timeOfCurrentSensorsNextPoll), rawCurrentTime);

		if (timeUntilCurrentSensorsNextPoll < nearestTimeUntilNextSensorPoll)
			nearestTimeUntilNextSensorPoll = timeUntilCurrentSensorsNextPoll;
	}

	return nearestTimeUntilNextSensorPoll;
}

time_t _IoTLib_get_current_time()
{
	return time(NULL);
}