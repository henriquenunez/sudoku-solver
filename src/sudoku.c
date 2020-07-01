#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gtk/gtk.h>

#include "graph.h"

//Number of subcells a.k.a sudoku squares.
#define SUBCELL_N 81
#define SUBCELLS_PER_CELL 9
#define SUBCELLS_PER_ROW 9
#define SUBCELLS_PER_COL 9

/* Macros for index helping index var */
#define M_FROM(F) ((int)(((int)(F/9))/3))
#define N_FROM(F) (((int)(F/9))%3)
#define I_FROM(F) ((int)((F%9)/3))
#define J_FROM(F) (((int)F%9)%3)

/*
*/

typedef
    struct
    {
	int generated;
	GObject *window;
	GtkBuilder *builder;

	//Widgets
	GObject *reset_button; //Button
	GObject *preset_file_button; //Button
	GObject *difficulty_chooser_cbox; //Combo box
	GObject *algo_chooser_cbox; //Combo box
	GObject *solver_speed_sbutton; //Spin button
	GObject *solve_button; //Button
	GObject *subcells[SUBCELL_N]; //Pointers to subcells.

	unsigned int delay; //Execution delay.

	//Graph related.
	GRAPH* sudoku_graph;

	FILE* file_preset;
    }
main_obj_t;


//Runs on every subcell, updating it.
void __update_sudoku_squares_numbers(main_obj_t* main_objs)
{
    int temp_color;
    char temp_label[8];

    for(int i = 0 ; i < SUBCELL_N ; i++)
    {
		//something like the following.
		temp_color = get_color_at_vtx_graph(main_objs->sudoku_graph, i);
		if(temp_color > 0)
		{
			snprintf(temp_label, 7, "%d", temp_color);

			gtk_label_set_text(GTK_LABEL(main_objs->subcells[i]),
					temp_label);
		}
		else
		{
			temp_label[0]='-';
			temp_label[1]='\0';
			gtk_label_set_text(GTK_LABEL(main_objs->subcells[i]),
					temp_label);
		}
    }
}

void __read_preset_file(main_obj_t* main_objs)
{
	int col, row, block_col, block_row;
	int graph_index;

	int color = 0;
	int index = 0;

	// Reset graph
    for(int i = 0 ; i < SUBCELL_N ; i++)
    {
		reset_color_at_vtx_graph(main_objs->sudoku_graph, i);
	}

	// Populate with values from file 
	fseek(main_objs->file_preset, 0, SEEK_SET);
	while(fscanf(main_objs->file_preset, "%d", &color) == 1) 
	{
		if(color != 0)
		{
			row = index/9;
			col = index%9;
			block_row = row/3;
			block_col = col/3;
			graph_index = (block_row*3+block_col)*9 + (row-block_row*3)*3 + (col-block_col*3);

			if(put_color_at_vtx_graph(main_objs->sudoku_graph, graph_index, color) != GR_OK)
			{
				printf("COLOR %d COULD NOT BE ASSIGNED TO %d!\n", color, graph_index);
			}
		}

		index++;
    }

    __update_sudoku_squares_numbers(main_objs);
}

void __file_preset_clicked(main_obj_t* main_objs)
{
    GtkWidget *preset_dialog; //Preset chooser.
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    preset_dialog = gtk_file_chooser_dialog_new ("Open File",
                                      GTK_WINDOW(main_objs->window),
                                      action,
                                      "_Cancel",
                                      GTK_RESPONSE_CANCEL,
                                      "_Open",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

    res = gtk_dialog_run (GTK_DIALOG (preset_dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (preset_dialog);
		filename = gtk_file_chooser_get_filename (chooser);
		main_objs->file_preset = fopen(filename, "r");

		__read_preset_file(main_objs);

		g_free(filename);
    }

    gtk_widget_destroy (preset_dialog);

}

void __reload_preset_file(main_obj_t* main_objs)
{
	// Check if some file is open
	if(main_objs->file_preset != NULL)
		__read_preset_file(main_objs);
	else
	{
		// Reset graph
		for(int i = 0 ; i < SUBCELL_N ; i++)
		{
			reset_color_at_vtx_graph(main_objs->sudoku_graph, i);
		}
	}
}

//Changes iteration speed.
void __solver_speed_sbutton_changed(main_obj_t* main_objs)
{
    printf("Solver speed spin changed!\n");
}

//This function will run the colouring algorithm.
void __solve_button_clicked(main_obj_t* main_objs)
{
    char* active_text;
	clock_t start, end;
    double cpu_time_used;

    active_text =
	gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(main_objs->algo_chooser_cbox));
    printf("Current algorithm: %s\n", active_text);

    if(strcmp(active_text, "Genetic Algorithm") == 0)
    {
		start = clock();
		if(genetic_algorithm_solver(main_objs->sudoku_graph) == GR_NO_SOLUTION)
			printf("No solution found.\n");
		end = clock();
    }
    else if(strcmp(active_text, "Graph Coloring") == 0)
    {
		start = clock();
		if(color_solver(main_objs->sudoku_graph) == GR_NO_SOLUTION)
			printf("No solution found.\n");
		end = clock();
    }
	else if(strcmp(active_text, "Backtracking") == 0)
	{
		start = clock();
		if(backtracking_solver(main_objs->sudoku_graph) == GR_NO_SOLUTION)
			printf("No solution found.\n");
		end = clock();
	}
	else if(strcmp(active_text, "Welsh Powell") == 0)
	{
		start = clock();
		if(welsh_powell_solver(main_objs->sudoku_graph) == GR_NO_SOLUTION)
			printf("No solution found.\n");
		end = clock();
	}

	double time_taken = ((double)end - start)/CLOCKS_PER_SEC;
    printf("Time to finish: %lfs\n", time_taken);

    g_free(active_text);
    __update_sudoku_squares_numbers(main_objs);
}

/* General functions */
void __init_squares(main_obj_t* main_objs)
{
    char subcell_indexer[23];

    for(int f = 0 ; f < SUBCELL_N ; f++)
    {
		snprintf(subcell_indexer,
				22,
				"subcell_%d_%d_%d_%d_label",
				M_FROM(f), N_FROM(f), I_FROM(f), J_FROM(f));
		main_objs->subcells[f] = gtk_builder_get_object(main_objs->builder,
								subcell_indexer);
		gtk_label_set_text(GTK_LABEL(main_objs->subcells[f]), "-");
    }
}

void __main_init(main_obj_t* main_objs)
{
    GError *error = NULL;

    main_objs->generated = 0;
	main_objs->file_preset = NULL;

    main_objs->builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (main_objs->builder, "layout.glade", &error) == 0)
    {
	g_printerr ("Error loading file: %s\n", error->message);
	g_clear_error (&error);
	exit(1);
    }

    //APPLICATION WINDOW
    main_objs->window = gtk_builder_get_object (main_objs->builder,
						"main_window");
    g_signal_connect (main_objs->window,
			"destroy",
			G_CALLBACK (gtk_main_quit),
			NULL);

    //PRESET BUTTON
    main_objs->preset_file_button = gtk_builder_get_object (main_objs->builder,
						"sudoku_preset_sel_button");
    g_signal_connect_swapped (main_objs->preset_file_button,
			"clicked",
			G_CALLBACK (__file_preset_clicked),
			main_objs);


    //RESET BUTTON
    main_objs->reset_button = gtk_builder_get_object (main_objs->builder,
						"sudoku_reset_button");
    g_signal_connect_swapped (main_objs->reset_button,
			"clicked",
			G_CALLBACK (__reload_preset_file),
			main_objs);

    //SOLVE BUTTON
    main_objs->solve_button = gtk_builder_get_object (main_objs->builder,
						"sudoku_solve_button");
    g_signal_connect_swapped (main_objs->solve_button,
			"clicked",
			G_CALLBACK (__solve_button_clicked),
			main_objs);

    //SOLVER SPEED
    main_objs->solver_speed_sbutton =
		    gtk_builder_get_object (main_objs->builder,
					    "sudoku_solver_speed_spin_button");
    g_signal_connect (main_objs->solver_speed_sbutton,
			"change-value",
			G_CALLBACK (__solver_speed_sbutton_changed),
			main_objs);

    //DIFFICULTY COMBO BOX
    main_objs->difficulty_chooser_cbox =
		gtk_builder_get_object (main_objs->builder,
					"sudoku_difficulty_chooser_combo_box");

    //DIFFICULTY COMBO BOX
    main_objs->algo_chooser_cbox =
		gtk_builder_get_object (main_objs->builder,
					"sudoku_algorithm_chooser_combo_box");

    srand(time(NULL));
}

void __graph_init(main_obj_t* main_objs)
{
    //Creates graph with vertex number equal as the subcell number.
    main_objs->sudoku_graph = new_graph(ADJ_LIST, SUBCELL_N);

    //Only needs index.
    #define ITER_CELL(A, B) ((M_FROM(A)*27) + (N_FROM(A)*9) + B)
    //Runs on squares (left and right, thus requires 2 indexes.)
    #define ITER_ROW(A, B, C) ((M_FROM(A)*27) + (B*9) + I_FROM(A)*3 + C)
    //Runs on squares (upper and lower, thus requires 2 indexes.)
    #define ITER_COL(A, B, C) ((B*27) + (N_FROM(A)*9) + C*3 + J_FROM(A))

    //Now, link vertexes that are related.
    for(int f = 0 ; f < SUBCELL_N ; f++)
    {

	#ifdef MAIN_DEBUG
	printf("%d LINKING SAME CELL:\n", f);
	#endif

	//Link subcells at the same cell.
	for(int w = 0 ; w < SUBCELLS_PER_CELL ; w++)
	{
	    if(ITER_CELL(f, w) == f)
	    {
		//printf("Loop on cell.\n");
		continue; //Dont make loops.
	    }
	    insert_edge_graph(main_objs->sudoku_graph,
				f,
				ITER_CELL(f, w));
	    #ifdef MAIN_DEBUG
	    printf("linking to: %d\n", ITER_CELL(f, w));
	    #endif
	}

	#ifdef MAIN_DEBUG
	printf("LINKING SAME ROW:\n");
	#endif

	//Link subcells at the same row.
	for(int n = 0 ; n < 3 ; n++) //Walking on n
	{
	    for(int j = 0 ; j < 3 ; j++) //Walking on j
	    {
		if(ITER_ROW(f, n, j) == f)
		{
		    //printf("Loop on row.\n");
		    continue; //Dont make loops.
		}
		insert_edge_graph(main_objs->sudoku_graph,
				f,
				ITER_ROW(f, n, j));
	    #ifdef MAIN_DEBUG
	    printf("linking to: %d\n", ITER_ROW(f, n, j));
	    #endif
	    }
	}

	#ifdef MAIN_DEBUG
	printf("LINKING SAME COLUMN:\n");
	#endif

	//Link subcells at the same column.
	for(int m = 0 ; m < 3 ; m++) //Walking on m
	{
	    for(int i = 0 ; i < 3 ; i++) //Walking on i
	    {
		if(ITER_COL(f, m, i) == f)
		{
		    //printf("Loop on column.\n\n\n");
		    continue; //Dont make loops.
		}

	    insert_edge_graph(main_objs->sudoku_graph,
				f,
				ITER_COL(f, m, i));
	    #ifdef MAIN_DEBUG
	    printf("linking to: %d\n", ITER_COL(f, m, i));
	    #endif
	    }
	}
	#ifdef MAIN_DEBUG
	printf("\n\n\n");
	#endif
    }
}

//MAIN

int main(int argc, char* argv[])
{
    main_obj_t main_objs;

    gtk_init(&argc, &argv);
    __main_init(&main_objs);
    __init_squares(&main_objs);
    __graph_init(&main_objs);
    gtk_main();

    delete_graph(main_objs.sudoku_graph);

	free(main_objs.file_preset);
    return 0;
}

