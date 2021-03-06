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

    // Check if can be colored by adjacencies.
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
/*
    a_vertex_color = get_color_at_vtx_graph(this_graph, a_vertex);

    if(a_vertex_color == 0)
	return -1;
*/
    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];
/*	#ifdef DEBUG_COLOR_SOLVE
	printf("Curr vertex: %d\n", curr_vertex);
	#endif*/
	//If adj is colored, increase count.
	if(get_color_at_vtx_graph(this_graph, curr_vertex) != 0) count++;
    }

    return count;
}

int count_available_colors(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;
    color_t a_vertex_color;
    int colors_avail[COLOR_NUMBER] = {1,2,3,4,5,6,7,8,9};
    int count = 0;
    int aux;

    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];
	aux = get_color_at_vtx_graph(this_graph, curr_vertex);
	if(aux == 0) continue;
	if(aux > COLOR_NUMBER) return -2; //Colored with more than 9. No solution.
	//Zero list of availability.
	colors_avail[aux - 1] = 0;
    }

    //Count list of availability.
    for(int j = 0 ; j < COLOR_NUMBER ; j++)
    {
	if (colors_avail[j] > 0) count++;
    }

    return count;
}

int get_first_available_color(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;
    color_t a_vertex_color;
    int colors_avail[COLOR_NUMBER] = {1,2,3,4,5,6,7,8,9};
    int ret_color = -1;
    int aux;

    if(count_available_colors(this_graph, a_vertex) > 1)
    {
	return -1;
    }

    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
	curr_vertex = adjacencies.data[i];
	aux = get_color_at_vtx_graph(this_graph, curr_vertex);
	if(aux == 0) continue;
	//Zero list of availability.
	colors_avail[aux - 1] = 0;
    }

    //Find first color in list.
    for(int j = 0 ; j < COLOR_NUMBER ; j++)
    {
	if (colors_avail[j] > 0)
	{
	    ret_color = colors_avail[j];
	    break;
	}
    }

    return ret_color;
}

graph_err_t update_color_list_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
    VECTOR adjacencies = __get_adjacent_vtxs_graph(this_graph, a_vertex);
    vertex_n curr_vertex;

    //Gets color, should be one of the previously defined macros, or 0x00
    color_t a_vertex_color = this_graph->color_vtx_list[a_vertex];

    if(a_vertex_color <= 0) return GR_VTX_NOT_COLORED;

    //Run on every vertex thats adjacent to 'a_vertex'
    for(int i = 0 ; i < adjacencies.size ; i++)
    {
		curr_vertex = adjacencies.data[i];
		this_graph->color_vtx_list[curr_vertex] =
				this_graph->color_vtx_list[curr_vertex] & (~a_vertex_color);
    }

    return GR_OK;
}

graph_err_t reset_color_at_vtx_graph(GRAPH* this_graph, vertex_n a_vertex)
{
	this_graph->color_vtx_welsh_list[a_vertex] = 0;
	return GR_OK;
}

graph_err_t color_solver(GRAPH* this_graph)
{
    while(this_graph->colored_vtxs < this_graph->size)
    {
	int color_number = 1;
	int saturation_dgree = 21, tempdgree; //The lowest, the most unsaturated.
	int index = -1;
	int available_colors = -1;
	int color = -1;

	for(int i = 0 ; i < this_graph->size ; i++)
	{
	    if(get_color_at_vtx_graph(this_graph, i) == 0) //If not colored.
	    {
		available_colors = count_available_colors(this_graph, i);

		if(available_colors == 1)
		{
		    //Then theres only one color available. Color node.
		    color = get_first_available_color(this_graph, i);
		    put_color_at_vtx_graph(this_graph, i, color);
		    i = 0;
		    saturation_dgree = 21;
		    continue;
		}
		else if(available_colors == -2)
		{
		    return GR_NO_SOLUTION;
		}

		tempdgree =
		    count_adj_colored(this_graph, i);

		#ifdef DEBUG_COLOR_SOLVE
		printf("Degree %d of vertex %d\n", tempdgree, i);
		#endif

		if(tempdgree < saturation_dgree)
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

	#ifdef DEBUG_COLOR_SOLVE
	printf("Convergency: degree %d of vertex %d\n", tempdgree, index);
	#endif
	if(index == -1) return GR_NO_SOLUTION;
	color_number = get_first_available_color(this_graph, index);
	if(color_number > 0){
	    put_color_at_vtx_graph(this_graph, index, color_number);
	}
	if(color_number == -1) return GR_NO_SOLUTION;
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
	const long int max_gen = 10000;// Maximum of 10000 generations
	const int qtd_ind = 200;// 200 individuals
	// Run time parameters
	char found_solution = 0;

	// Population info
	int genes[qtd_ind][size];
	// Fitness = quantity conflict (solution when fitness==0)
	int fitness[qtd_ind];
	int best_ind = 0;
	int worse_ind = 0;
	int generation = 0;

	// Initialize population
	for(int ind = 0; ind < qtd_ind ; ind++)
	{
		for(int gen = 0; gen < size ; gen++)
		{
			// Each vertex in the gene with random value from 0 to 9
			int row = gen/9;
			int col = gen%9;
			int block_row = row/3;
			int block_col = col/3;
			int graph_vert = (block_row*3+block_col)*9 + (row-block_row*3)*3 + (col-block_col*3);
			
			// Convert from graph_vert to vert
			int this_color = get_color_at_vtx_graph(this_graph, graph_vert);
			if(this_color <= 0)
				genes[ind][gen] = rand()%9+1;
			else
				genes[ind][gen] = this_color;
		}
	}

	do	
	{
		generation++;
		printf("[Gen %d] Best ind: %d mistakes\n", generation, fitness[best_ind]);
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
			
				int genes_to_mutate = 5;
				while(genes_to_mutate--)
				{
					// Mutate one random gene
					int random_gene = rand()%size;

					int row = random_gene/9;
					int col = random_gene%9;
					int block_row = row/3;
					int block_col = col/3;
					int graph_vert = (block_row*3+block_col)*9 + (row-block_row*3)*3 + (col-block_col*3);

					int this_color = get_color_at_vtx_graph(this_graph, graph_vert);
					if(this_color <= 0)
						genes[ind][random_gene] = rand()%9+1;
					else
						genes[ind][random_gene] = this_color;
				}
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
					int row = gene/9;
					int col = gene%9;
					int block_row = row/3;
					int block_col = col/3;
					int graph_vert = (block_row*3+block_col)*9 + (row-block_row*3)*3 + (col-block_col*3);

					int this_color = get_color_at_vtx_graph(this_graph, graph_vert);
					if(this_color <= 0)
						genes[ind][gene] = rand()%9+1;
					else
						genes[ind][gene] = this_color;
				}
			}
		}
	}while(fitness[best_ind] > 0 && generation < max_gen);

	// Populate graph with answer
	int col, row, block_col, block_row;
	int graph_vert;
	for(int vert = 0; vert < this_graph->size ; vert++)
	{
		row = vert/9;
		col = vert%9;
		block_row = row/3;
		block_col = col/3;
		graph_vert = (block_row*3+block_col)*9 + (row-block_row*3)*3 + (col-block_col*3);

		this_graph->color_vtx_welsh_list[graph_vert] = genes[best_ind][vert];
	}

	// Check if the sudoku is complete
	if(fitness[best_ind]==0)
		return GR_OK;
	else
		return GR_NO_SOLUTION;
}

graph_err_t backtracking_solver(GRAPH* this_graph)
{
	// Backtracking algorithm (https://en.wikipedia.org/wiki/Sudoku_solving_algorithms)
	if(this_graph == NULL || this_graph->size==0)
		return GR_NO_SOLUTION;

	VECTOR adjacencies;
	vertex_n other_vertex;
	int this_color;
	int other_color;
	int start_vertex = -1;
	int start_graph[this_graph->size];
	int go_back = 0;

	// Populate start graph (cant change these values)
	for(int vert = 0; vert < this_graph->size ; vert++)
	{
		int row = vert/9;
		int col = vert%9;
		int block_row = row/3;
		int block_col = col/3;
		int i = (block_row*3+block_col)*9 + (row-block_row*3)*3 + (col-block_col*3);

		this_color = this_graph->color_vtx_welsh_list[i];
		start_graph[i] = this_color;

		// Find start vertex (First empty cell)
		if(start_vertex == -1 && this_color<=0)
			start_vertex = vert;
	}

	// Start from vertex 0
	for(int vert = start_vertex; vert < this_graph->size ; vert++)
	{
		int row = vert/9;
		int col = vert%9;
		int block_row = row/3;
		int block_col = col/3;
		int i = (block_row*3+block_col)*9 + (row-block_row*3)*3 + (col-block_col*3);

		if(start_graph[i]>0)
		{
			if(go_back==0)
				continue;
			else
			{
				vert-=2;
				// No solution found
				if(vert<start_vertex-1)
					return GR_NO_SOLUTION;
				// Return to previous vertex
				go_back = 1;
				continue;
			}
		}
		go_back = 0;

		adjacencies = __get_adjacent_vtxs_graph(this_graph, i);
		this_color = this_graph->color_vtx_welsh_list[i];
		// Colors from 0 to 9
		this_color == -1? this_color = 0 : this_color;

		// Start from next color
		this_color++;

		// Return to previous vertex if maximum value for this vertex reached
		if(this_color>9)
		{
			// Reset this vertex
			this_graph->color_vtx_welsh_list[i] = 0;
			vert-=2;
			// No solution found
			if(vert<start_vertex-1)
				return GR_NO_SOLUTION;
			// Return to previous vertex
			go_back = 1;
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

			other_color = this_graph->color_vtx_welsh_list[other_vertex];
			if(other_color != -1)
				colors_blocked[other_color] = 1;
		}

		char can_change_color = 0;
		// Update value of this vertex with lowest possible value
		for(int j = this_color; j <= 9 ; j++)
		{
			// For each adjacency
			if(colors_blocked[j]==0)
			{
				// Change color of this vertex
				this_graph->color_vtx_welsh_list[i] = j;
				can_change_color = 1;
				break;
			}
		}

		if(!can_change_color)
		{
			// If cant change color, reset this vertex and return to previous
			// Reset this vertex
			this_graph->color_vtx_welsh_list[i] = 0;
			vert-=2;// i-=2;i++; -> i-=1

			// No solution found
			if(vert<start_vertex-1)
				return GR_NO_SOLUTION;
			// Return to previous vertex
			go_back = 1;
			continue;
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
			if(this_color > 0)
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
				if(other_color == color && other_color>0)
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
		if(this_color <= 0)
			return GR_NO_SOLUTION;
	}
	return GR_OK;
}
