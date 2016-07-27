#ifndef IoTLib_MACROS_H
#define IoTLib_MACROS_H

#include <stdarg.h>

#define IoTLib_define_SensorID_with_data_struct(structName, dataType) \
	struct structName \
	{ \
		IoTLib_SensorID id; \
		dataType data; \
	}

#define IoTLib_wrap_in_va_start_and_end(lastParameterBeforeElipse, code) \
	va_list extraArgs; \
	va_start(extraArgs, message); \
	code; \
	va_end(extraArgs)

#endif
