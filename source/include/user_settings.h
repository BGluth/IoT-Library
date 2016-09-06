/**
 * @defgroup User_Settings
 * @{
 */

/** \file user_settings.h
 * \brief Library configuration file.
 *
 * Configuration for the library. You will most likely need to change multiple values here!
 */

#ifndef IoTLib_USER_SETTINGS_H
#define IoTLib_USER_SETTINGS_H

// Library Settings

#include <stdint.h>

/** \brief Enum used to set the verbosity of debugging.
 *
 * @see IoTLib_DEBUG_MESSAGE_LEVEL
 */
enum IoTLib_DebugLevel
{
	IoTLib_DEBGLVL_NONE = 0,
	IoTLib_DEBGLVL_ERROR = 1,
	IoTLib_DEBGLVL_ERROR_WARNING = 2,
	IoTLib_DEBGLVL_ERROR_WARNING_INFO = 3
};

/** \brief The number of sensors in use.
 *
 */
#define IoTLib_SENSOR_COUNT 1

/** \brief The number of sensors with a registered minimum operating temperature.
 *
 */
#define IoTLib_NUM_SENSORS_WITH_MIN_TEMP 1

/** \brief The number of sensors with a registered maximum operating temperature.
 *
 */
#define IoTLib_NUM_SENSORS_WITH_MAX_TEMP 1

/** \brief The number of sensors with a power-on function.
 *
 */
#define IoTLib_NUM_POWER_ON_FUNCTIONS 1

/** \brief CURRENTLY NOT USED
 *
 * SLATED FOR REMOVAL
 */
#define IoTLib_NUM_NON_SENSOR_INIT_FUNCTIONS 1

/** \brief The minimum number of seconds between upload function calls.
 *
 */
#define IoTLib_MIN_SECONDS_BETWEEN_UPLOADS 900

/** \brief Set whether you want to enable or disable checking the current temperature
 * and preventing sensors from running outside of their operating range.
 *
 * WARNING: Inactive mode **NOT** tested!
 *
 * Set to 1 to enable.
 * Set to 0 to disable.
 *
 * @see IoTLib_NUM_SENSORS_WITH_MIN_TEMP
 * @see IoTLib_NUM_SENSORS_WITH_MAX_TEMP
 */
#define IoTLib_USE_MIN_MAX_SENSOR_TEMPERATURES 1

/** \brief Configure the verbosity level of the library debugging messages.
 *
 */
#define IoTLib_DEBUG_MESSAGE_LEVEL IoTLib_DEBGLVL_ERROR_WARNING_INFO

/** \brief If a debug call is made with the level of error, then the program will
 * halt if this is enabled.
 *
 * @see IoTLib_DEBUG_MESSAGE_LEVEL
 */
#define IoTLib_DEBUG_HALT_ON_ERROR 1

/** \brief The datatype of how time is represented on the device.
 *
 * This will likely vary between different devices.
 */
#define IoTLib_TIME_T uint64_t

/** \brief Used to allow easy defining of macro symbols.
 *
 * Intended to alleviate having to go through setting up a build system to
 * define macro symbols during each compilation unit. Also, certain pre-made
 * build systems (like Particle's) may not allow you to define custom macro symbols
 * during the build process, which can make it difficult if not impossible to specify
 * custom macro symbols.
 *
 * Set to 0 if you want to provide the macro symbols by the build system.
 * Set to 1 to allow symbol definition from this file.
 */
#define IoTLib_DEFINE_LOCAL_MACRO_SYMBOLS_IN_USER_SETTINGS 1

#if IoTLib_DEFINE_LOCAL_MACRO_SYMBOLS_IN_USER_SETTINGS == 1
	/** \brief Compiles in some extra checks to catch errors that should not normally
	 * occur if the library is set up correctly (provided the library has no bugs,
	 * which it currently does!).
	 *
	 * Examples of extra checks include:
	 * - Array bounds checking
	 * - Checking if certain needed functions have not been registered
	 * - Checking if a key was not found in a key array when it was expected to be there
	 *
	 * The library will throw a error if any of the above issues occur.
	 *
	 * Despite sharing the same name, DEBUG is not needed to enable debug logging.
	 *
	 * @see IoTLib_DEBUG_MESSAGE_LEVEL
	 * @see IoTLib_DEBUG_HALT_ON_ERROR
	 */
	#define DEBUG
#endif

/**
 * @}
 */

// Helper macros (No touching!)
// **************************************

#define IoTLib_TOTAL_INIT_FUNCTIONS IoTLib_SENSOR_COUNT + IoTLib_NON_SENSOR_INIT_FUNCTIONS









// **************************************

#endif
