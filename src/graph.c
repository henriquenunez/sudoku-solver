#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

#define ABS(a) ((a > 0) ? a : a * -1)

/*
Vector for storing data with known size
*/
typedef struct _vector
{
    int size;
    vertex_n* data;
    int capacity; //actual size of alloc'd values
} VECTOR;

VECTOR __realloc_vector(VECTOR* this_vector)
{
    VECTOR ret_vec;

    vertex_n* temp_vertexes
            = (vertex_n*) malloc(this_vector->size * sizeof(vertex_n));

    //Copies elements which are not -1 into 'temp_vec'
    int j = 0;
    for(int i = 0 ; i < this_vector->size ; i++)
    {
    if (this_vector->data[i] == -1) continue;
    temp_vertexes[j++] = this_vector->data[i];
    }

    //Copies data into new vector
    ret_vec.data = (vertex_n*) malloc(j * sizeof(vertex_n));
    ret_vec.size = j;
    memcpy(ret_vec.data, temp_vertexes, j *sizeof(vertex_n));

    free(this_vector);
    free(temp_vertexes);

    return ret_vec;
}

vertex_n* __search_in_vector(VECTOR* this_vector, int number)
{
    //printf("looking for %d\n", number);
    for(int i = 0 ; i < this_vector->size ; i++)
    {
        if ( *((vertex_n*)(this_vector->data+i)) == number)
        return ((vertex_n*)this_vector->data) + i;
    }
    return NULL;
}

/*
Pair, works pretty much like a tuple
*/
typedef struct _pair
{
    int first;
    int second;
} PAIR;

/*
This refers to a dynamically allocated array
that points to vectors.
*/
typedef struct _list
{
    //Size refers to |V| (vertex number)
    int size;
    //Array to vector of edges
    VECTOR* vertexes;
} LIST;

typedef struct _matrix
{
    //Size refers to |V| (vertex number)
    int size;
    int **content;
} MATRIX;

struct _graph
{
    graph_type _type;
    union
    {
	LIST adj_list;
	MATRIX adj_matrix;
    };
    color_t* color_vtx_list;
    int size; //Small technical adjustment here. TODO redo implementation;
};

/*ADJACENCE LIST*/
LIST new_adj_list(int size)
{
    LIST ret_list;
    ret_list.size = size;

    //Array of vectors that correspond to a vertex adj. list
    ret_list.vertexes = (VECTOR*) calloc (size, sizeof(VECTOR));

    return ret_list;
}

void delete_adj_list(LIST *this_list)
{
    for (int i = 0 ; i < this_list->size ; i++) {
    //accesses i-th position of vectors array and frees data
    free(this_list->vertexes[i].data);
    }
    free(this_list->vertexes);
}

void __insert_edge_adj_list(LIST* this_list, vertex_n src, vertex_n dest)
{
    //realloc data, and append desired
    /*
    This functions assumes we are inserting vertex_n types into the vec.
    */
    VECTOR* this_vertex;

    //moves 'this_vertex' into right position
    this_vertex = this_list->vertexes + src;

    if (__search_in_vector(this_vertex, dest) != NULL) return;

    this_vertex->size++;
    if (this_vertex->capacity == 0)
    {
    this_vertex->data =
        (vertex_n*) malloc(sizeof(vertex_n) * (int)(this_list->size/2));
    this_vertex->capacity = (int)(this_list->size/2);
    }
    else if (this_vertex->size >= this_vertex->capacity)
    {
    this_vertex->data = realloc(this_vertex->data,
                    sizeof(vertex_n) * this_vertex->size);
    this_vertex->capacity = this_vertex->size;
    }
    if(this_vertex->data == NULL) exit(-1); //PANIC

    this_vertex->data[this_vertex->size-1] = dest;
}

void __remove_edge_adj_list(LIST* this_list, vertex_n src, vertex_n dest)
{
    /*
    This funtion removes and concatenates data
    */
    VECTOR* this_vertex;

    //moves 'this_vertex' into right position
    this_vertex = this_list->vertexes + src;

    //Position of removed vertex in vector
    vertex_n* dest_position = __search_in_vector(this_vertex, dest);
    if (dest_position == NULL) return;

    //this_vertex->size--;
    *dest_position = -1;
}

void __print_adj_list(LIST* this_list)
{
    VECTOR* this_vertex;

    for (int i  = 0 ; i < this_list->size ; i++)
    {
    this_vertex = this_list->vertexes + i;
    printf("%d:", i);
    for (int j = 0 ; j < this_vertex->size ; j++)
    {
        if (this_vertex->data[j] < 0) continue;
        printf(" %d", this_vertex->data[j]);
    }
    printf("\n");
    }
    printf("\n");
}

/*ADJACENCE MATRIX*/
MATRIX new_adj_matrix(int size)
{
    MATRIX ret_matrix;

    int **matrix_ptr;

    matrix_ptr = (int**) malloc (size * sizeof(int*));
    if (matrix_ptr == NULL) exit(1); //deu caca

    for (int i = 0 ; i < size ; i++)
    {
    //already inserts zeroes into matrix
    matrix_ptr[i] = (int*) calloc (size, sizeof(int));
    if (matrix_ptr[i] == NULL) exit(1); //deu caca
    }

    ret_matrix.size = size;
    ret_matrix.content = matrix_ptr;

    return ret_matrix;
}

void delete_adj_matrix(MATRIX *this_matrix)
{
    for (int i = 0 ; i < this_matrix->size ; i++)
    {
    free(this_matrix->content[i]);
    }

    free(this_matrix->content);

    this_matrix->size = 0;
    this_matrix->content = NULL;
}

/* MISCELLANEOUS */

VECTOR __get_adjacent_vtxs_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR ret_vec;

    if(a_vertex > this_graph->size) exit(1); //Dont pass weird stuff.

    //We will be only using adjacence list on this project.
    if (this_graph->_type & ADJ_LIST)
    {
	ret_vec = this_graph->adj_list.vertexes[a_vertex];
    }
    else if(this_graph->_type & ADJ_MATRIX)
	;

    return ret_vec;
}

/*EXPOSED FUNCTIONS*/

GRAPH* new_graph(graph_type _type, int vertex_num)
{
    GRAPH* ret_ref;

    ret_ref = (GRAPH*) malloc (sizeof(GRAPH));
    ret_ref->_type = _type;

    if (_type & ADJ_MATRIX)
    {
    ret_ref->adj_matrix = new_adj_matrix(vertex_num);
    }
    else
    {
    ret_ref->adj_list = new_adj_list(vertex_num);
    }

    ret_ref->color_vtx_list = (color_t*) calloc(vertex_num, sizeof(color_t));
    ret_ref->size = vertex_num;

    return ret_ref;
}

void delete_graph(GRAPH* this_graph)
{
    if (this_graph->_type & ADJ_MATRIX)
    {
    //takes the address of the matrix in the graph
    delete_adj_matrix(&(this_graph->adj_matrix));
    }
    else
    {
    delete_adj_list(&this_graph->adj_list);
    }
    //this_graph->size = -1;
    free(this_graph->color_vtx_list);
    free(this_graph);
}

graph_err_t insert_edge_graph(GRAPH* this_graph, vertex_n src, vertex_n dest)
{
    if (this_graph->_type & ADJ_MATRIX)
    {
    //if is not oriented, fill the opposite way
    if ((this_graph->_type & ORIENTED) == 0)
        this_graph->adj_matrix.content[dest][src] = 1;
    this_graph->adj_matrix.content[src][dest] = 1;
    }
    else if (this_graph->_type & ADJ_LIST)
    {
    //if is not oriented, fill the opposite way
    if ((this_graph->_type & ORIENTED) == 0)
        __insert_edge_adj_list(&this_graph->adj_list, dest, src);
    __insert_edge_adj_list(&this_graph->adj_list, src, dest);
    }

    return GR_OK;
}

graph_err_t remove_edge_graph(GRAPH* this_graph, vertex_n src, vertex_n dest)
{
    if (this_graph->_type & ADJ_MATRIX)
    {
    //if is not oriented, remove the opposite way
    if ((this_graph->_type & ORIENTED) == 0)
        this_graph->adj_matrix.content[dest][src] = 0;
    this_graph->adj_matrix.content[src][dest] = 0;
    }
    else if (this_graph->_type & ADJ_LIST)
    {
    if ((this_graph->_type & ORIENTED) == 0)
        __remove_edge_adj_list(&this_graph->adj_list, src, dest);
    __remove_edge_adj_list(&this_graph->adj_list, dest, src);
    }

    return GR_OK;
}

void print_graph(GRAPH* this_graph)
{

    if (this_graph->_type & ADJ_MATRIX)
    {
    #define _size (this_graph->adj_matrix.size)
    for (int i = 0 ; i < _size ; i++)
    {
        for (int j = 0 ; j < _size ; j++)
        {
        printf("%d ", this_graph->adj_matrix.content[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    #undef _size
    }
    else if (this_graph->_type & ADJ_LIST)
    {
    __print_adj_list(&this_graph->adj_list);
    }
}

#include "color.c"

#undef ABS

