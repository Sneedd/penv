/*
 * commanddialog.cpp - Implementation of the CommandDialog class
 *
 * Author: Steffen Ott
 * Date: 03/18/2007
 *
 */

#include "commanddialog.h"
#include "../penvhelper.h"
#include "commandtoolbarwindow.h"
#include "commandmenuwindow.h"
#include "commandshortcutwindow.h"


namespace penv {


//----------------------------------------------------------------
CommandDialog::CommandDialog()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
CommandDialog::CommandDialog(wxWindow* parent, int id, const wxString& title,
    wxPoint pos, wxSize size, int style)
    : wxDialog(parent, id, title, pos, size, style)
/**
 * \brief Constructor, initializes the CommandDialog object.
 **/
{
    InitializeUI();
    DoLayout();
}

//----------------------------------------------------------------
CommandDialog::~CommandDialog()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
void CommandDialog::SaveContent()
/**
 * \brief Saves the changed content into the appropriate classes.
 **/
{
    ((CommandMenuWindow*)m_panelmenubar)->SaveAndUpdate();
    ((CommandToolBarWindow*)m_paneltoolbars)->SaveAndUpdate();
    ((CommandShortcutWindow*)m_panelshortcuts)->SaveAndUpdate();
}


//----------------------------------------------------------------
void CommandDialog::InitializeUI()
/**
 * \brief Initializes the user interface components.
 **/
{
    // Create Notebook and Panels for it
	m_notebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_panelmenubar = new CommandMenuWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	m_paneltoolbars = new CommandToolBarWindow(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelshortcuts = new CommandShortcutWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );


    // Add Panels
	m_notebook->AddPage(m_panelmenubar, _("MenuBar"), true, wxNullBitmap);
	m_notebook->AddPage(m_paneltoolbars, _("ToolBars"), false, wxNullBitmap);
    m_notebook->AddPage(m_panelshortcuts, _("Shortcuts"), false, wxNullBitmap);

    // Create Button stuff
	m_buttonspanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_paneldummy = new wxPanel( m_buttonspanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_buttonok = new wxButton( m_buttonspanel, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttoncancel = new wxButton( m_buttonspanel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonaccept = new wxButton( m_buttonspanel, wxID_ANY, wxT("Accept"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonaccept->Enable( false );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CommandDialog::OnClose ) );
	m_buttonok->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(CommandDialog::OnButtonOkClick), NULL, this );
	m_buttoncancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(CommandDialog::OnButtonCancelClick), NULL, this );
	m_buttonaccept->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(CommandDialog::OnButtonAcceptClick), NULL, this );
}



//----------------------------------------------------------------
void CommandDialog::DoLayout()
/**
 * \brief Does the layout for this dialog.
 **/
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* sizermain;
	sizermain = new wxFlexGridSizer( 2, 1, 0, 0 );
	sizermain->AddGrowableCol( 0 );
	sizermain->AddGrowableRow( 0 );
	sizermain->SetFlexibleDirection( wxBOTH );
	sizermain->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizermain->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	sizermain->Add( m_buttonspanel, 1, wxEXPAND | wxALL, 5 );


	wxFlexGridSizer* sizerbuttons;
	sizerbuttons = new wxFlexGridSizer( 1, 4, 0, 0 );
	sizerbuttons->AddGrowableCol( 0 );
	sizerbuttons->AddGrowableRow( 0 );
	sizerbuttons->SetFlexibleDirection( wxBOTH );
	sizerbuttons->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizerbuttons->Add( m_paneldummy, 1, wxEXPAND | wxALL, 5 );
	sizerbuttons->Add( m_buttonok, 0, wxALL, 5 );
	sizerbuttons->Add( m_buttoncancel, 0, wxALL, 5 );
	sizerbuttons->Add( m_buttonaccept, 0, wxALL, 5 );
	m_buttonspanel->SetSizer( sizerbuttons );
	m_buttonspanel->Layout();
	sizerbuttons->Fit( m_buttonspanel );


	this->SetSizer( sizermain );
	this->Layout();
}

//----------------------------------------------------------------
void CommandDialog::OnClose(wxCloseEvent& event)
/**
 * \brief Closes the dialog, sets the return code to cancel.
 * \param event CommandEvent.
 **/
{
    SetReturnCode(wxID_CANCEL);
    Destroy();
}

//----------------------------------------------------------------
void CommandDialog::OnButtonOkClick(wxCommandEvent& event)
/**
 * \brief Occurs when the Ok button is clicked. Saves and closes
 * the dialog.
 * \param event CommandEvent.
 **/
{
    SetReturnCode(wxID_OK);
    SaveContent();
    Destroy();
}


//----------------------------------------------------------------
void CommandDialog::OnButtonCancelClick(wxCommandEvent& event)
/**
 * \brief Occurs when the Cancel Button is clicked. Closes the Dialog.
 * \param event CommandEvent.
 **/
{
    SetReturnCode(wxID_CANCEL);
    Destroy();
}


//----------------------------------------------------------------
void CommandDialog::OnButtonAcceptClick(wxCommandEvent& event)
/**
 * \brief Occurs when the Accept Button is clicked. Saves the content.
 * \param event CommandEvent.
 **/
{
    SaveContent();
}


} // namespace penv



