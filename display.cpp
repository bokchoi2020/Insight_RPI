#include <gtk/gtk.h>

int main(int argc, char *argv[])
{

    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *boxVert;
    GtkWidget       *labelDir;
    GError     *error = NULL;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if( ! gtk_builder_add_from_file( builder, "UI.glade", &error ) )
    {
            g_warning( "%s", error->message );
            g_free( error );
            return( 1 );
    }
    window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    boxVert = GTK_WIDGET(gtk_builder_get_object(builder, "boxVert"));
    gtk_builder_connect_signals(builder, NULL);
   // g_signal_connect (window, "delete_event", G_CALLBACK (on_MainWindow_destroy_event), NULL);
    labelDir = GTK_WIDGET(gtk_builder_get_object(builder, "labelDir"));
    g_object_unref(builder);
    GdkColor black = {0, 0x0000, 0x0000, 0x0000};
    GdkColor white = {0, 0xFFFF, 0xFFFF, 0xFFFF};
    gtk_widget_modify_bg(boxVert, GTK_STATE_NORMAL, &black); 
    gtk_widget_modify_fg(boxVert, GTK_STATE_NORMAL, &white);

    gtk_widget_show(window);
    gtk_main();
    return 0;

}