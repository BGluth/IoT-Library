#define _IoTLib_define_managed_key_value_array_insert(managedKeyValueArrayTypeName, keyType, valueType) \
	void managedKeyValueArrayTypeName##_insert(struct managedKeyValueArrayTypeName* keyValueArray, keyType key, valueType value) \
	{ \
		IoTLib_perform_managed_array_bounds_check(&keyValueArray->length, &keyValueArray->capacity); \
		keyValueArray->keys[keyValueArray->length] = key; \
		keyValueArray->values[keyValueArray->length] = value; \
		keyValueArray->length++; \
	}

#define _IoTLib_define_managed_key_value_array_functions(managedKeyValueArrayType, keyType, valueType) \
	_IoTLib_define_managed_key_value_array_insert(managedKeyValueArrayType, keyType, valueType);

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

#define IoTLib_initialize_managed_key_value_array(managedKeyValueArrayName, managedKeyValueArrayType, keyType, dataType, arraySize) \
	keyType managedKeyValueArrayName##_keys[arraySize]; \
	dataType managedKeyValueArrayName##_values[arraySize]; \
	managedKeyValueArrayType managedKeyValueArrayName = {.keys = managedKeyValueArrayName##_keys, .values = managedKeyValueArrayName##_values, .length = 0, .capacity = arraySize};
