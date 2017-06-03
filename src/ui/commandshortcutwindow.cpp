/*
 * commandshortcutwindow.h - Declaration of the CommandShortcutWindow class
 *
 * Author: Steffen Ott
 * Date: 04/23/2008
 *
 */

#include "commandshortcutwindow.h"
#include "../penvhelper.h"


namespace penv {


//----------------------------------------------------------------
CommandShortcutWindow::CommandShortcutWindow()
    : wxPanel()
/**
 * \brief Constructor (private).
 **/
{
}



//----------------------------------------------------------------
CommandShortcutWindow::CommandShortcutWindow(wxWindow* parent, int id, wxPoint pos, wxSize size, int style)
    : wxPanel(parent, id, pos, size, style)
/**
 * \brief
 **/
{
    InitializeUI();
    DoLayout();
}



//----------------------------------------------------------------
CommandShortcutWindow::~CommandShortcutWindow()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
void CommandShortcutWindow::InitializeUI()
/**
 * \brief
 **/
{
	m_labelnotimplemented = new wxStaticText(this, wxID_ANY, _("Not implemented yet."), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelnotimplemented->Wrap(-1);
}


//----------------------------------------------------------------
void CommandShortcutWindow::DoLayout()
/**
 * \brief
 **/
{
	wxFlexGridSizer* sizermessage;
	sizermessage = new wxFlexGridSizer( 1, 1, 0, 0 );
	sizermessage->AddGrowableCol( 0 );
	sizermessage->AddGrowableRow( 0 );
	sizermessage->SetFlexibleDirection( wxBOTH );
	sizermessage->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizermessage->Add( m_labelnotimplemented, 0, wxALIGN_CENTER|wxALL, 5 );
	this->SetSizer( sizermessage );
	this->Layout();
	sizermessage->Fit(this);
}


//----------------------------------------------------------------
void CommandShortcutWindow::ConnectEvents()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
void CommandShortcutWindow::UpdateComponents()
/**
 * \brief
 **/
{
}




//----------------------------------------------------------------
void CommandShortcutWindow::SaveAndUpdate()
/**
 * \brief Saves and updates the adjusted shortcuts.
 **/
{
    NOT_IMPLEMENTED_YET();
}






} // namespace penv



