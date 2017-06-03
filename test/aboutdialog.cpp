/*
 * aboutdialog.cpp - Implementation of the AboutDialog class
 *
 * Author: Steffen Ott
 * Date: 05/16/2008
 *
 */



#include "aboutdialog.h"


//----------------------------------------------------------------
AboutDialog::AboutDialog(wxWindow* parent, int id)
    : wxDialog( parent, id, _("About ..."), wxDefaultPosition,
        wxSize( 470,320 ), wxDEFAULT_DIALOG_STYLE)
/**
 * \brief
 **/
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer( wxVERTICAL );

	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelabout = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* sizerabout;
	sizerabout = new wxFlexGridSizer( 1, 1, 0, 0 );
	sizerabout->AddGrowableCol( 0 );
	sizerabout->AddGrowableRow( 0 );
	sizerabout->SetFlexibleDirection( wxBOTH );
	sizerabout->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_htmlwin = new wxHtmlWindow( m_panelabout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
	sizerabout->Add( m_htmlwin, 0, wxALL|wxEXPAND, 5 );

	m_panelabout->SetSizer( sizerabout );
	m_panelabout->Layout();
	sizerabout->Fit( m_panelabout );
	m_notebook->AddPage( m_panelabout, wxT("About"), true );
	m_panelpackages = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* sizerpackages;
	sizerpackages = new wxFlexGridSizer( 1, 1, 0, 0 );
	sizerpackages->AddGrowableCol( 0 );
	sizerpackages->AddGrowableRow( 0 );
	sizerpackages->SetFlexibleDirection( wxBOTH );
	sizerpackages->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_textctrl = new wxTextCtrl( m_panelpackages, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE  );
	sizerpackages->Add( m_textctrl, 0, wxALL|wxEXPAND, 5 );

	m_panelpackages->SetSizer( sizerpackages );
	m_panelpackages->Layout();
	sizerpackages->Fit( m_panelpackages );
	m_notebook->AddPage( m_panelpackages, wxT("Packages"), false );

	sizer->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );

	m_buttonclose = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( m_buttonclose, 0, wxALIGN_RIGHT|wxALL, 5 );

	this->SetSizer( sizer );
	this->Layout();

	// Connect Events
	m_buttonclose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialog::OnButtonCloseClick ), NULL, this );



    // Fill the control content
    wxString htmltext = _T("<html>");
    htmltext.Append(_T("<head><title></title></head>"));
    htmltext.Append(_T("<body><b>Welcome<br>to the penv test application</b><br><br>"));
    htmltext.Append(_T("This application is for testing and representation purposes only. "));
    htmltext.Append(_T("But we're glad that you are interesting in the penv framework.<br>"));
    htmltext.Append(_T("The penv framework is created by <br>Steffen Ott and Michael Schwierz.<br>"));
    htmltext.Append(_T(""));
    htmltext.Append(_T(""));
    htmltext.Append(_T(""));
    htmltext.Append(_T("</body></html>"));
    m_htmlwin->SetPage(htmltext);

    wxString pkgtext = _T("Initialized Packages:\n");
    penv::PackageList* list = penv::Environment::Get()->GetPackageList();
    wxArrayString* array = list->GetPackages();
    for (size_t i=0; i<array->Count(); ++i)
    {
        penv::Package* pkg = list->GetItem((*array)[i]);
        pkgtext.Append(pkg->GetName()).Append(_T("  "))
            .Append(pkg->GetVersion().GetVersion()).Append(_T("\n"));
    }
    m_textctrl->SetValue(pkgtext);
    delete array;
}



//----------------------------------------------------------------
void AboutDialog::OnButtonCloseClick(wxCommandEvent& event)
/**
 * \brief
 **/
{
    this->Destroy();
}




