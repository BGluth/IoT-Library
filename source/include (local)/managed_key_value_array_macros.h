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
		IoTLib_managed_key_value_array_getFunc_validate_key_index(index); \
		return keyValueArray->values[index]; \
	}

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

#define _IoTLib_define_managed_key_value_array_contains_key_function(managedKeyValueArrayTypeName, keyType) \
	bool managedKeyValueArrayTypeName##_containsKey(struct managedKeyValueArrayTypeName* keyValueArray, keyType key) \
	{ \
		int index = keyType##_get_key_index(keyValueArray->keys, keyValueArray->length, key); \
		return IoTLib_managed_key_value_array_index_is_valid_index(index); \
	}


#define _IoTLib_define_managed_key_value_array_functions(managedKeyValueArrayType, keyType, valueType) \
	_IoTLib_define_managed_key_value_array_insert_function(managedKeyValueArrayType, keyType, valueType); \
	_IoTLib_define_managed_key_value_array_get_function(managedKeyValueArrayType, keyType, valueType); \
	_IoTLib_define_managed_key_value_array_try_get_function(managedKeyValueArrayType, keyType, valueType); \
	_IoTLib_define_managed_key_value_array_contains_key_function(managedKeyValueArrayType, keyType)

#define IoTLib_define_functions_for_key_type(keyType, getKeyIndexFunc) \
	int keyType##_get_key_index(const keyType* keyArray, const int arrayLength, const keyType keyToSearchFor) \
	{ \
		return getKeyIndexFunc(keyArray, arrayLength, keyToSearchFor); \
	}

#define IoTLib_define_managed_key_value_array(managedKeyValueArrayTypeName, keyType, valueType) \
	struct managedKeyValueArrayTypeName \
	{ \
		keyType* keys; \
		valueType* values; \
		int length; \
		int capacity; \
	}; \
	_IoTLib_define_managed_key_value_array_functions(managedKeyValueArrayTypeName, keyType, valueType)


#define IoTLib_MKV_insert(managedKeyValueArray, managedKeyValueArrayTypeName, key, value) \
	managedKeyValueArrayTypeName##_insert(&managedKeyValueArray, key, value)

#define IoTLib_MKV_get(managedKeyValueArray, managedKeyValueArrayTypeName, key) \
	managedKeyValueArrayTypeName##_get(&managedKeyValueArray, key)

#define IoTLib_MKV_try_get(managedKeyValueArray, managedKeyValueArrayTypeName, key, valueOut) \
	managedKeyValueArrayTypeName##_tryGet(&managedKeyValueArray, key, valueOut)

#define IoTLib_MKV_contains_key(managedKeyValueArray, managedKeyValueArrayTypeName, key) \
	managedKeyValueArrayTypeName##_containsKey(&managedKeyValueArray, key)


#define IoTLib_initialize_managed_key_value_array(managedKeyValueArrayName, managedKeyValueArrayType, keyType, dataType, arrayCapacity) \
	keyType managedKeyValueArrayName##_keys[arrayCapacity]; \
	dataType managedKeyValueArrayName##_values[arrayCapacity]; \
	managedKeyValueArrayType managedKeyValueArrayName = {.keys = managedKeyValueArrayName##_keys, .values = managedKeyValueArrayName##_values, .length = 0, .capacity = arrayCapacity}
