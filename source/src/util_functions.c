#include "util_functions.h"

#include <stddef.h>
#include <time.h>

time_t _IoTLib_get_current_time()
{
	return time(NULL);
}
