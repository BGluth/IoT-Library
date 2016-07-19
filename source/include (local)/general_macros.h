#ifndef IoTLib_MACROS_H
#define IoTLib_MACROS_H

#include "macro_util.h"

// Fake methods that will be implemented later.
void IoTLib_Error(char* message) {};

/// Create an array of type inside of a struct with a length variable.
#define IoTLib_define_managed_array_for_type(managedArrayTypeName, dataType) \
struct managedArrayTypeName \
{ \
	dataType* array; \
	int length; \
	int capacity; \
}

#define IoTLib_initialize_managed_array(managedArrayType, managedArrayName, dataType, arrayCapacity) \
	dataType managedArrayName##_data_Array[arrayCapacity]; \
	managedArrayType managedArrayName = {.array = managedArrayName##_data_Array, .length = 0, .capacity = arrayCapacity}


// Conditional will not exist at run time.
#define IoTLib_array_add(managedArray, item) \
	IoTLib_perform_managed_array_bounds_check(&managedArray.length, &managedArray.capacity); \
	managedArray.array[managedArray.length] = item; \
	managedArray.length++


#endif
