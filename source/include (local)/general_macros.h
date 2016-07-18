#ifndef IoTLib_MACROS_H
#define IoTLib_MACROS_H

#include "macro_util.h"

// Fake methods that will be implemented later.
void IoTLib_Error(char* message) {};

/// Create an array of type inside of a struct with a length variable.
#define IoTLib_define_managed_array_for_type(type_name, type) \
struct type_name \
{ \
	type* array; \
	int length; \
	int capacity; \
};

#define IoTLib_initialize_managed_array(managedArrayType, managedArrayName, dataType, arraySize) \
	dataType managedArrayName##_data_Array[arraySize]; \
	managedArrayType managedArrayName = {.array = managedArrayName##_data_Array, .length = 0, .capacity = arraySize};


// Conditional will not exist at run time.
#define IoTLib_array_add(managedArray, item) \
	IoTLib_perform_managed_array_bounds_check(&managedArray.length, &managedArray.capacity); \
	managedArray.array[managedArray.length] = item; \
	managedArray.length++;


#endif
