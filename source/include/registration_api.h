/**
 * @defgroup Registration_API
 * @{
 */

/** \file registration_api.h
 * \brief The core API to interact with the library.
 *
 * Provides API calls to register user defined functions that will be called when the library is run.
 */

#ifndef IoTLib_SENSOR_REGISTRAITION_H
#define IoTLib_SENSOR_REGISTRAITION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "user_settings.h"
#include "typedefs.h"
#include "public_structs.h"

/** \brief "Runs" the library.
 *
 * Performs a single run through the library. In short:
 * - Determining what sensors should be polled based on their operating temperature and their polling frequency
 * - Calling each sensor's init and (if it exists) poweron function
 * - Polling each sensor for a reading
 * - Uploading the newly polled data if enough time has passed for an upload and storing it locally if not
 * - Uploading any already locally stored data if enough time has passed for an upload
 * - Updating and locally storing the library's last upload time as well as each sensor's last poll time
 *
 * - (Future) Call any "clean up" functions that a sensor may have (sensor sleep mode, etc)
 * - (In Progress) Provide warm up time for any sensors that need one
 *
 * The intent is to let the library user do whatever they want after they call the run function.
 * Most likely, the user will want to put the device into a sleep mode until the
 * minimum amount time has passed for another sensor to be polled.
 *
 * @see IoTLib_calculate_time_in_seconds_until_next_sensor_polling()
 */
void IoTLib_run();

/** \brief Initializes the library and performs simple checks.
 *
 * This function should be called after all functions/values have been registered but before
 * calling the run function.
 *
 * The function will also perform simple checks to make sure that all required
 * functions/values have been registered and will print out an error and halt
 * (if IoTLib_DEBUG_HALT_ON_ERROR is set to 1 in user_settings) if any required
 * functions/values have not been registered.
 *
 * Also initializes a vector that requires dynamic memory allocation.
 *
 * @see IoTLib_run()
 */
void IoTLib_sensor_registration_init();

/** \brief Registers a sensor.
 *
 * Registers a sensor with a given string and returns its sensorID which
 * can be used to register other functions for the sensor.
 *
 * @param sensorName The name of the sensor which is used for debugging purposes only
 * @return The SensorID of the newly registered sensor
 */
IoTLib_SensorID IoTLib_register_sensor(const char* sensorName);

/** \brief Registers an init function for a specific sensor.
 *
 * The init function is called before anything is done with the sensor.
 * The idea is to do any sensor initialization that will be undone by the future
 * (currently unimplemented) function cleanup (opposite of init) once the run
 * function has finished.
 *
 * Things to note:
 * - Currently every sensor needs an init function, even if the init
 * function does nothing. This was a design mistake and will be fixed in the future.
 * - The init function is called once per sensor in the IoTLib_run if the sensor is
 * able to run. If you are *not* sleeping the device (MCU) after calling the run
 * function, note that a sensor's init function may be called multiple times
 * if run is called multiple times without powering down the device.
 *
 * @param sensorID The SensorID that the function belongs to.
 * @param sensorInitFunc The init function for the sensor.
 *
 * @see IoTLib_register_sensor_power_on_function(IoTLib_SensorID sensorID, void (*powerOnFunc)())
 * @see IoTLib_run()
 */
void IoTLib_register_sensor_init_function(IoTLib_SensorID sensorID, void (*sensorInitFunc)());

/** \brief Registers a function to poll data from a sensor.
 *
 * The poll function should return a pointer to the polled data as a void*.
 *
 * @param sensorID The SensorID that the function belongs to.
 * @param pollSensorFunc The function that knows how to poll the sensor.
 */
void IoTLib_register_sensor_poll_function(IoTLib_SensorID sensorID, void* (*pollSensorFunc)());

/** \brief CURRENTLY NOT USED
 *
 * SLATED FOR REMOVAL
 */
void IoTLib_register_sensor_convert_raw_sensor_data_to_string_function(IoTLib_SensorID sensorID, char* (*dataToStringFunc)(void* rawSensorData));

/** \brief Registers a function to power on a sensor.
 *
 * Called immediately after all sensor init functions have been called.
 *
 * Right now I'm not seeing a good reason to separate the init and poweron function
 * so there is a good chance that these two functions will be merged.
 *
 * @param sensorID The SensorID that the function belongs to.
 * @param powerOnFunc The function to power on the sensor.
 */
void IoTLib_register_sensor_power_on_function(IoTLib_SensorID sensorID, void (*powerOnFunc)());

/** \brief Register a function to store polled sensor data offline when
 *  uploading fails or is not possible.
 *
 * This function should store the data passed to it in non-volatile memory
 * (SD card, EEPROM, FLASH, etc.) for later reading and deletion once the data
 * has been successfully uploaded. The function should somehow keep track of
 * which data belongs to which sensor, as when the library wants to send unsent data,
 * it expects it in the form of an array of IoTLib_RawSensorDataAndSensorID.
 * This is fairly easy when using a files system (like an SD card) because then
 * you can just group unsent data by sensors in separate files. However, when using
 * something like FLASH or EEPROM, this can get tricky.
 *
 * @param sensorID The SensorID that the function belongs to.
 * @param rawSensorData A void* pointer to the polled sensor data. You will need to cast this to the appropriate type.
 *
 * @see IoTLib_register_retrieve_all_stored_unsent_sensor_data_function(struct IoTLib_RawSensorDataAndSensorID* (*retrieveAllUnsentDataFunc)())
 * @see IoTLib_register_get_stored_unsent_data_count_function(size_t (*getStoredUnsentDataCountFunc)());
 */
void IoTLib_register_sensor_store_unsent_data_function(IoTLib_SensorID sensorID, void (*storeUnsentFunc)(void* rawSensorData));

/** \brief Registers a function to generate a URL payload that will be used to
 *  send the polled sensor data to the server.
 *
 * Given the polled sensor data, this function should generate a URL payload
 * to get the newly polled sensor data to the server backend.
 *
 * @param sensorID The SensorID that the function belongs to.
 * @param generateUploadPayloadFunc The function to generate the URL payload.
 * @param rawSensorData A void* pointer to the polled sensor data. You will need to cast this to the appropriate type.
 *
 * @see IoTLib_register_upload_function(void (*uploadFunction)(char* urlUploadString))
 */
void IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData));

/** \brief Registers a function to retrieve the last poll time for a sensor.
 *
 * The function is used internally to determine whether or not enough time has
 * passed for the sensor to be polled based on the sensor's polling frequency.
 *
 * @param sensorID The SensorID that the function belongs to.
 * @param retrieveSensorLastPolledTimeFunc The function to retrieve the sensor's last polled time.
 *
 * @see IoTLib_register_sensor_store_last_polled_time_function(IoTLib_SensorID sensorID, void (*storeSensorLastPolledTimeFunc)(IoTLib_TIME_T lastPollTime))
 * @see IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, IoTLib_TIME_T sensorReadFrequency)
 * @see IoTLib_register_get_current_time_function(IoTLib_TIME_T (*getCurrentTimeFunc)())
 */
void IoTLib_register_sensor_retrieve_last_polled_time_function(IoTLib_SensorID sensorID, IoTLib_TIME_T (*retrieveSensorLastPolledTimeFunc)());

/** \brief Registers a function to locally store the last polled time for a sensor.
 *
 * Function should somehow store the passed in polled time in a way that the value
 * will be accessible after the device is powered down. The most common way would
 * probably be to write the data to non-volatile memory (SD card, EEPROM, etc.).
 *
 * @param sensorID The SensorID that the function belongs to.
 * @param storeSensorLastPolledTimeFunc The function to store the sensor's last polled time.
 *
 * @see IoTLib_register_sensor_generate_upload_payload_function(IoTLib_SensorID sensorID, char* (*generateUploadPayloadFunc)(void* rawSensorData))
 * @see IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, IoTLib_TIME_T sensorReadFrequency)
 * @see IoTLib_register_get_current_time_function(IoTLib_TIME_T (*getCurrentTimeFunc)())
 */
void IoTLib_register_sensor_store_last_polled_time_function(IoTLib_SensorID sensorID, void (*storeSensorLastPolledTimeFunc)(IoTLib_TIME_T lastPollTime));

/** \brief Sets the poll frequency for a sensor
 *
 * The poll frequency of a sensor is simply the minimum amount of time in
 * units defined by the IoTLib_TIME_T macro definition that the sensor must wait
 * in between polls. The user is *free* to use whatever units they want, as long as
 * the units used in the library are consistent.
 *
 * For example, the get current time function should return the time in the same
 * units as used by the poll frequencies.
 *
 * @param sensorID The SensorID that the poll frequency is for.
 * @param sensorReadFrequency The minimum amount of time (in units of IoTLib_TIME_T)
 * that must pass after a sensor poll before the sensor can be polled again.
 *
 * @see IoTLib_TIME_T
 */
void IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, IoTLib_TIME_T sensorReadFrequency);

/** \brief Registers the minimum temperature that a sensor will be polled at.
 *
 * In temperatures below this minimum the sensor will simply be skipped.
 * Make sure that the temperature units are the same as the units output by the
 * raw to temp function.
 *
 * @param sensorID The SensorID that has the minimum operating temperature.
 * @param minTemp The minimum temperature that the sensor will be polled at.
 *
 * @see IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp)
 * @see IoTLib_register_temp_sensorid_and_raw_to_temp_function(IoTLib_SensorID tempSensorID, float (*rawToTempFunc)(void* rawSensorData))
 */
void IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp);

/** \brief Registers the maximum temperature that a sensor will be polled at.
 *
 * In temperatures above this minimum the sensor will simply be skipped.
 * Make sure that the temperature units are the same as the units output by the
 * raw to temp function.
 *
 * @param sensorID The SensorID that has the minimum operating temperature.
 * @param maxTemp The maximum temperature that the sensor will be polled at.
 *
 * @see IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp)
 * @see IoTLib_register_temp_sensorid_and_raw_to_temp_function(IoTLib_SensorID tempSensorID, float (*rawToTempFunc)(void* rawSensorData))
 */
void IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp);

/** \brief This function should upload the given URL payload.
 *
 * While the library does not directly do anything with any response from the payload,
 * it is a very good idea to handle the case when an upload fails. To handle this,
 * the upload function should also enqueue a task to the library's task waitlist.
 *
 * As a simple example, the upload init function could create a task that simply
 * goes over a list of in-progress uploads where each upload would eventually fail
 * if the upload times out. The actual upload function would simply add an entry
 * to the in-progress uploads. Tasks are looped over at the end of the run function
 * until all of the tasks are complete.
 *
 * @param uploadFunction The function that handles uploading individual URL uploads.
 * @param urlUploadString The URL payload.
 *
 * @see IoTLib_add_task_to_waitlist(bool (*checkAndHandleTaskCompletionFunc)()
 * @see IoTLib_register_upload_init_function(void (*uploadInitFunc)()
 */
void IoTLib_register_upload_function(void (*uploadFunction)(char* urlUploadString));

/** \brief Function that handles the debug/warning/error output from the library.
 *
 * Also a good spot for logging library output.
 *
 * @param debugFunction The function that handles the library output.
 * @param debugString The string of text to be output.
 */
void IoTLib_register_debug_function(void (*debugFunction)(char* debugString));

/** \brief Register the sensorID of the temperature sensor along with a function
 * to convert the sensor's polled output to a temperature as a float.
 *
 * The temperature returned by the function should be in the same units as the
 * min/max temperatures set for sensors.
 *
 * Note that the temperature sensor will always be run regardless of its
 * min/max temperature ratings and its polling frequency. This is because every
 * time we run the library, we need to know the current temperature to determine
 * what sensors can be run.
 *
 * There is a bit of a conundrum here though. How can we know the current temperature
 * without polling the temperature sensor? If it's outside of its temp range, the data we get
 * back might be invalid. I honestly don't know what the right thing to do here is.
 * Maybe disable all sensor readings if the temp sensor is at its limit
 * even if it's operating outside of its temperature range.
 *
 * @param tempSensorID The sensorID of the temperature sensor.
 * @param rawToTempFunc The function that converts the passed in raw polled sensor
 * output into a float temperature.
 * @param rawSensorData The raw temperature poll data.
 *
 * @see IoTLib_register_sensor_min_operating_temp(IoTLib_SensorID sensorID, float minTemp)
 * @see IoTLib_register_sensor_max_operating_temp(IoTLib_SensorID sensorID, float maxTemp)
 * @see IoTLib_register_sensor_poll_function(IoTLib_SensorID sensorID, void* (*pollSensorFunc)())
 */
void IoTLib_register_temp_sensorid_and_raw_to_temp_function(IoTLib_SensorID tempSensorID, float (*rawToTempFunc)(void* rawSensorData));

/** \brief Registers a function to locally store the last device upload time.
 *
 * The upload time should be stored so the value will still be accessible after
 * the device is powered down (SD card, EEPROM, etc.).
 *
 * The upload time will be stored at the very end of the run function.
 *
 * @param storeLastUploadTimeFunc The function to store and update the last device
 * upload time.
 * @param lastUploadTime The updated last upload time.
 *
 * @see IoTLib_run()
 */
void IoTLib_register_store_last_upload_time_function(void (*storeLastUploadTimeFunc)(IoTLib_TIME_T lastUploadTime));

/** \brief Registers a function to get the last upload time.
 *
 * The function is used internally to determine whether or not enough time has
 * passed to upload.
 *
 * @param retireveLastUploadTimeFunc The function to get the last device upload
 * time.
 *
 * @see void IoTLib_register_upload_function(void (*uploadFunction)(char* urlUploadString))
 * @see IoTLib_register_temp_sensorid_and_raw_to_temp_function(IoTLib_SensorID tempSensorID, float (*rawToTempFunc)(void* rawSensorData))
 * @see IoTLib_MIN_SECONDS_BETWEEN_UPLOADS
 */
void IoTLib_register_retrieve_last_upload_time_function(IoTLib_TIME_T (*retireveLastUploadTimeFunc)());

/** \brief Registers a function to retrieve all of the stored unsent data as an
 * array of unsent data paired with the sensor's ID.
 *
 * *IMPORTANT NOTE*: The function signature is likely to change in the future.
 * As an example, if the
 * amount of stored unsent data is very large, it could easily cause an out of
 * memory error on the device. Instead, the function will likely be changed to
 * something like where a parameter is being passed in to request only a certain
 * amount of unsent data, and then have the function be called multiple times until
 * there is no more unsent data.
 *
 * @param retrieveAllUnsentDataFunc The function to retrieve all of the unsent data.
 *
 * @see IoTLib_register_sensor_store_unsent_data_function(IoTLib_SensorID sensorID, void (*storeUnsentFunc)(void* rawSensorData))
 * @see IoTLib_register_get_stored_unsent_data_count_function(size_t (*getStoredUnsentDataCountFunc)())
 */
void IoTLib_register_retrieve_all_stored_unsent_sensor_data_function(struct IoTLib_RawSensorDataAndSensorID* (*retrieveAllUnsentDataFunc)());

/** \brief Registers a function to return the total count of unsent data.
 * Needed to initialize internal arrays.
 *
 * @params getStoredUnsentDataCountFunc The function to get the total unsent data count.
 *
 * @see IoTLib_register_retrieve_last_upload_time_function(IoTLib_TIME_T (*retireveLastUploadTimeFunc)())
 * @see IoTLib_register_sensor_store_unsent_data_function(IoTLib_SensorID sensorID, void (*storeUnsentFunc)(void* rawSensorData))
 */
void IoTLib_register_get_stored_unsent_data_count_function(size_t (*getStoredUnsentDataCountFunc)());

/** \brief Register a function to get the current time.
 *
 * The returned time can be represented with any data type the user wants (see
 * IoTLib_TIME_T in user_settings). However, make sure that the units of time
 * used here and with sensor poll frequencies are the same (ie. milliseconds vs seconds).
 *
 * @params getCurrentTimeFunc The function to get the current time.
 *
 * @see IoTLib_register_convert_time_type_to_seconds_func(double (*convertTimeTypeToSecondsFunc)(IoTLib_TIME_T rawTime))
 * @see IoTLib_register_sensor_poll_frequency(IoTLib_SensorID sensorID, IoTLib_TIME_T sensorReadFrequency)
 */
void IoTLib_register_get_current_time_function(IoTLib_TIME_T (*getCurrentTimeFunc)());

/** \brief Register a function to convert the user specified time type to seconds.
 *
 * @params convertTimeTypeToSecondsFunc Function to convert the user specified
 * time type to seconds represented as a double.
 *
 * @see IoTLib_register_get_current_time_function(IoTLib_TIME_T (*getCurrentTimeFunc)())
 */
void IoTLib_register_convert_time_type_to_seconds_func(double (*convertTimeTypeToSecondsFunc)(IoTLib_TIME_T rawTime));

/** \brief Adds a task to the library's wait list.
 *
 * The wait list is a collection of tasks (function pointers) that return true
 * when the task is complete and false when the task needs more time. It's a good
 * idea to have the task eventually fail after a certain amount of time to prevent
 * the library from waiting forever for a task to complete.
 *
 * The purpose of a task is to provide a way for the program to wait until certain
 * "jobs" are completed.
 *
 * The actual code of the task is free to do any processing that is needed and
 * does not just only return if it's complete or not. For example, a
 * task may check if any of the pending uploaded data has been successfully
 * received and when it has been, remove it from the local offline storage.
 * Then when either all uploads have been successfully uploaded or they time out,
 * then the task is complete and returns true.
 *
 * Note: The waitlist functionality has had minimal testing.
 *
 * @param checkAndHandleTaskCompletionFunc The function that is the actual task.
 */
void IoTLib_add_task_to_waitlist(bool (*checkAndHandleTaskCompletionFunc)());

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
