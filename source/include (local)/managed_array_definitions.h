#ifndef IoTLib_MANGED_ARRAY_DEFINITIONS_H
#define IoTLib_MANGED_ARRAY_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

#include "typedefs.h"
#include "general_macros.h"
#include "managed_array_macros.h"
#include "managed_key_value_array_macros.h"

IoTLib_define_SensorID_with_data_struct(IoTLib_SnsrIDDataPtr, void*);

IoTLib_define_managed_array_and_declare_functions(IoTLib_MngdArray_SnsrID, IoTLib_SensorID);
IoTLib_define_managed_array_and_declare_functions(IoTLib_MngdArray_String, char*);

IoTLib_define_managed_key_value_array_and_declare_functions(IoTLib_MngdKVArray_SnsrIDString, IoTLib_SensorID, const char*);
IoTLib_define_managed_key_value_array_and_declare_functions(IoTLib_MngdKVArray_SnsrIDDataPtr, IoTLib_SensorID, void*);
IoTLib_define_managed_key_value_array_and_declare_functions(IoTLib_MngdKVArray_SnsrIDFloat, IoTLib_SensorID, float);
IoTLib_define_managed_key_value_array_and_declare_functions(IoTLib_MngdKVArray_SnsrIDInt, IoTLib_SensorID, int);

#ifdef __cplusplus
}
#endif

#endif
