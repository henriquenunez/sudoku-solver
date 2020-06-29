#ifndef DSTRUCTURES_H
#define DSTRUCTURES_H

/*
Vector for storing data with known size
*/
typedef struct _vector
{
    int size;
    vertex_n* data;
    int capacity; //actual size of alloc'd values
} VECTOR;

#endif

