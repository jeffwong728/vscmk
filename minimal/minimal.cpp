#include "wx/wxprec.h"
#include "wx/wx.h"
#include <gtk/gtk.h>
#include <gmodule.h>
#include <glib/gprintf.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};

class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    int blackTheam_m = 0;

private:
    wxDECLARE_EVENT_TABLE();
};

enum
{
    Minimal_Quit = wxID_EXIT,
    Minimal_About = wxID_ABOUT
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    MyFrame *frame = new MyFrame("Minimal wxWidgets App");

    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(G_OBJECT(settings), "gtk-application-prefer-dark-theme", 1, NULL);
    frame->Show(true);
    gtk_widget_show_all(gtk_window_get_titlebar(GTK_WINDOW(frame->m_widget)));

    return true;
}

MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{

    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), title.c_str());
    gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(header_bar), FALSE);
    GtkWidget *menu = gtk_menu_button_new();
    gtk_menu_button_set_direction(GTK_MENU_BUTTON(menu), GTK_ARROW_NONE);
    GtkWidget *popover = gtk_popover_new(NULL);
    gtk_container_set_border_width(GTK_CONTAINER(popover), 10);
    GtkWidget *label = gtk_label_new("Popovers work too!");
    gtk_container_add(GTK_CONTAINER(popover), label);
    gtk_widget_show(label);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu), popover);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), menu);
    gtk_window_set_titlebar(GTK_WINDOW(m_widget), header_bar);

    SetIcon(wxICON(sample));
    wxMenu *fileMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");
    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    gtk_window_set_interactive_debugging(1);
}
