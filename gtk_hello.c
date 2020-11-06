#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gmodule.h>
#include <glib/gprintf.h>

static void
activate(GtkApplication *app,
				 gpointer user_data)
{
	GtkWidget *window;
	GdkPixbuf *pixbuf;
	GtkWidget *image;
	GError *err = NULL;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

	pixbuf = gdk_pixbuf_new_from_file("test.svg", &err);
	if (!pixbuf)
	{
		fprintf(stderr, "Unable to load pixbuf: %s\n", err->message);
		exit(1);
	}
	image = gtk_image_new_from_pixbuf(pixbuf);

	gtk_container_add(GTK_CONTAINER(window), image);

	gtk_widget_show_all(window);
}

int main(int argc,
				 char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
