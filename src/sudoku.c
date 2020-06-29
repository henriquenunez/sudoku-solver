#include <stdio.h>
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

typedef
    struct
    {
        GObject *window;
	GtkBuilder *builder;

	//Widgets
   	GObject *generate_button; //Button
   	GObject *difficulty_chooser_cbox; //Combo box
   	GObject *solver_speed_sbutton; //Spin button
   	GObject *solve_button; //Button
   	GObject *subcells[SUBCELL_N]; //Pointers to subcells.

	unsigned int delay; //Execution delay.

	//Graph related.
	GRAPH* sudoku_graph;

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
	if(temp_color != -1)
	{
	    snprintf(temp_label, 7, "%d", temp_color);
	    gtk_label_set_text(GTK_LABEL(main_objs->subcells[i]),
				temp_label);
	}
    }
}

/*Functions on buttons*/

//Randomly inserts values on subcells.
void __generate_button_clicked(main_obj_t* main_objs)
{
    int predefined_squares;
    int rand_color;
    int rand_vertex;
    char* active_text;
    printf("Generate button clicked!\n");

    active_text =
    gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(main_objs->difficulty_chooser_cbox));
    printf("Active text: %s\n", active_text);

    //GENERATE DEFAULTS TO MEDIUM FOR NOW...
    if(strcmp(active_text, "Medium") == 0)
    {
	//Sets medium parameters.
	predefined_squares = 10;
    }

    printf("graph: %p\n", main_objs->sudoku_graph);

    //According to generate logic, will run through the entire graph randomly
    //and will try to assign random colors to the randomly selected vertex.
    while(predefined_squares)
    {
	rand_color = (rand() % COLOR_NUMBER) + 1;
	rand_vertex = rand() % SUBCELL_N;
	    if(put_color_at_vtx_graph(main_objs->sudoku_graph,
				    rand_vertex,
				    rand_color) == GR_OK) predefined_squares--;
    }

    g_free(active_text);

    __update_sudoku_squares_numbers(main_objs);
}

    //Changes iteration speed.
void __solver_speed_sbutton_changed(main_obj_t* main_objs)
{
    printf("Solver speed spin changed!\n");
}

//This function will run the colouring algorithm.
void __solve_button_clicked(main_obj_t* main_objs)
{
    printf("Solve button clicked!\n");
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
	#ifdef MAIN_DEBUG
	printf("generated: %s\n", subcell_indexer);
	gtk_label_set_text(GTK_LABEL(main_objs->subcells[f]), subcell_indexer);
	#else
	    gtk_label_set_text(GTK_LABEL(main_objs->subcells[f]), "-");
	#endif

    }
}

void __main_init(main_obj_t* main_objs)
{
    GError *error = NULL;

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

    //GENERATE BUTTON
    main_objs->generate_button = gtk_builder_get_object (main_objs->builder,
						"sudoku_generate_button");
    g_signal_connect_swapped (main_objs->generate_button,
			"clicked",
			G_CALLBACK (__generate_button_clicked),
			main_objs);

    //SOLVE BUTTON
    main_objs->solve_button = gtk_builder_get_object (main_objs->builder,
						"sudoku_solve_button");
    g_signal_connect (main_objs->solve_button,
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

}

void __graph_init(main_obj_t* main_objs)
{
    //Creates graph with vertex number equal as the subcell number.
    main_objs->sudoku_graph = new_graph(ADJ_LIST, SUBCELL_N);

    //Now, link vertexes that are related.
    for(int f = 0 ; f < SUBCELL_N ; f++)
    {

	#ifdef MAIN_DEBUG
	printf("LINKING SAME CELL:\n");
	#endif

	//Link subcells at the same cell.
	for(int w = 0 ; w < SUBCELLS_PER_CELL ; w++)
	{
	   insert_edge_graph(main_objs->sudoku_graph,
				f,
				(M_FROM(f)*27) + (N_FROM(f)*9) + w);
	    #ifdef MAIN_DEBUG
	    printf("linking to: %d\n", (M_FROM(f)*27) + (N_FROM(f)*9) + w);
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
		insert_edge_graph(main_objs->sudoku_graph,
				f,
				(M_FROM(f)*27) + (n*9) + I_FROM(f)*3 + j);
	    #ifdef MAIN_DEBUG
	    printf("linking to: %d\n", (M_FROM(f)*27) + (n*9) + I_FROM(f)*3 + j);
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
		insert_edge_graph(main_objs->sudoku_graph,
				f,
				(m*27) + (N_FROM(f)*9) + i*3 + J_FROM(f));
	    #ifdef MAIN_DEBUG
	    printf("linking to: %d\n", (m*27) + (N_FROM(f)*9) + i*3 + J_FROM(f));
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

    gtk_init (&argc, &argv);
    __main_init(&main_objs);
    __init_squares(&main_objs);
    __graph_init(&main_objs);
    gtk_main();

    delete_graph(main_objs.sudoku_graph);

    return 0;
}

