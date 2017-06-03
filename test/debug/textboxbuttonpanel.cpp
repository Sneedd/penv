/*
 * textboxbuttonpanel.cpp - Implementation of the TextBoxButtonPanel class
 *
 * Author: Steffen Ott
 * Date: 04/08/2008
 *
 */

#include "textboxbuttonpanel.h"



//----------------------------------------------------------------
TextBoxButtonPanel::TextBoxButtonPanel(wxWindow* parent, int id)
    : wxPanel(parent, id)
/**
 * \brief Constructor.
 **/
{
}

//----------------------------------------------------------------
TextBoxButtonPanel::~TextBoxButtonPanel()
/**
 * \brief
 **/
{
}

//----------------------------------------------------------------
void TextBoxButtonPanel::InitializeUI(const wxString& buttontext)
/**
 * \brief Initializeses the UI components.
 **/
{
	m_textctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    m_button = new wxButton(this, wxID_ANY, buttontext,
        wxDefaultPosition, wxDefaultSize, 0);
    m_button->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(TextBoxButtonPanel::OnButtonClick), NULL, this);
}


//----------------------------------------------------------------
void TextBoxButtonPanel::DoLayout()
/**
 * \brief Updates the layout.
 **/
{
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer(2, 1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizer->Add(m_textctrl, 0, wxALL|wxEXPAND, 5);
	sizer->Add(m_button, 0, wxALL|wxEXPAND, 5);
	this->SetSizer(sizer);
    this->Layout();
}



//----------------------------------------------------------------
void TextBoxButtonPanel::OnButtonClick(wxCommandEvent& event)
/**
 * \brief Executed when clicked an refresh.
 **/
{
}



//----------------------------------------------------------------
void TextBoxButtonPanel::WriteLine(const wxString& string)
/**
 * \brief Writes a line into the text ctrl.
 * \param string Linte to write.
 **/
{
    wxString strg = m_textctrl->GetValue();
    strg.Append(string).Append(_T("\n"));
    m_textctrl->SetValue(strg);
}

//----------------------------------------------------------------
void TextBoxButtonPanel::ClearTextbox()
/**
 * \brief Clears the textbox.
 **/
{
    m_textctrl->SetValue(wxEmptyString);
}

