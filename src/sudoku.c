#include <stdio.h>
#include <gtk/gtk.h>

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

/*Functions on buttons*/


int main(int argc, char* argv[])
{
    GError *error = NULL;
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

    } main_objs;

    gtk_init (&argc, &argv);

    main_objs.builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (main_objs.builder, "layout.glade", &error) == 0)
    {
	g_printerr ("Error loading file: %s\n", error->message);
	g_clear_error (&error);
	return 1;
    }

    //Connect signal handlers to the constructed widgets.
    main_objs.window = gtk_builder_get_object (main_objs.builder, "main_window");
    g_signal_connect (main_objs.window,
			"destroy",
			G_CALLBACK (gtk_main_quit),
			NULL);

    gtk_main();

    return 0;
}

