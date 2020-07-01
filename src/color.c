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

    switch(some_color)
    {
	case 1:
	    this_graph->color_vtx_list[a_vertex] = COLOR_1;
	    break;

	case 2:
	    this_graph->color_vtx_list[a_vertex] = COLOR_2;
	    break;

	case 3:
	    this_graph->color_vtx_list[a_vertex] = COLOR_3;
	    break;

	case 4:
	    this_graph->color_vtx_list[a_vertex] = COLOR_4;
	    break;

	case 5:
	    this_graph->color_vtx_list[a_vertex] = COLOR_5;
	    break;

	case 6:
	    this_graph->color_vtx_list[a_vertex] = COLOR_6;
	    break;

	case 7:
	    this_graph->color_vtx_list[a_vertex] = COLOR_7;
	    break;

	case 8:
	    this_graph->color_vtx_list[a_vertex] = COLOR_8;
	    break;

	case 9:
	    this_graph->color_vtx_list[a_vertex] = COLOR_9;
	    break;
    }

    return GR_OK;
}

graph_err_t update_color_list_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;

    //Gets color, should be one of the previously defined macros, or 0x00
    color_t a_vertex_color = this_graph->color_vtx_list[a_vertex];

    if(a_vertex_color == 0) return GR_VTX_NOT_COLORED;

    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];
	this_graph->color_vtx_list[curr_vertex] =
		    this_graph->color_vtx_list[curr_vertex] & (~a_vertex_color);
    }

    return GR_OK;
}

*/
graph_err_t reset_color_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
	this_graph->color_vtx_list[a_vertex] = 0;
	return GR_OK;
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
	// Solving sudoku with genetic algorithm
	if(this_graph == NULL || this_graph->size == 0)
		return GR_ERROR;

	// Fixed parameters
	const int size = this_graph->size;
	const int max_gen = 100000;// Maximum of 1000 generations
	const int qtd_ind = 10;// 10 individuals
	// Run time parameters
	char found_solution = 0;

	// Population info
	int genes[qtd_ind][size];
	// Fitness = quantity conflict (solution when fitness==0)
	int fitness[qtd_ind];
	int best_ind;
	int worse_ind;
	int generation = 0;

	// Initialize population
	for(int ind = 0; ind < qtd_ind ; ind++)
	{
		for(int gen = 0; gen < size ; gen++)
		{
			// Each vertex in the gene with random value from 0 to 9
			int this_color = get_color_at_vtx_graph(this_graph, gen);
			if(this_color == -1)
				genes[ind][gen] = rand()%9+1;
			else
				genes[ind][gen] = this_color;
		}
	}

	do	
	{
		generation++;
		printf("best ind: %d %d\n", fitness[best_ind], generation);
		// Calculate fitness
		for(int ind = 0; ind < qtd_ind ; ind++)
		{
			fitness[ind] = 0;
			int qty_conflicts = 0;
			for(int vertex = 0; vertex < size ; vertex++)
			{
				int row = vertex/9;
				int col = vertex%9;
				int block_row = row/3;
				int block_col = col/3;

				// Check vertices on same row
				for(int aux = row*9 ; aux < row*9+9 ; aux++)
				{
					if(aux == vertex)
						continue;
					if(genes[ind][aux] == genes[ind][vertex])
						fitness[ind]++;
				}
				// Check vertices on same column
				for(int aux = col ; aux < size ; aux+=9)
				{
					if(aux == vertex)
						continue;
					if(genes[ind][aux] == genes[ind][vertex])
						fitness[ind]++;
				}

				// Check vertices on same square
				for(int i = block_row ; i < block_row+3 ; i++)
				{
					for(int j = block_col ; j < block_col+3 ; j++)
					{
						int aux = j*9+i;

						if(aux == vertex)
							continue;

						if(genes[ind][aux] == genes[ind][vertex])
							fitness[ind]++;
					}
				}
			}
		}

		// Find best ind
		best_ind = 0;
		worse_ind = 0;
		for(int ind = 0; ind < qtd_ind ; ind++)
		{
			if(fitness[ind]<fitness[best_ind])
			{
				best_ind = ind;
			}
			if(fitness[ind]>fitness[best_ind])
			{
				worse_ind = ind;
			}
		}

		// Crossing and mutation
		if(generation%100 != 0)
		{
			for(int ind = 0; ind < qtd_ind ; ind++)
			{
				if(ind == best_ind)
					continue;

				// Get 20 genes from best ind
				for(int i = 0; i < 20; i++)
				{
					int random_gene = rand()%size;
					genes[ind][random_gene] = genes[best_ind][random_gene];
				}
			
				// Mutate one random gene
				int random_gene = rand()%size;
				genes[ind][random_gene] = rand()%9+1;
			}
		}
		else
		{
			// Genocide
			for(int ind = 0; ind < qtd_ind ; ind++)
			{
				if(ind == best_ind)
					continue;
				for(int gene = 0; gene < size; gene++)
				{
					genes[ind][gene] = rand()%9+1;
				}
			}
		}
	}while(fitness[best_ind] > 0 && generation < max_gen);
	// Check if the sudoku is complete
	for(int vert = 0; vert < this_graph->size ; vert++)
	{
		put_color_at_vtx_graph(this_graph, vert, genes[best_ind][vert]);
	}

	if(fitness[best_ind]==0)
		return GR_OK;
	else
		return GR_NO_SOLUTION;
}

