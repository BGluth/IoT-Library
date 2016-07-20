#ifndef IoTLib_MACROS_H
#define IoTLib_MACROS_H

#include "macro_util.h"

// Fake methods that will be implemented later.
void IoTLib_Error(char* message) {};

#define _IoTLib_define_managed_array_add_function(managedArrayTypeName, dataType) \
	void managedArrayTypeName##_add(struct managedArrayTypeName* managedArray, dataType item) \
	{ \
		IoTLib_perform_managed_array_bounds_check(managedArray->length, managedArray->capacity); \
		managedArray->array[managedArray->length] = item; \
		managedArray->length++; \
	}

#define _IoTLib_define_managed_array_functions(managedArrayTypeName, dataType) \
	_IoTLib_define_managed_array_add_function(managedArrayTypeName, dataType)

/// Create an array of type inside of a struct with a length variable.
#define IoTLib_define_managed_array_for_type(managedArrayTypeName, dataType) \
struct managedArrayTypeName \
{ \
	dataType* array; \
	int length; \
	int capacity; \
}; \
_IoTLib_define_managed_array_functions(managedArrayTypeName, dataType)

#define IoTLib_initialize_managed_array(managedArrayName, managedArrayType, dataType, arrayCapacity) \
	dataType managedArrayName##_data_Array[arrayCapacity]; \
	managedArrayType managedArrayName = {.array = managedArrayName##_data_Array, .length = 0, .capacity = arrayCapacity}


// Conditional will not exist at run time.
#define IoTLib_MA_add(managedArrayRef, item, managedArrayTypeName) \
	managedArrayTypeName##_add(managedArrayRef, item)


#endif
