/* I did not write this, besides some small style changes.
 * In an effort to save some precious time, I'm using
 * (maybe temporarily) EmilHernvall's simple vector implementation.
 *
 * Link Here: https://gist.github.com/EmilHernvall/953968
 * As mentioned in the gist's comments, EmilHernvall licenses this under the MIT license.
 */

#ifndef IoTLib_VECTOR_H_
#define IoTLib_VECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

struct IoTLib_Vector
{
    void** data;
    size_t size;
    size_t count;
};

void IoTLib_vector_init(struct IoTLib_Vector*);
int IoTLib_vector_count(struct IoTLib_Vector*);
void IoTLib_vector_add(struct IoTLib_Vector*, void*);
void IoTLib_vector_set(struct IoTLib_Vector*, size_t, void*);
void* IoTLib_vector_get(struct IoTLib_Vector*, size_t);
void IoTLib_vector_delete(struct IoTLib_Vector*, size_t);
void IoTLib_vector_free(struct IoTLib_Vector*);

#ifdef __cplusplus
}
#endif

#endif
