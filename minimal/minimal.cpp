#include "wx/wxprec.h"
#include "wx/wx.h"
#include "wx/overlay.h"
#include <gtk/gtk.h>
#include <gmodule.h>
#include <glib/gprintf.h>
#include <cairomm/cairomm.h>

class BasicDrawPane : public wxPanel
{

public:
    BasicDrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);
    void OnLeftMouseDown(wxMouseEvent &e);
    void OnLeftMouseUp(wxMouseEvent &e);
    void OnMouseMove(wxMouseEvent &e);
    static wxRect GetRegularRectangle(const wxPoint &pt1, const wxPoint &pt2);
    wxPoint    m_lastPos;
    wxPoint    m_mousePos;
    wxPoint    m_mouseAnchor;
    bool m_rubberBanding{false};

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
    Bind(wxEVT_LEFT_DOWN, &BasicDrawPane::OnLeftMouseDown, this, wxID_ANY);
    Bind(wxEVT_LEFT_UP, &BasicDrawPane::OnLeftMouseUp, this, wxID_ANY);
    Bind(wxEVT_MOTION, &BasicDrawPane::OnMouseMove, this, wxID_ANY);
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
    if (m_rubberBanding)
    {
        wxPoint tlPos{ std::min(m_mouseAnchor.x, m_mousePos.x), std::min(m_mouseAnchor.y, m_mousePos.y) };
        wxPoint brPos{ std::max(m_mouseAnchor.x, m_mousePos.x), std::max(m_mouseAnchor.y, m_mousePos.y) };

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(wxPen(wxColor(255, 0, 255, 255), 1));
        dc.DrawRectangle(wxRect(tlPos, brPos));
    }
}

void BasicDrawPane::OnLeftMouseDown(wxMouseEvent &e)
{
    if (!HasCapture())
    {
        CaptureMouse();
    }

    m_rubberBanding = true;
    m_lastPos = e.GetPosition();
    m_mousePos = e.GetPosition();
    m_mouseAnchor = e.GetPosition();
}

void BasicDrawPane::OnLeftMouseUp(wxMouseEvent &e)
{
    m_rubberBanding = false;

    if (HasCapture())
    {
        ReleaseMouse();
    }

    m_lastPos = m_mousePos;
    m_mousePos = e.GetPosition();
    wxRect oldRect = GetRegularRectangle(m_mouseAnchor, m_lastPos);
    wxRect newRect = GetRegularRectangle(m_mouseAnchor, m_mousePos);

    wxRect invalidRect = oldRect.Union(newRect);
    Refresh(false, &invalidRect);
}

void BasicDrawPane::OnMouseMove(wxMouseEvent &e)
{
    m_lastPos = m_mousePos;
    m_mousePos = e.GetPosition();
    wxRect oldRect = GetRegularRectangle(m_mouseAnchor, m_lastPos);
    wxRect newRect = GetRegularRectangle(m_mouseAnchor, m_mousePos);

    wxRect invalidRect = oldRect.Union(newRect);
    Refresh(false, &invalidRect);
}

wxRect BasicDrawPane::GetRegularRectangle(const wxPoint &pt1, const wxPoint &pt2)
{
    wxPoint tlPos{ std::min(pt1.x, pt2.x), std::min(pt1.y, pt2.y) };
    wxPoint brPos{ std::max(pt1.x, pt2.x), std::max(pt1.y, pt2.y) };
    wxRect invalidRect(tlPos + wxPoint(-2, -2), brPos + wxPoint(2, 2));
    return invalidRect;
}
