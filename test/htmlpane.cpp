/*
 * htmlpane.cpp - Implementation of the HtmlPane class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */


#include "htmlpane.h"


//----------------------------------------------------------------
HtmlPane::HtmlPane()
    : WindowPane(_T("MAIN_HTML_WINPANE"))
/**
 * \brief
 **/
{
}



//----------------------------------------------------------------
HtmlPane::~HtmlPane()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
penv::Window* HtmlPane::OnCreate(penv::ProjectItem* item)
/**
 * \brief
 **/
{
    HtmlPane* win = new HtmlPane();
    win->Initialize(_T("Html Pane"), item);
    win->InitializeUI();
    win->Layout();
    win->SetPaneSize(wxSize(250,200));
    return (win);
}


//----------------------------------------------------------------
bool HtmlPane::OnInitializeUI(wxWindow* parent)
/**
 * \brief
 **/
{
    m_htmlwindow = new wxHtmlWindow(parent, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxHW_DEFAULT_STYLE);
    wxString htmltext = _T("<html>");
    htmltext.Append(_T("<head><title></title></head>"));
    htmltext.Append(_T("<body><b>Welcome<br>to penv Test</b><br><br>"));
    for (int i=0; i<1000; ++i) htmltext.Append(_T("bla "));
    htmltext.Append(_T("</body></html>"));
    m_htmlwindow->SetPage(htmltext);
    m_window = (wxWindow*)m_htmlwindow;
    return (true);
}


//----------------------------------------------------------------
bool HtmlPane::OnLayout()
/**
 * \brief
 **/
{
    m_window->Layout();
    return (true);
}


