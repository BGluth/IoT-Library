#include "run_helper_functions.h"

#include "public_structs.h"
#include "util_functions.h"
#include "managed_array_definitions.h"
#include "user_settings.h"

extern struct IoTLib_MngdKVArray_SnsrIDString IoTLib_sensorIDsAndNames;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_initFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_powerOnFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_generateUploadPayloadFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_pollFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_rawDataToStringFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_retrieveSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_storeSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMinTemps;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMaxTemps;
extern struct IoTLib_MngdKVArray_SnsrIDInt IoTLib_sensorPollFrequencies;

extern struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc;

extern time_t (*IoTLib_retrieveLastUploadTimeFunc)();
extern void (*IoTLib_uploadFunction)(char* urlUploadString);
extern struct IoTLib_RawSensorDataAndSensorID* (*IoTLib_retrieveAllUnsentDataFunc)();
extern size_t (*IoTLib_getStoredUnsentDataCountFunc)();

// Assuming the compiler will inline this function.
void _IoTLib_call_all_void_functions_in_buffer(struct IoTLib_MngdKVArray_SnsrIDDataPtr voidFunctionBuffer)
{
	for (size_t i = 0; i < voidFunctionBuffer.length; i++)
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

	for (size_t i = 0; i < IoTLib_sensorIDsAndNames.length; i++)
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
	for (size_t i = 0; i < activeSensors.length; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensors.array[i];

		 time_t (*getLastPolledTimeFunc)() = IoTLib_MKV_get(&IoTLib_retrieveSensorLastPolledTimeFunctions,
			IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		 time_t timeSensorWasLastPolled = getLastPolledTimeFunc();

		if (!_IoTLib_enough_time_elapsed_for_sensor_poll(timeSensorWasLastPolled, currentSensorID))
		{
			_IoTLib_replace_sensorID_at_current_index_with_first_sensor_from_back_of_buffer_that_can_run(i, timeSensorWasLastPolled, activeSensors);
		}
	}
}

bool _IoTLib_enough_time_elapsed_for_sensor_poll(time_t timeSensorWasLastPolled , IoTLib_SensorID sensorID)
{
	time_t sensorReadFrequency = IoTLib_MKV_get(&IoTLib_sensorPollFrequencies,
		IoTLib_MngdKVArray_SnsrIDInt, sensorID);
	double timeSinceSensorWasLastPolled = difftime(_IoTLib_get_current_time(), timeSensorWasLastPolled);

	return timeSensorWasLastPolled > sensorReadFrequency;
}

void _IoTLib_replace_sensorID_at_current_index_with_first_sensor_from_back_of_buffer_that_can_run(size_t indexOfSensorToSwap,
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

void _IoTLib_poll_data_from_sensors(struct IoTLib_MngdKVArray_SnsrIDDataPtr rawSensorDataBuffer,
	const struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	for (size_t i = 0; i < rawSensorDataBuffer.capacity; i++)
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
	for (size_t i = 0; i < rawSensorDataBuffer.length; i++)
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
	for (size_t i = 0; i < activeSensorIDs.length; i++)
	{
		void (*setSensorLastPolledTimeFunc)(time_t lastPollTime) = IoTLib_MKV_get(
			&IoTLib_storeSensorLastPolledTimeFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, activeSensorIDs.array[i]);
		setSensorLastPolledTimeFunc(_IoTLib_get_current_time());
	}
}

void _IoTLib_upload_all_pending_sensor_data_or_store_new_data_locally(
	struct IoTLib_MngdKVArray_SnsrIDDataPtr newRawSensorDataBuffer, const struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	if (_IoTLib_enough_time_elapsed_for_upload())
	{
		_IoTLib_upload_all_pending_sensor_data(newRawSensorDataBuffer, activeSensorIDs);
	}
	else
	{
		// TODO: Store new data locally.
	}
}

bool _IoTLib_enough_time_elapsed_for_upload()
{
	double timeSinceLastUpdate = difftime(_IoTLib_get_current_time(), IoTLib_retrieveLastUploadTimeFunc());
	return IoTLib_MIN_SECONDS_BETWEEN_UPLOADS < timeSinceLastUpdate;
}

void _IoTLib_upload_all_pending_sensor_data(const struct IoTLib_MngdKVArray_SnsrIDDataPtr newRawSensorDataBuffer,
	const struct IoTLib_MngdArray_SnsrID activeSensorIDs)
{
	size_t storedUnsentSensorPollCount = IoTLib_getStoredUnsentDataCountFunc();
	IoTLib_initialize_managed_array(urlPayloadsBuffer, struct IoTLib_MngdArray_String,
		char*, activeSensorIDs.length + storedUnsentSensorPollCount);

	_IoTLib_generate_url_payloads_for_newly_polled_sensor_data(urlPayloadsBuffer, activeSensorIDs, newRawSensorDataBuffer);
	_IoTLib_generate_url_payloads_for_all_unsent_polled_sensor_data(urlPayloadsBuffer);
	
	for (size_t i = 0; i < urlPayloadsBuffer.length; i++)
	{
		IoTLib_uploadFunction(urlPayloadsBuffer.array[i]);
	}
}

void _IoTLib_generate_url_payloads_for_newly_polled_sensor_data(struct IoTLib_MngdArray_String urlPayloadsBuffer,
	const struct IoTLib_MngdArray_SnsrID activeSensorIDs, const struct IoTLib_MngdKVArray_SnsrIDDataPtr rawSensorDataBuffer)
{
	for (size_t i = 0 ; i < activeSensorIDs.length; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensorIDs.array[i];
		char* (*generateUploadPayloadFunc)(void* rawSensorData) = IoTLib_MKV_get(
			&IoTLib_generateUploadPayloadFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		void* rawSensorData = IoTLib_MKV_get(&rawSensorDataBuffer, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);

		IoTLib_MA_add(&urlPayloadsBuffer, generateUploadPayloadFunc(rawSensorData), IoTLib_MngdArray_String);
	}
}

void _IoTLib_generate_url_payloads_for_all_unsent_polled_sensor_data(struct IoTLib_MngdArray_String urlPayloadsBuffer)
{
	struct IoTLib_RawSensorDataAndSensorID* unsentSensorPollDataBuffer = IoTLib_retrieveAllUnsentDataFunc();

	// Append stored unsent data to buffer.
	for (size_t urlBufPos = urlPayloadsBuffer.length, unsentBufPos = 0;
		urlBufPos < urlPayloadsBuffer.capacity; urlBufPos++, unsentBufPos++)
	{
		struct IoTLib_RawSensorDataAndSensorID currentUnsentSensorData = unsentSensorPollDataBuffer[unsentBufPos];
		char* (*generateUrlPayloadFunc)(void* rawData) = IoTLib_MKV_get(
			&IoTLib_generateUploadPayloadFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, currentUnsentSensorData.id);

		IoTLib_MA_add(&urlPayloadsBuffer, generateUrlPayloadFunc(currentUnsentSensorData.data), IoTLib_MngdArray_String);
	}
}
