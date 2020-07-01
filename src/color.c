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

    //Gets color, should be one of the previously defined macros.
    color_t a_vertex_color = this_graph->color_vtx_list[a_vertex];

    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];

/*	printf("Vertex %d colors: %x\n", curr_vertex,
		this_graph->color_vtx_list[curr_vertex]);
	printf("Removing color %x at vertex %d\n", a_vertex_color, curr_vertex);
*/
	this_graph->color_vtx_list[curr_vertex] =
		    this_graph->color_vtx_list[curr_vertex] & (~a_vertex_color);
/*	printf("after >> Vertex %d colors: %x\n", curr_vertex,
		this_graph->color_vtx_list[curr_vertex]);
*/

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
//  printf("\t>>Color unavailable\n");
    ret_code = GR_COLOR_INVALID;
    }
/*
    printf("Color at vertex %d is: %x\n", a_vertex,
		    this_graph->color_vtx_list[a_vertex]);*/

    return GR_OK;
}
/*
graph_err_t put_color_at_vtx_graph(GRAPH* this_graph,
				    vertex_n a_vertex,
				    int some_color)
{
    if (some_color < 1 || some_color > COLOR_NUMBER) return GR_COLOR_INVALID;
    if (get_color_at_vtx_graph(this_graph, a_vertex) != -1) return GR_VTX_COLORED;

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
*/
graph_err_t reset_color_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
	this_graph->color_vtx_list[a_vertex] = 0;
	return GR_OK;
}

graph_err_t color_solver(GRAPH* this_graph)
{
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
	VECTOR adjacencies;
	vertex_n other_vertex;
	int this_color;
	int other_color;

	// For each color
	for(int color = 1 ; color <= 9; color++)
	{
		// For each vertex (same vertex)
		// Because all vertices have the same degree, dont need to order
		for(int vert = 0; vert < this_graph->size ; vert++)
		{
			adjacencies = __get_adjacent_vtxs_graph(this_graph, vert);
			this_color = get_color_at_vtx_graph(this_graph, vert);

			// Pass vertex already colored
			if(this_color != -1)
				continue;

			int8_t can_color = 1;
			// Check if it is adjacent to other vertex with this color
			for(int j = 0; j < adjacencies.size ; j++)
			{
				other_vertex = adjacencies.data[j];
				// Dont check vertex j with itself
				if(other_vertex == vert)
					continue;

				other_color = get_color_at_vtx_graph(this_graph, other_vertex);
				if(other_color == color)
					can_color = 0;
			}

			if(can_color)
			{
				put_color_at_vtx_graph(this_graph, vert, color);
			}
		}
	}

	// Check if the sudoku is complete
	for(int vert = 0; vert < this_graph->size ; vert++)
	{
		this_color = get_color_at_vtx_graph(this_graph, vert);
		if(this_color == -1)
			return GR_NO_SOLUTION;
	}
	return GR_OK;
}


graph_err_t genetic_algorithm_solver(GRAPH* this_graph)
{
	// Solving sudoku with genetic algorithm
	if(this_graph == NULL || this_graph->size == 0)
		return GR_ERROR;

	// Fixed parameters
	const int size = this_graph->size;
	const int max_gen = 10000;// Maximum of 1000 generations
	const int qtd_ind = 1000;// 10 individuals
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
			genes[ind][gen] = rand()%9+1;
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
				for(int i = 0; i < 10; i++)
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


