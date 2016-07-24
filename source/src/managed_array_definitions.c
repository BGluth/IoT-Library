#include "managed_array_definitions.h"

#include <time.h>
#include "sensorID.h"

IoTLib_define_functions_for_key_type(IoTLib_SensorID, IoTLib_sensorid_get_key_index);

IoTLib_define_managed_array_functions(IoTLib_MngdArray_SnsrID, IoTLib_SensorID);
IoTLib_define_managed_array_functions(IoTLib_MngdArray_String, char*);

IoTLib_define_managed_key_value_array_functions(IoTLib_MngdKVArray_SnsrIDString, IoTLib_SensorID, char*);
IoTLib_define_managed_key_value_array_functions(IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_SensorID, void*);
IoTLib_define_managed_key_value_array_functions(IoTLib_MngdKVArray_SnsrIDFloat, IoTLib_SensorID, float);
IoTLib_define_managed_key_value_array_functions(IoTLib_MngdKVArray_SnsrIDInt, IoTLib_SensorID, int);
