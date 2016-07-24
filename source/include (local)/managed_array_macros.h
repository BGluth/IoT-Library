#ifndef IoTLib_MANAGED_ARRAY_MACROS_H
#define IoTLib_MANAGED_ARRAY_MACROS_H

#include "macro_util.h"
#include <stddef.h>

#define _IoTLib_declare_managed_array_add_function(managedArrayTypeName, dataType) \
	void managedArrayTypeName##_add(struct managedArrayTypeName* managedArray, dataType item)

#define _IoTLib_define_managed_array_add_function(managedArrayTypeName, dataType) \
	void managedArrayTypeName##_add(struct managedArrayTypeName* managedArray, dataType item) \
	{ \
		IoTLib_perform_managed_array_bounds_check(managedArray->length, managedArray->capacity); \
		managedArray->array[managedArray->length] = item; \
		managedArray->length++; \
	}


#define _IoTLib_declare_managed_array_functions(managedArrayTypeName, dataType) \
	_IoTLib_declare_managed_array_add_function(managedArrayTypeName, dataType)

#define IoTLib_define_managed_array_functions(managedArrayTypeName, dataType) \
	_IoTLib_define_managed_array_add_function(managedArrayTypeName, dataType)


#define IoTLib_define_managed_array_and_declare_functions(managedArrayTypeName, dataType) \
struct managedArrayTypeName \
{ \
	dataType* array; \
	size_t length; \
	size_t capacity; \
}; \
_IoTLib_declare_managed_array_functions(managedArrayTypeName, dataType)


#define IoTLib_MA_add(managedArrayRef, item, managedArrayTypeName) \
	managedArrayTypeName##_add(managedArrayRef, item)


#define IoTLib_initialize_managed_array(managedArrayName, managedArrayType, dataType, arrayCapacity) \
	dataType managedArrayName##_data_Array[arrayCapacity]; \
	managedArrayType managedArrayName = {.array = managedArrayName##_data_Array, .length = 0, .capacity = arrayCapacity}

#endif
