#include "ui.h"

#include <gtk/gtk.h>

static GtkBuilder *ui_builder;

void ui_init(int *argc, char **argv[])
{
    GtkWindow *window;

    /* initialize gtk */
    gtk_init(argc, argv);

    /* load glade file */
    ui_builder = gtk_builder_new();
    gtk_builder_add_from_file(ui_builder, "tekvisaosc.ui", NULL);

    /* connect signals to objects from builder */
    window = GTK_WINDOW(gtk_builder_get_object(ui_builder, "main_window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* start gtk */
    gtk_main();
}
