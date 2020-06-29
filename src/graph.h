#ifndef GRAPH_H
#define GRAPH_H

typedef struct _graph GRAPH;
typedef unsigned int vertex_n;
typedef int color_t;
typedef enum
{
    GR_OK,
    GR_VTX_OUT_BOUNDS,
    GR_VTX_COLORED,
    GR_VTX_NOT_COLORED,
    GR_COLOR_INVALID
} graph_err_t;

typedef unsigned char graph_type;

#define ADJ_MATRIX 0b000000001
#define ADJ_LIST   0b000000010
#define ORIENTED   0b000000100
#define COLOR_NUMBER 9 //We only work with 9 colors for now.

GRAPH* new_graph(graph_type, int);
graph_err_t insert_edge_graph(GRAPH*, vertex_n, vertex_n);
graph_err_t remove_edge_graph(GRAPH*, vertex_n, vertex_n);
void print_graph(GRAPH*);
void delete_graph(GRAPH*);

//Graph colouring.
graph_err_t put_color_at_vtx_graph(GRAPH*, vertex_n, int color);
int get_color_at_vtx_graph(GRAPH*, vertex_n); //returns -1 in the undefined case.
graph_err_t auto_color_graph(GRAPH*);

#endif

