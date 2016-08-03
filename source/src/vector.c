/* I did not write this, besides some small style changes.
 * In an effort to save some precious time, I'm using
 * (maybe temporarily) EmilHernvall's simple Vector implementation.
 *
 * Link Here: https://gist.github.com/EmilHernvall/953968
 * As mentioned in the gist's comments, EmilHernvall licenses this under the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

void IoTLib_vector_init(struct IoTLib_Vector *v)
{
    v->data = NULL;
    v->size = 0;
    v->count = 0;
}

int IoTLib_vector_count(struct IoTLib_Vector *v)
{
    return v->count;
}

void IoTLib_vector_add(struct IoTLib_Vector *v, void *e)
{
    if (v->size == 0) {
        v->size = 10;
        v->data = (void**) malloc(sizeof(void*) * v->size);
        memset(v->data, '\0', sizeof(void*) * v->size);
    }

    if (v->size == v->count) {
        v->size *= 2;
        v->data = (void**) realloc(v->data, sizeof(void*) * v->size);
    }

    v->data[v->count] = e;
    v->count++;
}

void IoTLib_vector_set(struct IoTLib_Vector *v, size_t index, void *e)
{
    if (index >= v->count) {
        return;
    }

    v->data[index] = e;
}

void *IoTLib_vector_get(struct IoTLib_Vector *v, size_t index)
{
    if (index >= v->count) {
        return NULL;
    }

    return v->data[index];
}

void IoTLib_vector_delete(struct IoTLib_Vector *v, size_t index)
{
    if (index >= v->count) {
        return;
    }

    for (size_t i = index, j = index; i < v->count; i++) {
        v->data[j] = v->data[i];
        j++;
    }

    v->count--;
}

void IoTLib_vector_free(struct IoTLib_Vector *v)
{
    free(v->data);
}
