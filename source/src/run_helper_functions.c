#include "run_helper_functions.h"

#include <stddef.h>

#include "managed_array_definitions.h"
#include "user_settings.h"

extern struct IoTLib_MngdKVArray_SnsrIDString IoTLib_sensorIDsAndNames;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_initFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_powerOnFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_pollFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_rawDataToStringFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_getSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_setSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMinTemps;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMaxTemps;
extern struct IoTLib_MngdKVArray_SnsrIDInt IoTLib_sensorPollFrequencies;

extern struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc;
extern time_t (*IoTLib_readDeviceLastActiveTimeFunc)();

// Assuming the compiler will inline this function.
void _IoTLib_call_all_void_functions_in_buffer(struct IoTLib_MngdKVArray_SnsrIDDataPtr voidFunctionBuffer)
{
	for (int i = 0; i < voidFunctionBuffer.length; i++)
	{
		void (*voidFunction)() = voidFunctionBuffer.values[i];
		voidFunction();
	}
}

void _IoTLib_call_sensor_init_functions()
{
	_IoTLib_call_all_void_functions_in_buffer(IoTLib_initFunctions);
}

void _IoTLib_call_sensor_power_on_functions()
{
	_IoTLib_call_all_void_functions_in_buffer(IoTLib_powerOnFunctions);
}

void _IoTLib_determine_active_sensors(struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	if (IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES)
		_IoTLib_determine_active_sensors_by_current_temp(activeSensorIDs);
	else
		_IoTLib_add_all_sensors_to_active_sensors(activeSensorIDs);

	_IoTLib_filter_out_sensors_by_poll_frequency(activeSensorIDs);
}

void _IoTLib_determine_active_sensors_by_current_temp(struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	// TODO: What if temperature sensor has a power on function?
	void* (*rawTemperatureSensorReadFunc)() = IoTLib_MKV_get(
		&IoTLib_initFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_tempSnsrIDAndRawToFloatFunc.id);
	float (*temperatureRawToFloatFunc)(void* rawTempSnsrData) = IoTLib_tempSnsrIDAndRawToFloatFunc.data;

	void* rawTemperatureData = rawTemperatureSensorReadFunc();
	float currentTemperature = temperatureRawToFloatFunc(rawTemperatureData);

	for (int i = 0; i < IoTLib_sensorIDsAndNames.length; i++)
	{
		IoTLib_SensorID sensorID = IoTLib_sensorIDsAndNames.keys[i];
		if (_IoTLib_sensor_can_operate_in_current_temperature(currentTemperature, sensorID))
		{
			IoTLib_MA_add(&activeSensorIDs, sensorID, IoTLib_MngdArray_SnsrID);
		}
	}
}

bool _IoTLib_sensor_can_operate_in_current_temperature(float currentTemperature, IoTLib_SensorID sensorID)
{
	float minTemp;
	if (IoTLib_MKV_try_get(&IoTLib_sensorMinTemps, IoTLib_MngdKVArray_SnsrIDFloat, sensorID, &minTemp)
			&& currentTemperature < minTemp)
		return false;

	float maxTemp;
	if (IoTLib_MKV_try_get(&IoTLib_sensorMaxTemps, IoTLib_MngdKVArray_SnsrIDFloat, sensorID, &maxTemp)
			&& currentTemperature > maxTemp)
		return false;

	return true;
}

void _IoTLib_add_all_sensors_to_active_sensors(struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	activeSensorIDs.array = IoTLib_sensorIDsAndNames.keys;
	activeSensorIDs.length = IoTLib_sensorIDsAndNames.length;
}

void _IoTLib_filter_out_sensors_by_poll_frequency(struct IoTLib_MngdArray_SnsrID activeSensors)
{
	for (int i = 0; i < activeSensors.length; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensors.array[i];

		 time_t (*getLastPolledTimeFunc)() = IoTLib_MKV_get(&IoTLib_getSensorLastPolledTimeFunctions,
			IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		 time_t timeSinceSensorWasLastPolled = getLastPolledTimeFunc();

		if (!_IoTLib_enough_time_elapsed_for_sensor_poll(timeSinceSensorWasLastPolled, currentSensorID))
		{
			_IoTLib_replace_sensorID_at_current_index_with_first_sensor_from_back_of_buffer_that_can_run(i, timeSinceSensorWasLastPolled, activeSensors);
		}
	}
}

bool _IoTLib_enough_time_elapsed_for_sensor_poll(time_t timeSinceSensorWasLastPolled , IoTLib_SensorID sensorID)
{
	int currentSensorReadFrequency = IoTLib_MKV_get(&IoTLib_sensorPollFrequencies,
		IoTLib_MngdKVArray_SnsrIDInt, sensorID);

	return timeSinceSensorWasLastPolled > currentSensorReadFrequency;
}

void _IoTLib_replace_sensorID_at_current_index_with_first_sensor_from_back_of_buffer_that_can_run(int indexOfSensorToSwap,
	time_t timeSinceSensorWasLastPolled, struct IoTLib_MngdArray_SnsrID activeSensors)
{
	while (activeSensors.length > indexOfSensorToSwap &&
		!_IoTLib_enough_time_elapsed_for_sensor_poll(timeSinceSensorWasLastPolled, activeSensors.array[activeSensors.length]))
	{
		activeSensors.length--;
	}

	activeSensors.array[indexOfSensorToSwap] = activeSensors.array[activeSensors.length];
	activeSensors.length--;
}

void _IoTLib_read_and_store_data_from_sensors(struct IoTLib_MngdKVArray_SnsrIDDataPtr rawSensorDataBuffer,
	const struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	for (int i = 0; i < rawSensorDataBuffer.capacity; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensorIDs.array[i];
		void* (*sensorReadFunc)() = IoTLib_MKV_get(&IoTLib_pollFunctions,
			IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		void* rawSensorData = sensorReadFunc();

		IoTLib_MKV_insert(&rawSensorDataBuffer, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID, rawSensorData);
	}
}

void _IoTLib_get_string_represenations_of_raw_sensor_data(struct IoTLib_MngdKVArray_SnsrIDString stringSensorDataBuffer,
	const struct IoTLib_MngdKVArray_SnsrIDDataPtr rawSensorDataBuffer)
{
	for (int i = 0; i < rawSensorDataBuffer.length; i++)
	{
		IoTLib_SensorID currentSensorID = rawSensorDataBuffer.keys[i];
		char* (*rawSensorDataToStringFunc)(void* rawSensorData) =
			IoTLib_MKV_get(&IoTLib_rawDataToStringFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		char* stringSensorData = rawSensorDataToStringFunc(rawSensorDataBuffer.values[i]);

		IoTLib_MKV_insert(&stringSensorDataBuffer, IoTLib_MngdKVArray_SnsrIDString, currentSensorID, stringSensorData);
	}
}

void _IoTLib_set_last_poll_time_for_active_sensors(struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	for (int i = 0; i < activeSensorIDs.length; i++)
	{
		void (*setSensorLastPolledTimeFunc)(time_t lastPollTime) = IoTLib_MKV_get(
			&IoTLib_setSensorLastPolledTimeFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, activeSensorIDs.array[i]);
		setSensorLastPolledTimeFunc(time(NULL));
	}
}
