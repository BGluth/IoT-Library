#include <stdbool.h>

#define _IoTLib_declare_managed_key_value_array_insert_function(managedKeyValueArrayTypeName, keyType, valueType) \
	void managedKeyValueArrayTypeName##_insert(struct managedKeyValueArrayTypeName* keyValueArray, keyType key, valueType value)

#define _IoTLib_define_managed_key_value_array_insert_function(managedKeyValueArrayTypeName, keyType, valueType) \
	void managedKeyValueArrayTypeName##_insert(struct managedKeyValueArrayTypeName* keyValueArray, keyType key, valueType value) \
	{ \
		IoTLib_perform_managed_array_bounds_check(keyValueArray->length, keyValueArray->capacity); \
		keyValueArray->keys[keyValueArray->length] = key; \
		keyValueArray->values[keyValueArray->length] = value; \
		keyValueArray->length++; \
	}

#define _IoTLib_declare_managed_key_value_array_get_function(managedKeyValueArrayTypeName, keyType, valueType) \
	valueType managedKeyValueArrayTypeName##_get(struct managedKeyValueArrayTypeName* keyValueArray, keyType key)

#define _IoTLib_define_managed_key_value_array_get_function(managedKeyValueArrayTypeName, keyType, valueType) \
	valueType managedKeyValueArrayTypeName##_get(struct managedKeyValueArrayTypeName* keyValueArray, keyType key) \
	{ \
		int index = keyType##_get_key_index(keyValueArray->keys, keyValueArray->length, key); \
		IoTLib_managed_key_value_array_getFunc_validate_key_index(index); \
		return keyValueArray->values[index]; \
	}

#define _IoTLib_declare_managed_key_value_array_try_get_function(managedKeyValueArrayTypeName, keyType, valueType) \
	bool managedKeyValueArrayTypeName##_tryGet(struct managedKeyValueArrayTypeName* keyValueArray, keyType key, valueType* valueTypeOut)

#define _IoTLib_define_managed_key_value_array_try_get_function(managedKeyValueArrayTypeName, keyType, valueType) \
	bool managedKeyValueArrayTypeName##_tryGet(struct managedKeyValueArrayTypeName* keyValueArray, keyType key, valueType* valueTypeOut) \
	{ \
		int index = keyType##_get_key_index(keyValueArray->keys, keyValueArray->length, key); \
		if (IoTLib_managed_key_value_array_index_is_valid_index(index)) \
		{ \
			*valueTypeOut = keyValueArray->values[index]; \
			return true; \
		} \
		return false; \
	}

#define _IoTLib_declare_managed_key_value_array_contains_key_function(managedKeyValueArrayTypeName, keyType) \
	bool managedKeyValueArrayTypeName##_containsKey(struct managedKeyValueArrayTypeName* keyValueArray, keyType key)

#define _IoTLib_define_managed_key_value_array_contains_key_function(managedKeyValueArrayTypeName, keyType) \
	bool managedKeyValueArrayTypeName##_containsKey(struct managedKeyValueArrayTypeName* keyValueArray, keyType key) \
	{ \
		int index = keyType##_get_key_index(keyValueArray->keys, keyValueArray->length, key); \
		return IoTLib_managed_key_value_array_index_is_valid_index(index); \
	}


#define _IoTLib_declare_managed_key_value_array_functions(managedKeyValueArrayTypeName, keyType, valueType) \
	_IoTLib_declare_managed_key_value_array_insert_function(managedKeyValueArrayTypeName, keyType, valueType); \
	_IoTLib_declare_managed_key_value_array_get_function(managedKeyValueArrayTypeName, keyType, valueType); \
	_IoTLib_declare_managed_key_value_array_try_get_function(managedKeyValueArrayTypeName, keyType, valueType); \
	_IoTLib_declare_managed_key_value_array_contains_key_function(managedKeyValueArrayTypeName, keyType)

#define IoTLib_define_managed_key_value_array_functions(managedKeyValueArrayTypeName, keyType, valueType) \
	_IoTLib_define_managed_key_value_array_insert_function(managedKeyValueArrayTypeName, keyType, valueType); \
	_IoTLib_define_managed_key_value_array_get_function(managedKeyValueArrayTypeName, keyType, valueType); \
	_IoTLib_define_managed_key_value_array_try_get_function(managedKeyValueArrayTypeName, keyType, valueType); \
	_IoTLib_define_managed_key_value_array_contains_key_function(managedKeyValueArrayTypeName, keyType)


#define IoTLib_define_functions_for_key_type(keyType, getKeyIndexFunc) \
	int keyType##_get_key_index(const keyType* keyArray, const int arrayLength, const keyType keyToSearchFor) \
	{ \
		return getKeyIndexFunc(keyArray, arrayLength, keyToSearchFor); \
	}

#define IoTLib_define_managed_key_value_array_and_declare_functions(managedKeyValueArrayTypeName, keyType, valueType) \
	struct managedKeyValueArrayTypeName \
	{ \
		keyType* keys; \
		valueType* values; \
		int length; \
		int capacity; \
	}; \
	_IoTLib_declare_managed_key_value_array_functions(managedKeyValueArrayTypeName, keyType, valueType)


#define IoTLib_MKV_insert(managedKeyValueArrayRef, managedKeyValueArrayTypeName, key, value) \
	managedKeyValueArrayTypeName##_insert(managedKeyValueArrayRef, key, value)

#define IoTLib_MKV_get(managedKeyValueArrayRef, managedKeyValueArrayTypeName, key) \
	managedKeyValueArrayTypeName##_get(managedKeyValueArrayRef, key)

#define IoTLib_MKV_try_get(managedKeyValueArrayRef, managedKeyValueArrayTypeName, key, valueOut) \
	managedKeyValueArrayTypeName##_tryGet(managedKeyValueArrayRef, key, valueOut)

#define IoTLib_MKV_contains_key(managedKeyValueArrayRef, managedKeyValueArrayTypeName, key) \
	managedKeyValueArrayTypeName##_containsKey(managedKeyValueArrayRef, key)


#define IoTLib_initialize_managed_key_value_array(managedKeyValueArrayName, managedKeyValueArrayType, keyType, dataType, arrayCapacity) \
	keyType managedKeyValueArrayName##_keys[arrayCapacity]; \
	dataType managedKeyValueArrayName##_values[arrayCapacity]; \
	managedKeyValueArrayType managedKeyValueArrayName = {.keys = managedKeyValueArrayName##_keys, .values = managedKeyValueArrayName##_values, .length = 0, .capacity = arrayCapacity}
