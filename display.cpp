#include "display.h"

GtkBuilder      *builder;
GtkWidget       *window;
GtkWidget       *boxVert;
GtkWidget       *labelDir;
GtkWidget       *labelTime;
GtkWidget       *labelDest;
GtkWidget       *labelETA;

void setLabel(GtkWidget * a, string text)
{
    gtk_label_set_text(GTK_LABEL(a), text.c_str());
}

void setDir(string dir)
{
    setLabel(labelDir, dir);
}

void setTime(string time)
{
    setLabel(labelTime, time);
}

void setDest(string dest)
{
    setLabel(labelDest, dest);
}

void setETA (string ETA)
{
    setLabel(labelETA, ETA);
}

int setupGTKDisplay(int argc, char *argv[])
{
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
    labelTime = GTK_WIDGET(gtk_builder_get_object(builder, "labelTime")); 
    labelDest = GTK_WIDGET(gtk_builder_get_object(builder, "labelDest"));
    labelETA = GTK_WIDGET(gtk_builder_get_object(builder, "labelETA"));


    g_object_unref(builder);
    GdkColor black = {0, 0x0000, 0x0000, 0x0000};
    GdkColor white = {0, 0xFFFF, 0xFFFF, 0xFFFF};
    gtk_widget_modify_bg(boxVert, GTK_STATE_NORMAL, &black); 
    gtk_widget_modify_fg(boxVert, GTK_STATE_NORMAL, &white);

    gtk_widget_show(window);
    //gtk_main();
    return 0;
}