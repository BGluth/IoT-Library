#include "run_helper_functions.h"

#include "debug_functions.h"
#include "vector.h"
#include "public_structs.h"
#include "util_functions.h"
#include "managed_array_definitions.h"

extern struct IoTLib_MngdKVArray_SnsrIDString IoTLib_sensorIDsAndNames;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_initFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_powerOnFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_generateUploadPayloadFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_pollFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_rawDataToStringFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_retrieveSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_storeSensorLastPolledTimeFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDDataPtr IoTLib_storeUnsentDataFunctions;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMinTemps;
extern struct IoTLib_MngdKVArray_SnsrIDFloat IoTLib_sensorMaxTemps;
extern struct IoTLib_MngdKVArray_SnsrIDTime_t IoTLib_sensorPollFrequencies;
extern struct IoTLib_Vector IoTLib_waitlist_funcs;

extern struct IoTLib_SnsrIDDataPtr IoTLib_tempSnsrIDAndRawToFloatFunc;

extern IoTLib_TIME_T (*IoTLib_retrieveLastUploadTimeFunc)();
extern void (*IoTLib_storeLastUploadTimeFunc)(IoTLib_TIME_T lastActiveTime);
extern void (*IoTLib_uploadFunction)(char* urlUploadString);
extern struct IoTLib_RawSensorDataAndSensorID* (*IoTLib_retrieveAllUnsentDataFunc)();
extern size_t (*IoTLib_getStoredUnsentDataCountFunc)();
extern IoTLib_TIME_T (*IoTLib_getCurrentTimeFunction)();

static IoTLib_TIME_T IoTLib_runFunctionStartTime;

void _IotLib_run_implementation()
{
	IoTLib_runFunctionStartTime = IoTLib_getCurrentTimeFunction();

	IoTLib_initialize_managed_array(activeSensorIDs, struct IoTLib_MngdArray_SnsrID, IoTLib_SensorID, IoTLib_SENSOR_COUNT);
	IoTLib_initialize_managed_array(sensorsToInitAndPowerOn, struct IoTLib_MngdArray_SnsrID, IoTLib_SensorID, IoTLib_SENSOR_COUNT);

	_IoTLib_determine_active_sensors(&activeSensorIDs);
	_IoTLib_get_sensor_ids_to_init_and_potentially_power_on(&activeSensorIDs, &sensorsToInitAndPowerOn);
	_IoTLib_call_init_functions_for_active_sensors(&sensorsToInitAndPowerOn);
	_IoTLib_call_power_on_functions_for_active_sensors(&sensorsToInitAndPowerOn);

	IoTLib_initialize_managed_key_value_array(rawSensorDataBuffer,
		struct IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_SensorID, void*, activeSensorIDs.length);

	_IoTLib_poll_data_from_sensors(&rawSensorDataBuffer, &activeSensorIDs);
	_IoTLib_set_last_poll_time_for_active_sensors(&activeSensorIDs);

	_IoTLib_upload_all_pending_sensor_data_or_store_new_data_locally(&rawSensorDataBuffer, &activeSensorIDs);

	_IoTLib_wait_for_tasks_to_complete();

	_IoTLib_store_current_time_as_upload_time();
}
// Assuming the compiler will inline this function.
void _IoTLib_lookup_and_call_all_void_functions_in_MKV_with_sensorIDs(
	struct IoTLib_MngdArray_SnsrID* sensorIDs,
	struct IoTLib_MngdKVArray_SnsrIDDataPtr* voidFunctionBuffer)
{
	for (size_t i = 0; i < sensorIDs->length; i++)
	{
		_IoTLib_lookup_and_call_void_function_for_sensorID(sensorIDs->array[i], voidFunctionBuffer);
	}
}

void _IoTLib_lookup_and_call_void_function_for_sensorID(IoTLib_SensorID sensorID,
	struct IoTLib_MngdKVArray_SnsrIDDataPtr* voidFunctionBuffer)
{
	void (*voidFunction)() = (void (*)()) IoTLib_MKV_get(
		voidFunctionBuffer, IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID);
	voidFunction();
}

void _IoTLib_get_sensor_ids_to_init_and_potentially_power_on(const struct IoTLib_MngdArray_SnsrID* activeSensorIDs,
	struct IoTLib_MngdArray_SnsrID* sensorsIdsToInitAndPowerOn)
{
	for (size_t i = 0; i < activeSensorIDs->length; i++)
	{
		if (IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES && activeSensorIDs->array[i] != IoTLib_tempSnsrIDAndRawToFloatFunc.id)
		{
			IoTLib_MA_add(sensorsIdsToInitAndPowerOn, activeSensorIDs->array[i], IoTLib_MngdArray_SnsrID);
		}
	}
}

void _IoTLib_call_init_functions_for_active_sensors(struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	IoTLib_debug_info("Calling sensor init functions. (Count: %i)", activeSensorIDs->length);
	_IoTLib_lookup_and_call_all_void_functions_in_MKV_with_sensorIDs(activeSensorIDs, &IoTLib_initFunctions);
}

void _IoTLib_call_power_on_functions_for_active_sensors(struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	IoTLib_debug_info("Calling sensor power on functions.");

	for (size_t i = 0; i < activeSensorIDs->length; i++)
	{
		_IoTLib_try_lookup_and_call_void_function_in_MKV_with_sensorIDs(activeSensorIDs->array[i], &IoTLib_powerOnFunctions);
	}
}

void _IoTLib_try_lookup_and_call_void_function_in_MKV_with_sensorIDs(IoTLib_SensorID sensorID,
	struct IoTLib_MngdKVArray_SnsrIDDataPtr* voidFunctionBuffer)
{
	void* rawVoidFunction;
	if (IoTLib_MKV_try_get(voidFunctionBuffer, IoTLib_MngdKVArray_SnsrIDDataPtr, sensorID, &rawVoidFunction))
	{
		void (*voidFunction)() = (void (*)())rawVoidFunction;
		voidFunction();
	}
}

void _IoTLib_determine_active_sensors(struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	_IoTLib_add_all_sensors_to_active_sensors(activeSensorIDs);

	IoTLib_debug_info("Filtering out sensors by last poll time and sensor frequency...");
	_IoTLib_filter_out_sensors_by_poll_frequency(activeSensorIDs);
	IoTLib_debug_info("Active sensor count after frequency filter: %i", activeSensorIDs->length);
	_IoTLib_debug_active_sensor_names_and_ids(activeSensorIDs);

	if (IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES)
	{
		if (activeSensorIDs->length > 0)
		{
			_IoTLib_init_and_poweron_temp_sensor();

			IoTLib_debug_info("Filtering active sensors by current temp...");
			_IoTLib_determine_active_sensors_by_current_temp(activeSensorIDs);
			IoTLib_debug_info("Active sensor count after temp filter: %i", activeSensorIDs->length);
			_IoTLib_debug_active_sensor_names_and_ids(activeSensorIDs);
		}
		else
			IoTLib_debug_info("No sensors to filter by current temp.");
	}
	else
	{
		IoTLib_debug_info("Not filtering by current temp.");
	}
}

void _IoTLib_init_and_poweron_temp_sensor()
{
	_IoTLib_lookup_and_call_void_function_for_sensorID(IoTLib_tempSnsrIDAndRawToFloatFunc.id, &IoTLib_initFunctions);
	_IoTLib_try_lookup_and_call_void_function_in_MKV_with_sensorIDs(IoTLib_tempSnsrIDAndRawToFloatFunc.id, &IoTLib_powerOnFunctions);
}

void _IoTLib_determine_active_sensors_by_current_temp(struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	// TODO: What if temperature sensor has a power on function?
	void* (*rawTemperatureSensorReadFunc)() = (void* (*)()) IoTLib_MKV_get(
		&IoTLib_pollFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_tempSnsrIDAndRawToFloatFunc.id);
	float (*temperatureRawToFloatFunc)(void* rawTempSnsrData) = (float (*)(void*)) IoTLib_tempSnsrIDAndRawToFloatFunc.data;

	void* rawTemperatureData = rawTemperatureSensorReadFunc();
	float currentTemperature = temperatureRawToFloatFunc(rawTemperatureData);


	for (int i = (int)activeSensorIDs->length - 1; i >= 0; i--)
	{
		IoTLib_debug_info("IN LOOP IN TEMP FILTER FUNC!");
		IoTLib_SensorID sensorID = activeSensorIDs->array[i];
		if (!_IoTLib_sensor_can_operate_in_current_temperature(currentTemperature, sensorID))
		{
			_IoTLib_swap_with_end_of_sensor_id_array(activeSensorIDs, i);
		}
	}
}

void _IoTLib_swap_with_end_of_sensor_id_array(struct IoTLib_MngdArray_SnsrID* sensorIDArray, size_t currentIndex)
{
	sensorIDArray->length--;
	sensorIDArray->array[currentIndex] = sensorIDArray->array[sensorIDArray->length];
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

void _IoTLib_add_all_sensors_to_active_sensors(struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	activeSensorIDs->array = IoTLib_sensorIDsAndNames.keys;
	activeSensorIDs->length = IoTLib_sensorIDsAndNames.length;
}

void _IoTLib_filter_out_sensors_by_poll_frequency(struct IoTLib_MngdArray_SnsrID* activeSensors)
{
	for (size_t i = 0; i < activeSensors->length; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensors->array[i];

		IoTLib_TIME_T (*getLastPolledTimeFunc)() = (IoTLib_TIME_T (*)()) IoTLib_MKV_get(&IoTLib_retrieveSensorLastPolledTimeFunctions,
			IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		IoTLib_TIME_T timeSensorWasLastPolled = getLastPolledTimeFunc();

		if (!_IoTLib_enough_time_elapsed_for_sensor_poll(timeSensorWasLastPolled, currentSensorID))
		{
			_IoTLib_replace_sensorID_at_current_index_with_first_sensor_from_back_of_buffer_that_can_run(i, timeSensorWasLastPolled, activeSensors);
		}
	}
}

bool _IoTLib_enough_time_elapsed_for_sensor_poll(IoTLib_TIME_T timeSensorWasLastPolled , IoTLib_SensorID sensorID)
{
	IoTLib_TIME_T sensorReadFrequency = IoTLib_MKV_get(&IoTLib_sensorPollFrequencies,
		IoTLib_MngdKVArray_SnsrIDTime_t, sensorID);
	double timeSinceSensorWasLastPolled = IoTLib_calculate_time_difference_in_seconds(IoTLib_runFunctionStartTime, timeSensorWasLastPolled);

	return timeSinceSensorWasLastPolled > sensorReadFrequency;
}

void _IoTLib_replace_sensorID_at_current_index_with_first_sensor_from_back_of_buffer_that_can_run(size_t indexOfSensorToSwap,
	IoTLib_TIME_T timeSinceSensorWasLastPolled, struct IoTLib_MngdArray_SnsrID* activeSensors)
{
	while (activeSensors->length > indexOfSensorToSwap &&
		!_IoTLib_enough_time_elapsed_for_sensor_poll(timeSinceSensorWasLastPolled, activeSensors->array[activeSensors->length]))
	{
		activeSensors->length--;
	}

	activeSensors->array[indexOfSensorToSwap] = activeSensors->array[activeSensors->length];
	if (activeSensors->length > 0)
		activeSensors->length--;
}

void _IoTLib_poll_data_from_sensors(struct IoTLib_MngdKVArray_SnsrIDDataPtr* rawSensorDataBuffer,
	const struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	IoTLib_debug_info("Polling data from sensors...");

	for (size_t i = 0; i < rawSensorDataBuffer->capacity; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensorIDs->array[i];
		void* (*sensorReadFunc)() = (void* (*)()) IoTLib_MKV_get(&IoTLib_pollFunctions,
			IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		void* rawSensorData = sensorReadFunc();

		IoTLib_MKV_insert(rawSensorDataBuffer, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID, rawSensorData);
	}
}

void _IoTLib_get_string_represenations_of_raw_sensor_data(struct IoTLib_MngdKVArray_SnsrIDString* stringSensorDataBuffer,
	const struct IoTLib_MngdKVArray_SnsrIDDataPtr* rawSensorDataBuffer)
{
	for (size_t i = 0; i < rawSensorDataBuffer->length; i++)
	{
		IoTLib_SensorID currentSensorID = rawSensorDataBuffer->keys[i];
		char* (*rawSensorDataToStringFunc)(void* rawSensorData) = (char* (*)(void*))
			IoTLib_MKV_get(&IoTLib_rawDataToStringFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		char* stringSensorData = rawSensorDataToStringFunc(rawSensorDataBuffer->values[i]);

		IoTLib_MKV_insert(stringSensorDataBuffer, IoTLib_MngdKVArray_SnsrIDString, currentSensorID, stringSensorData);
	}
}

void _IoTLib_set_last_poll_time_for_active_sensors(struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	IoTLib_debug_info("Setting all active sensors last poll time to current time...");

	for (size_t i = 0; i < activeSensorIDs->length; i++)
	{
		void (*setSensorLastPolledTimeFunc)(IoTLib_TIME_T lastPollTime) = (void (*)(IoTLib_TIME_T)) IoTLib_MKV_get(
			&IoTLib_storeSensorLastPolledTimeFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, activeSensorIDs->array[i]);
		setSensorLastPolledTimeFunc(IoTLib_runFunctionStartTime);
	}
}

void _IoTLib_upload_all_pending_sensor_data_or_store_new_data_locally(
	struct IoTLib_MngdKVArray_SnsrIDDataPtr* newRawSensorDataBuffer, const struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	if (_IoTLib_enough_time_elapsed_for_upload())
	{
		IoTLib_debug_info("Uploading all new and stored polled sensor results...");
		_IoTLib_upload_all_pending_sensor_data(newRawSensorDataBuffer, activeSensorIDs);
	}
	else
	{
		IoTLib_debug_info("Not enough time elapsed for uploading. Storing new data locally...");
		_IoTLib_store_newly_polled_sensor_data_locally(newRawSensorDataBuffer);
	}
}

bool _IoTLib_enough_time_elapsed_for_upload()
{
	double timeSinceLastUpdate = IoTLib_calculate_time_difference_in_seconds(IoTLib_runFunctionStartTime, IoTLib_retrieveLastUploadTimeFunc());
	return IoTLib_MIN_SECONDS_BETWEEN_UPLOADS < timeSinceLastUpdate;
}

void _IoTLib_upload_all_pending_sensor_data(const struct IoTLib_MngdKVArray_SnsrIDDataPtr* newRawSensorDataBuffer,
	const struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	size_t storedUnsentSensorPollCount = IoTLib_getStoredUnsentDataCountFunc();
	_IoTLib_debug_print_unsent_stored_sensor_polls(storedUnsentSensorPollCount);

	IoTLib_initialize_managed_array(urlPayloadsBuffer, struct IoTLib_MngdArray_String,
		char*, activeSensorIDs->length + storedUnsentSensorPollCount);

	_IoTLib_generate_url_payloads_for_newly_polled_sensor_data(&urlPayloadsBuffer, activeSensorIDs, newRawSensorDataBuffer);
	_IoTLib_generate_url_payloads_for_all_unsent_polled_sensor_data(&urlPayloadsBuffer);
	
	IoTLib_debug_info("Uploading %i URL payloads...", urlPayloadsBuffer.length);
	for (size_t i = 0; i < urlPayloadsBuffer.length; i++)
	{
		IoTLib_uploadFunction(urlPayloadsBuffer.array[i]);
	}
}

void _IoTLib_generate_url_payloads_for_newly_polled_sensor_data(struct IoTLib_MngdArray_String* urlPayloadsBuffer,
	const struct IoTLib_MngdArray_SnsrID* activeSensorIDs, const struct IoTLib_MngdKVArray_SnsrIDDataPtr* rawSensorDataBuffer)
{
	IoTLib_debug_info("Generating URL payloads for all new sensor polls...");

	for (size_t i = 0 ; i < activeSensorIDs->length; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensorIDs->array[i];
		char* (*generateUploadPayloadFunc)(void* rawSensorData) = (char* (*)(void*)) IoTLib_MKV_get(
			&IoTLib_generateUploadPayloadFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);
		void* rawSensorData = IoTLib_MKV_get(rawSensorDataBuffer, IoTLib_MngdKVArray_SnsrIDDataPtr, currentSensorID);

		IoTLib_MA_add(urlPayloadsBuffer, generateUploadPayloadFunc(rawSensorData), IoTLib_MngdArray_String);
	}
}

void _IoTLib_generate_url_payloads_for_all_unsent_polled_sensor_data(struct IoTLib_MngdArray_String* urlPayloadsBuffer)
{
	struct IoTLib_RawSensorDataAndSensorID* unsentSensorPollDataBuffer = IoTLib_retrieveAllUnsentDataFunc();

	// Append stored unsent data to buffer.
	for (size_t urlBufPos = urlPayloadsBuffer->length, unsentBufPos = 0;
		urlBufPos < urlPayloadsBuffer->capacity; urlBufPos++, unsentBufPos++)
	{
		struct IoTLib_RawSensorDataAndSensorID currentUnsentSensorData = unsentSensorPollDataBuffer[unsentBufPos];
		char* (*generateUrlPayloadFunc)(void* rawData) = (char* (*)(void*)) IoTLib_MKV_get(
			&IoTLib_generateUploadPayloadFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, currentUnsentSensorData.id);

		IoTLib_MA_add(urlPayloadsBuffer, generateUrlPayloadFunc(currentUnsentSensorData.data), IoTLib_MngdArray_String);
	}
}

void _IoTLib_store_newly_polled_sensor_data_locally(const struct IoTLib_MngdKVArray_SnsrIDDataPtr* newRawSensorDataBuffer)
{
	for (size_t i = 0; i < newRawSensorDataBuffer->length; i++)
	{
		void (*storeNewlyPolledSensorDataFunc)(void* rawSensorData) = (void (*)(void*)) IoTLib_MKV_get(
			&IoTLib_storeUnsentDataFunctions, IoTLib_MngdKVArray_SnsrIDDataPtr, newRawSensorDataBuffer->keys[i]);

		storeNewlyPolledSensorDataFunc(newRawSensorDataBuffer->values[i]);
	}
}

void _IoTLib_wait_for_tasks_to_complete()
{
	IoTLib_debug_info("Checking if there are any tasks on the wait list needing to be completed...");
	while (IoTLib_waitlist_funcs.count > 0)
	{
		IoTLib_debug_info("%i tasks waiting to be completed.", IoTLib_waitlist_funcs.count);
		IoTLib_debug_info("Attempting to complete %i tasks...", IoTLib_waitlist_funcs.count);

		for (size_t i = 0; i < IoTLib_waitlist_funcs.count; i++)
		{
			bool (*checkAndHandleTaskCompletionFunc)() =
				(bool (*)()) IoTLib_vector_get(&IoTLib_waitlist_funcs, i);
			bool completed = checkAndHandleTaskCompletionFunc();
			if (completed)
			{
				IoTLib_vector_delete(&IoTLib_waitlist_funcs, i);
			}
		}
	}
}

// Debug helper functions:
void _IoTLib_debug_active_sensor_names_and_ids(struct IoTLib_MngdArray_SnsrID* activeSensorIDs)
{
	for (size_t i = 0; i < activeSensorIDs->length; i++)
	{
		IoTLib_SensorID currentSensorID = activeSensorIDs->array[i];
		const char* currentSensorName = IoTLib_MKV_get(&IoTLib_sensorIDsAndNames, IoTLib_MngdKVArray_SnsrIDString, currentSensorID);

		IoTLib_debug_info("Name - %s  ID - %i", currentSensorName, currentSensorID);
	}
}

void _IoTLib_debug_print_unsent_stored_sensor_polls(size_t storedUnsentPollCount)
{
	if (storedUnsentPollCount > 0)
		IoTLib_debug_info("Generating URL payloads for %i unsent stored sensor polls...", storedUnsentPollCount);
}

void _IoTLib_store_current_time_as_upload_time()
{
	IoTLib_storeLastUploadTimeFunc(IoTLib_getCurrentTimeFunction());
}

