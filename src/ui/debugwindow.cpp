/*
 * debugwindow.cpp - Implemenation of the DebugWindow class
 *
 * Author: Steffen Ott
 * Date: 09/20/2007
 *
 */


#include "debugwindow.h"



namespace penv {


//----------------------------------------------------------------
DebugWindow::DebugWindow()
/**
 * \brief Constructor (private).
 **/
{
}



//----------------------------------------------------------------
DebugWindow::DebugWindow(wxWindow* parent, int id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
/**
 * \brief Constructor, initializes the DebugWindow object.
 * \param parent Parent window.
 * \param id Window id.
 **/
{
    // Erstellt mit wxFormBuilder
	wxFlexGridSizer* m_sizer;
	m_sizer = new wxFlexGridSizer( 1, 1, 0, 0 );
	m_sizer->AddGrowableCol( 0 );
	m_sizer->AddGrowableRow( 0 );
	m_sizer->SetFlexibleDirection( wxBOTH );
	m_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	m_notebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxNO_BORDER);
	m_sizer->Add( m_notebook, 1, wxEXPAND | wxALL, 0 );
	this->SetSizer( m_sizer );
	this->Layout();

	// Seiten hinzufügen
	m_debugpagewindows = new DebugPageWindows(m_notebook);
    m_notebook->AddPage(m_debugpagewindows, _T("Windows"));
    m_debugpagecommands = new DebugPageCommands(m_notebook);
    m_notebook->AddPage(m_debugpagecommands, _T("Commands"));



}

//----------------------------------------------------------------
DebugWindow::~DebugWindow()
/**
 * \brief Destructor.
 **/
{
}



//----------------------------------------------------------------
void DebugWindow::RefreshWindows()
/**
 * \brief Refreshes the commands in the 'windows' tab.
 **/
{
    if (m_debugpagecommands == NULL) return;
    m_debugpagewindows->UpdateComponents();
}

//----------------------------------------------------------------
void DebugWindow::RefreshCommands()
/**
 * \brief Refreshes the commands in the 'commands' tab.
 **/
{
    if (m_debugpagecommands == NULL) return;
    m_debugpagecommands->UpdateComponents();
}



} // namespace penv


