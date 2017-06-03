/*
 * textdocument.cpp - Implementation of the TextDocument class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */


#include "textdocument.h"


//----------------------------------------------------------------
TextDocument::TextDocument()
    : penv::WindowDocument(_T("MAIN_TEXT_WINDOC"))
/**
 * \brief
 **/
{
}



//----------------------------------------------------------------
TextDocument::~TextDocument()
/**
 * \brief
 **/
{
}



//----------------------------------------------------------------
penv::Window* TextDocument::OnCreate(penv::ProjectItem* item)
/**
 * \brief
 **/
{
    TextDocument* win = new TextDocument();
    win->Initialize(_T("TextDocument"), item);
    win->InitializeUI();
    win->Layout();
    return (win);
}



//----------------------------------------------------------------
bool TextDocument::OnInitializeUI(wxWindow* parent)
/**
 * \brief
 **/
{
    wxPanel* panel = new wxPanel(parent);
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer(1, 1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	m_textctrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	sizer->Add( m_textctrl, 0, wxALL|wxEXPAND, 5 );
	panel->SetSizer(sizer);
	panel->Layout();
    m_window = (wxWindow*)panel;
    return (true);
}



//----------------------------------------------------------------
bool TextDocument::OnLayout()
/**
 * \brief
 **/
{
    m_window->Layout();
    return (true);
}




