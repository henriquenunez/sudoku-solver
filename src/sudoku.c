#include <stdio.h>
#include <gtk/gtk.h>

#include "graph.h"

//Number of subcells a.k.a sudoku squares.
#define SUBCELL_N 81

const char* subcell_id[] =
{
//Cell 0 0
"subcell_0_0_0_0",//#0
"subcell_0_0_0_1",//#1
"subcell_0_0_0_2",//#2
"subcell_0_0_1_0",//#3
"subcell_0_0_1_1",//#4
"subcell_0_0_1_2",//#5
"subcell_0_0_2_0",//#6
"subcell_0_0_2_1",//#7
"subcell_0_0_2_2",//#8
//Cell 0 1
"subcell_0_1_0_0",//#9
"subcell_0_1_0_1",//#10
"subcell_0_1_0_2",//#11
"subcell_0_1_1_0",//#12
"subcell_0_1_1_1",//#13
"subcell_0_1_1_2",//#14
"subcell_0_1_2_0",//#15
"subcell_0_1_2_1",//#16
"subcell_0_1_2_2",//#17
//Cell 0 2
"subcell_0_2_0_0",//#18
"subcell_0_2_0_1",//#19
"subcell_0_2_0_2",//#20
"subcell_0_2_1_0",//#21
"subcell_0_2_1_1",//#22
"subcell_0_2_1_2",//#23
"subcell_0_2_2_0",//#24
"subcell_0_2_2_1",//#25
"subcell_0_2_2_2",//#26
//Cell 1 0
"subcell_1_0_0_0",//#27
"subcell_1_0_0_1",//#28
"subcell_1_0_0_2",//#29
"subcell_1_0_1_0",//#30
"subcell_1_0_1_1",//#31
"subcell_1_0_1_2",//#32
"subcell_1_0_2_0",//#33
"subcell_1_0_2_1",//#34
"subcell_1_0_2_2",//#35
//Cell 1 1
"subcell_1_1_0_0",//#36
"subcell_1_1_0_1",//#37
"subcell_1_1_0_2",//#38
"subcell_1_1_1_0",//#39
"subcell_1_1_1_1",//#40
"subcell_1_1_1_2",//#41
"subcell_1_1_2_0",//#42
"subcell_1_1_2_1",//#43
"subcell_1_1_2_2",//#44
//Cell 1 2
"subcell_1_2_0_0",//#45
"subcell_1_2_0_1",//#46
"subcell_1_2_0_2",//#47
"subcell_1_2_1_0",//#48
"subcell_1_2_1_1",//#49
"subcell_1_2_1_2",//#50
"subcell_1_2_2_0",//#51
"subcell_1_2_2_1",//#52
"subcell_1_2_2_2",//#53
//Cell 2 0
"subcell_2_0_0_0",//#54
"subcell_2_0_0_1",//#55
"subcell_2_0_0_2",//#56
"subcell_2_0_1_0",//#57
"subcell_2_0_1_1",//#58
"subcell_2_0_1_2",//#59
"subcell_2_0_2_0",//#60
"subcell_2_0_2_1",//#61
"subcell_2_0_2_2",//#62
//Cell 2 1
"subcell_2_1_0_0",//#63
"subcell_2_1_0_1",//#64
"subcell_2_1_0_2",//#65
"subcell_2_1_1_0",//#66
"subcell_2_1_1_1",//#67
"subcell_2_1_1_2",//#68
"subcell_2_1_2_0",//#69
"subcell_2_1_2_1",//#70
"subcell_2_1_2_2",//#71
//Cell 2 2
"subcell_2_2_0_0",//#72
"subcell_2_2_0_1",//#73
"subcell_2_2_0_2",//#74
"subcell_2_2_1_0",//#75
"subcell_2_2_1_1",//#76
"subcell_2_2_1_2",//#77
"subcell_2_2_2_0",//#78
"subcell_2_2_2_1",//#79
"subcell_2_2_2_2" //#80
};

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
    for(int i = 0 ; i < SUBCELL_N ; i++)
    {
	/*something like thye following.
	set_value(main_objs->subcells[i],
		    get_color_at_vertex(main_objs->sudoku_graph, i))*/
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

    //TODO  memory problem related to difficulty_chooser_cbox.
    //active_text = gtk_combo_box_text_get_active_text (main_objs->difficulty_chooser_cbox);
    printf("Active text: %p\n", active_text);

    active_text = "Medium";

    //GENERATE DEFAULTS TO MEDIUM FOR NOW...
    if(strcmp(active_text, "Medium") == 0)
    {
	//Sets medium parameters.
	predefined_squares = 10;
    }

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

    //g_free(active_text);
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
    g_signal_connect (main_objs->generate_button,
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

//MAIN

int main(int argc, char* argv[])
{
    main_obj_t main_objs;

    gtk_init (&argc, &argv);
    __main_init(&main_objs);
    gtk_main();

    return 0;
}

