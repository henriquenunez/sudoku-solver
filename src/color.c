/*
Graph coloring data structures and algorithms
*/


//Bit masks for defining colors.
#define COLOR_1 0b0000000000000001
#define COLOR_2 0b0000000000000010
#define COLOR_3 0b0000000000000100
#define COLOR_4 0b0000000000001000
#define COLOR_5 0b0000000000010000
#define COLOR_6 0b0000000000100000
#define COLOR_7 0b0000000001000000
#define COLOR_8 0b0000000010000000
#define COLOR_9 0b0000000100000000

#define IF_ONLY_IF(A, B) ( (A & B) && !(A & (~B)))

//If more than one color matches, will return -1.
int get_color_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    color_t a_vertex_color = this_graph->color_vtx_list[a_vertex];
    color_t temp;

    if( IF_ONLY_IF(a_vertex_color, COLOR_1) ) return 1;
    if( IF_ONLY_IF(a_vertex_color, COLOR_2) ) return 2;
    if( IF_ONLY_IF(a_vertex_color, COLOR_3) ) return 3;
    if( IF_ONLY_IF(a_vertex_color, COLOR_4) ) return 4;
    if( IF_ONLY_IF(a_vertex_color, COLOR_5) ) return 5;
    if( IF_ONLY_IF(a_vertex_color, COLOR_6) ) return 6;
    if( IF_ONLY_IF(a_vertex_color, COLOR_7) ) return 7;
    if( IF_ONLY_IF(a_vertex_color, COLOR_8) ) return 8;
    if( IF_ONLY_IF(a_vertex_color, COLOR_9) ) return 9;

    return -1;
}

graph_err_t update_color_list_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;

    if(get_color_at_vtx_graph(this_graph, a_vertex) == -1) return GR_VTX_NOT_COLORED;

    //Gets color, should be one of the previously defined macros, or 0x00
    color_t a_vertex_color = this_graph->color_vtx_list[a_vertex];

    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];

	printf("Vertex %d colors: %x\n", curr_vertex,
		this_graph->color_vtx_list[curr_vertex]);
	printf("Removing color %x at vertex %d\n", a_vertex_color, curr_vertex);

	this_graph->color_vtx_list[curr_vertex] =
		    this_graph->color_vtx_list[curr_vertex] & (~a_vertex_color);
	printf("after >> Vertex %d colors: %x\n", curr_vertex,
		this_graph->color_vtx_list[curr_vertex]);

    }

    return GR_OK;
}

graph_err_t put_color_at_vtx_graph(GRAPH* this_graph,
				    vertex_n a_vertex,
				    int some_color)
{
    if (some_color < 1 || some_color > COLOR_NUMBER) return GR_COLOR_INVALID;
    if (get_color_at_vtx_graph(this_graph, a_vertex) != -1) return GR_VTX_COLORED;
    color_t temp_color = 0x0000;

    /*printf("Colors at vertex %d : %x\n", a_vertex,
	this_graph->color_vtx_list[a_vertex]);*/

    switch(some_color)
    {
	case 1:
	    temp_color = COLOR_1;
	    break;

	case 2:
	    temp_color = COLOR_2;
	    break;

	case 3:
	    temp_color = COLOR_3;
	    break;

	case 4:
	    temp_color = COLOR_4;
	    break;

	case 5:
	    temp_color = COLOR_5;
	    break;

	case 6:
	    temp_color = COLOR_6;
	    break;

	case 7:
	    temp_color = COLOR_7;
	    break;

	case 8:
	    temp_color = COLOR_8;
	    break;

	case 9:
	    temp_color = COLOR_9;
	    break;
    }

    graph_err_t ret_code;

graph_err_t reset_color_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
	this_graph->color_vtx_list[a_vertex] = 0;
	return GR_OK;
}

graph_err_t update_color_list_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;
    //printf("\tTrying to put color %x >> ", temp_color);

    //Now, let's check whether color can be applied or not.
    if(this_graph->color_vtx_list[a_vertex] & temp_color)
    {
	//Has this color on availability list.
	this_graph->color_vtx_list[a_vertex] = temp_color;

        printf("Color at vertex %d is: %x\n", a_vertex,
				    this_graph->color_vtx_list[a_vertex]);
	//Update adjacent nodes.
	update_color_list_at_vtx_graph(this_graph, a_vertex);
	ret_code = GR_OK;
    }
    else
    {
//	printf("\t>>Color unavailable\n");
	ret_code = GR_COLOR_INVALID;
    }
/*
    printf("Color at vertex %d is: %x\n", a_vertex,
				    this_graph->color_vtx_list[a_vertex]);*/

    return GR_OK;
}
graph_err_t auto_color_vertex_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    //Based on previously generated list, will apply some color to given vtx.
    unsigned char shift_amount;
/*
    while(1)
    {
	shift_amount = rand % COLOR_NUMBER;
    }
*/
    return GR_OK;
}

graph_err_t brute_force_solver(GRAPH* this_graph)
{
	// Backtracking algorithm (https://en.wikipedia.org/wiki/Sudoku_solving_algorithms)
	VECTOR adjacencies;
	vertex_n other_vertex;
	int this_color;
	int other_color;


	for(int i = 0; i < this_graph->size ; i++)
	{
		adjacencies = __get_adjacent_vtxs_graph(this_graph, i);
		this_color = get_color_at_vtx_graph(this_graph, i);
		this_color == -1? this_color = 0 : this_color;
		
		// Return to previous vertex, maximum value for this vertex reached
		if(this_color+1>9)
		{
			// Reset this vertex
			reset_color_at_vtx_graph(this_graph, i);
			i-=2;
			// No solution found
			if(i<-1)
				return GR_NO_SOLUTION;
			// Return to previous vertex
			continue;
		}
    

		// Find colors that cant be used on this vertex
		int8_t colors_blocked[10] = {0};
		for(int j = 0; j < adjacencies.size ; j++)
		{
			other_vertex = adjacencies.data[j];
			// Dont check this vertex
			if(other_vertex == i)
				continue;

			other_color = get_color_at_vtx_graph(this_graph, other_vertex);
			if(other_color != -1)
				colors_blocked[other_color] = 1;
		}
		
		// Update value of this vertex 
		for(int j = this_color+1; j <= 9 ; j++)
		{
			// For each adjacency
			if(colors_blocked[j]==0)
			{
				// Change color of this vertex
				reset_color_at_vtx_graph(this_graph, i);
				put_color_at_vtx_graph(this_graph, i, j);
				break;
			}

			// Could not find possible value for this vertex
			if(j==9)
			{
				// Reset this vertex
				reset_color_at_vtx_graph(this_graph, i);
				i-=2;
				// No solution found
				if(i<-1)
					return GR_NO_SOLUTION;
				// Return to previous vertex
				continue;
			}
		}
	}
	return GR_OK;
}

graph_err_t welsh_powell_solver(GRAPH* this_graph)
{

}
