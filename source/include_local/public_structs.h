/** \file public_structs.h
 *
 * Structs that are not directly part of library's the public interface but are still needed externally.
 */


#ifndef IoTLib_PUBLIC_STRUCTS_H
#define IoTLib_PUBLIC_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "general_macros.h"

/** \brief The type used to represent sensor IDs.
 *
 */
IoTLib_define_SensorID_with_data_struct(IoTLib_RawSensorDataAndSensorID, void*);

#ifdef __cplusplus
}
#endif

#endif
