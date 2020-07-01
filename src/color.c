//Coloring

#include "graph.c"

int get_color_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    return this_graph->color_vtx_welsh_list[a_vertex];
}

graph_err_t put_color_at_vtx_graph(GRAPH* this_graph,
				    vertex_n a_vertex,
				    int color)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;

    if(get_color_at_vtx_graph(this_graph, a_vertex) != 0)
    {
	return GR_VTX_COLORED;
    }

    //Check if can be colored by adjacencies.
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];
	if(get_color_at_vtx_graph(this_graph, curr_vertex) == color)
	    return GR_COLOR_INVALID;
    }

    this_graph->color_vtx_welsh_list[a_vertex] = color;
    this_graph->colored_vtxs++;

    return GR_OK;
}

int count_adj_colored(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;
    color_t a_vertex_color;
    int count = 0;

    if((a_vertex_color = get_color_at_vtx_graph(this_graph, a_vertex)) == 0)
	return GR_VTX_NOT_COLORED;

    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];
	printf("Curr vertex: %d", curr_vertex);
	//If adj is colored, increase count.
	if(get_color_at_vtx_graph(this_graph, curr_vertex) != 0) count++;
    }

    return count;
}

graph_err_t color_solver(GRAPH* this_graph)
{
    while(this_graph->colored_vtxs < this_graph->size)
    {
	int color_number = 1;
	int saturation_dgree = -1, tempdgree; //The lowest, the most saturated.
	int index = -1;

	for(int i = 0 ; i < this_graph->size ; i++)
	{
	    if(get_color_at_vtx_graph(this_graph, i) == 0) //Not colored.
	    {
		tempdgree =
		    count_adj_colored(this_graph, i);

		if(tempdgree > saturation_dgree)
		{
		    saturation_dgree = tempdgree;
		    index = i;
		}
		else if(tempdgree == saturation_dgree)
		{
		    index = i;
		}
	    }
	}

	while(put_color_at_vtx_graph(this_graph, index, color_number) != GR_OK)
	{
	    color_number++;
	}
    }

    return GR_OK;
}

graph_err_t brute_force_solver(GRAPH* this_graph)
{
    return GR_OK;
}

