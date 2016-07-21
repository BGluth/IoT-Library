#ifndef IoTLib_MACROS_H
#define IoTLib_MACROS_H

#define IoTLib_define_SensorID_with_data_struct(structName, dataType) \
	struct structName \
	{ \
		IoTLib_SensorID id; \
		dataType data; \
	}

#endif
