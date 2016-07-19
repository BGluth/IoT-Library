#define _IoTLib_define_managed_key_value_array_insert_function(managedKeyValueArrayTypeName, keyType, valueType) \
	void managedKeyValueArrayTypeName##_insert(struct managedKeyValueArrayTypeName* keyValueArray, keyType key, valueType value) \
	{ \
		IoTLib_perform_managed_array_bounds_check(&keyValueArray->length, &keyValueArray->capacity); \
		keyValueArray->keys[keyValueArray->length] = key; \
		keyValueArray->values[keyValueArray->length] = value; \
		keyValueArray->length++; \
	}

#define _IoTLib_define_managed_key_value_array_get_function(managedKeyValueArrayTypeName, keyType, valueType) \
	valueType managedKeyValueArrayTypeName##_get(struct managedKeyValueArrayTypeName* keyValueArray, keyType key) \
	{ \
		int index = keyType##_get_key_index(keyValueArray->keys, keyValueArray->length, key); \
		return keyValueArray->values[index]; \
	}

#define _IoTLib_define_managed_key_value_array_functions(managedKeyValueArrayType, keyType, valueType) \
	_IoTLib_define_managed_key_value_array_insert_function(managedKeyValueArrayType, keyType, valueType); \
	_IoTLib_define_managed_key_value_array_get_function(managedKeyValueArrayType, keyType, valueType);

#define IoTLib_define_functions_for_key_type(keyType, getKeyIndexFunc) \
	int keyType##_get_key_index(const keyType* keyArray, const int arrayLength, const keyType keyToSearchFor) \
	{ \
		return getKeyIndexFunc(keyArray, arrayLength, keyToSearchFor); \
	}

#define IoTLib_define_managed_key_value_array(managedKeyValueArrayName, keyType, valueType) \
	struct managedKeyValueArrayName \
	{ \
		keyType* keys; \
		valueType* values; \
		int length; \
		int capacity; \
	}; \
	_IoTLib_define_managed_key_value_array_functions(managedKeyValueArrayName, keyType, valueType);

#define IoTLib_managed_key_value_array_insert(managedKeyValueArray, managedKeyValueArrayTypeName, key, value) \
	managedKeyValueArrayTypeName##_insert(&managedKeyValueArray, key, value);

#define IoTLib_managed_key_value_array_get(managedKeyValueArray, managedKeyValueArrayTypeName, key) \
	managedKeyValueArrayTypeName##_get(&managedKeyValueArray, key);

#define IoTLib_initialize_managed_key_value_array(managedKeyValueArrayName, managedKeyValueArrayType, keyType, dataType, arraySize) \
	keyType managedKeyValueArrayName##_keys[arraySize]; \
	dataType managedKeyValueArrayName##_values[arraySize]; \
	managedKeyValueArrayType managedKeyValueArrayName = {.keys = managedKeyValueArrayName##_keys, .values = managedKeyValueArrayName##_values, .length = 0, .capacity = arraySize};
