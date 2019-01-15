#include "display.h"
#include <time.h>  

GtkBuilder      *builder;
GtkWidget       *window;
GtkCssProvider  *cssProvider;

GtkWidget       *boxVert;
GtkWidget       *labelDir;
GtkWidget       *labelTime;
GtkWidget       *labelDest;
GtkWidget       *labelSpeed;
GtkWidget       *labelETA;
GtkWidget       *imgRec;
GtkWidget       *imgBluetooth;

void setLabel(GtkWidget * a, string text)
{
    gtk_label_set_text(GTK_LABEL(a), text.c_str());
}
void setDir(string dir) { setLabel(labelDir, dir); }
void setTime(string time) { setLabel(labelTime, time); }
void setDest(string dest) { setLabel(labelDest, dest); }
void setSpeed (string speed) { setLabel(labelSpeed, speed); }
void setETA (string ETA){ setLabel(labelETA, ETA); }
void setBtImg(bool BtOn){ gtk_image_set_from_file (GTK_IMAGE(imgBluetooth), BtOn?"bt_on.png":"bt_off.png");}

gint timeout_callback (gpointer data)
{
    time_t rawtime;
    time (&rawtime);
    string time = ctime (&rawtime);
    time = time.substr(11, 5);
    setTime(time);

    return 1;
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
    labelSpeed = GTK_WIDGET(gtk_builder_get_object(builder, "labelSpeed"));
    labelETA = GTK_WIDGET(gtk_builder_get_object(builder, "labelETA"));
    imgRec = GTK_WIDGET(gtk_builder_get_object(builder, "imgRec"));
    imgBluetooth = GTK_WIDGET(gtk_builder_get_object(builder, "imgBluetooth"));


    g_object_unref(builder);
    
    gtk_widget_set_name (boxVert, "boxVert");
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "dark.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    //set labels
    setDir("");
    setETA("");
    setSpeed("");
    setDest("Connect your device.");
    setBtImg(false);
    //update time every second.
    g_timeout_add (1000,timeout_callback, NULL);


    gtk_widget_show(window);
    //gtk_main();
    return 0;
}