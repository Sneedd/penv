/*
 * failureinfodialog.cpp - Implementation of the FailureInfoDialog class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */


#include "failureinfodialog.h"
#include "messagewindow.h"

namespace penv {


//----------------------------------------------------------------
FailureInfoDialog::FailureInfoDialog()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
FailureInfoDialog::FailureInfoDialog(wxWindow* parent, int id, const wxString& message,
    const wxString& shortmessage, const wxString& caption)
    : wxDialog(parent, id, caption, wxDefaultPosition, wxSize(480, 300))
/**
 * \brief
 **/
{
    m_messageshort = shortmessage;
    m_message = message;
    InitializeUI();
    DoLayout();
    ConnectEvents();
}

//----------------------------------------------------------------
FailureInfoDialog::~FailureInfoDialog()
/**
 * \brief
 **/
{
}

//----------------------------------------------------------------
void FailureInfoDialog::InitializeUI()
/**
 * \brief
 **/
{
	m_labeltitel = new wxStaticText(this, wxID_ANY, m_messageshort);
	m_labeltitel->Wrap(-1);
	m_labeltitel->SetFont(wxFont( 13, 70, 90, 92, false, wxEmptyString));
	m_panel = new MessageWindow(this, wxID_ANY);
	m_buttonclose = new wxButton(this, wxID_ANY, _("Close"));
	m_labelmessage = new wxStaticText(this, wxID_ANY, m_message);
	m_labelmessage->Wrap(470);
}

//----------------------------------------------------------------
void FailureInfoDialog::DoLayout()
/**
 * \brief
 **/
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer( 4, 1, 0, 0 );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 2 );
	sizer->SetFlexibleDirection( wxBOTH );
	sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizer->Add( m_labeltitel, 0, wxALL|wxEXPAND, 5 );
	sizer->Add( m_labelmessage, 0, wxALL|wxEXPAND, 5 );
	sizer->Add( m_panel, 1, wxEXPAND | wxALL, 5 );
	sizer->Add( m_buttonclose, 0, wxALIGN_RIGHT|wxALL, 5 );
	this->SetSizer( sizer );
	this->Layout();
}

//----------------------------------------------------------------
void FailureInfoDialog::ConnectEvents()
/**
 * \brief
 **/
{
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(FailureInfoDialog::OnClose));
	this->Connect(wxEVT_SIZE, wxSizeEventHandler(FailureInfoDialog::OnSize));
	m_buttonclose->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(FailureInfoDialog::OnButtonCloseClick), NULL, this);
}

//----------------------------------------------------------------
void FailureInfoDialog::OnClose(wxCloseEvent& event)
/**
 * \brief
 **/
{
    this->Destroy();
}

//----------------------------------------------------------------
void FailureInfoDialog::OnSize(wxSizeEvent& event)
/**
 * \brief
 **/
{
    m_labelmessage->Wrap(this->GetSize().GetWidth()-10);
}

//----------------------------------------------------------------
void FailureInfoDialog::OnButtonCloseClick(wxCommandEvent& event)
/**
 * \brief
 **/
{
    this->Destroy();
}


} // namespace penv


