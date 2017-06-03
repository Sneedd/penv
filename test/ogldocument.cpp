/*
 * ogldocument.cpp - Implementation of the OGLDocument class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */


#include "ogldocument.h"


//----------------------------------------------------------------
OGLDocument::OGLDocument()
    : penv::WindowDocument(_T("MAIN_OGL_WINDOC"))
/**
 * \brief
 **/
{
}

//----------------------------------------------------------------
OGLDocument::~OGLDocument()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
penv::Window* OGLDocument::OnCreate(penv::ProjectItem* item)
/**
 * \brief
 **/
{
    OGLDocument* win = new OGLDocument();
    win->Initialize(_T("OglDocument"), item);
    win->InitializeUI();
    win->Layout();
    return (win);
}


//----------------------------------------------------------------
bool OGLDocument::OnInitializeUI(wxWindow* parent)
/**
 * \brief
 **/
{
    m_panel = new wxPanel(parent);
    m_canvas = new wxShapeCanvas(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSUNKEN_BORDER | wxRETAINED);
    m_canvas->SetDiagram(new wxDiagram());
    m_canvas->SetBackgroundColour(*wxWHITE);
	m_canvas->SetCursor(wxCursor(wxCURSOR_CROSS));

    // Add a shape, buggy without a EventHandler
//	wxCircleShape* shape = new wxCircleShape(20.0);
//	shape->SetX(25.0);
//	shape->SetY(25.0);
//	m_canvas->AddShape(shape);
//	m_canvas->GetDiagram()->ShowAll(1);

    m_window = (wxWindow*)m_panel;
    return (true);
}


//----------------------------------------------------------------
bool OGLDocument::OnLayout()
/**
 * \brief
 **/
{
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer(1, 1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizer->Add( m_canvas, 0, wxALL|wxEXPAND, 5 );
	m_panel->SetSizer(sizer);
	m_panel->Layout();
    return (true);
}


