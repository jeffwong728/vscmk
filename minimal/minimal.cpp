#include "wx/wxprec.h"
#include "wx/wx.h"
#include <gtk/gtk.h>
#include <gmodule.h>s
#include <glib/gprintf.h>
#include <cairomm/cairomm.h>

class BasicDrawPane : public wxPanel
{

public:
    BasicDrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
    BasicDrawPane * drawPane;
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

BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
    EVT_PAINT(BasicDrawPane::paintEvent)
    END_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    MyFrame *frame = new MyFrame("Minimal wxWidgets App");

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPane = new BasicDrawPane((wxFrame*)frame);
    sizer->Add(drawPane, 1, wxEXPAND);

    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

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

BasicDrawPane::BasicDrawPane(wxFrame* parent) :
    wxPanel(parent)
{
}

void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void BasicDrawPane::render(wxDC&  dc)
{
    // draw some text
    dc.DrawText(wxT("Testing"), 40, 60);

    // draw a circle
    dc.SetBrush(*wxGREEN_BRUSH); // green filling
    dc.SetPen(wxPen(wxColor(255, 0, 0), 5)); // 5-pixels-thick red outline
    dc.DrawCircle(wxPoint(200, 100), 25 /* radius */);

    // draw a rectangle
    dc.SetBrush(*wxBLUE_BRUSH); // blue filling
    dc.SetPen(wxPen(wxColor(255, 175, 175), 10)); // 10-pixels-thick pink outline
    dc.DrawRectangle(300, 100, 400, 200);

    // draw a line
    dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
    dc.DrawLine(300, 100, 700, 300); // draw line across the rectangle

    cairo_t * cairoCtx = (cairo_t*)dc.GetImpl()->GetCairoContext();
    ::cairo_move_to(cairoCtx, 100, 100);
    ::cairo_line_to(cairoCtx, 500, 500);
    ::cairo_close_path(cairoCtx);
    ::cairo_stroke(cairoCtx);

    auto crScr = std::make_shared<Cairo::Context>((cairo_t *)cairoCtx);
    crScr->move_to(100, 200);
    crScr->line_to(300, 300);
    crScr->stroke();

    auto ctx = Cairo::make_refptr_for_instance(new Cairo::Context((cairo_t *)cairoCtx));
    ctx->move_to(200, 500);
    ctx->line_to(600, 700);
    ctx->stroke();
}
