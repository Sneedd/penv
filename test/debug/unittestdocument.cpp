/*
 * unittestdocument.h - Declaration of the UnitTestDocument class
 *
 * Author: Steffen Ott
 * Date: 05/18/2008
 *
 */



#include "unittestdocument.h"


//----------------------------------------------------------------
UnitTestDocument::UnitTestDocument()
    : penv::WindowDocument(_T("TESTING_UNITTEST_DOCUMENT"))
/**
 * \brief
 **/
{
    m_unittests = NULL;
}



//----------------------------------------------------------------
UnitTestDocument::~UnitTestDocument()
/**
 * \brief
 **/
{
    if (m_unittests != NULL) delete m_unittests;
}


//----------------------------------------------------------------
penv::Window* UnitTestDocument::OnCreate(penv::ProjectItem* item)
/**
 * \brief
 **/
{
    UnitTestDocument* win = new UnitTestDocument();
    win->Initialize(_T("UnitTest Document"), item);
    win->InitializeUI();
    win->Layout();
    win->ConnectEvents();
    win->m_unittests = new UnitTestHashMap();
    win->SetupTestCases();
    win->DisplayTestCases();
    return (win);
}


//----------------------------------------------------------------
bool UnitTestDocument::OnInitializeUI(wxWindow* parent)
/**
 * \brief
 **/
{
    m_panel = new wxPanel(parent, wxID_ANY);
	m_listbox = new wxListBox(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        0, NULL, wxLB_EXTENDED);
	m_buttontestselected = new wxButton(m_panel, wxID_ANY, _T("Test selected"),
        wxDefaultPosition, wxDefaultSize, 0);
	m_buttontestall = new wxButton(m_panel, wxID_ANY, wxT("Test all"),
        wxDefaultPosition, wxDefaultSize, 0);
	m_htmlwin = new wxHtmlWindow(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxHW_SCROLLBAR_AUTO|wxSTATIC_BORDER);
    m_window = m_panel;
    return (true);
}


//----------------------------------------------------------------
bool UnitTestDocument::OnLayout()
/**
 * \brief
 **/
{
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer( 3, 1, 0, 0 );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 2 );
	sizer->SetFlexibleDirection( wxBOTH );
	sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizer->Add( m_listbox, 0, wxALL|wxEXPAND, 5 );
	wxFlexGridSizer* sizerbuttons;
	sizerbuttons = new wxFlexGridSizer( 2, 2, 0, 0 );
	sizerbuttons->SetFlexibleDirection( wxBOTH );
	sizerbuttons->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizerbuttons->Add( m_buttontestselected, 0, wxALL, 5 );
	sizerbuttons->Add( m_buttontestall, 0, wxALL, 5 );
	sizer->Add( sizerbuttons, 1, wxEXPAND, 5 );
	sizer->Add( m_htmlwin, 0, wxALL|wxEXPAND, 5 );
	m_panel->SetSizer( sizer );
	m_panel->Layout();
	return (true);
}


//----------------------------------------------------------------
void UnitTestDocument::ConnectEvents()
/**
 * \brief
 **/
{
	m_listbox->Connect(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,
        wxCommandEventHandler(UnitTestDocument::OnListBoxDoubleClick), NULL, this);
	m_buttontestselected->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(UnitTestDocument::OnButtonTestSelectedClick), NULL, this);
	m_buttontestall->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(UnitTestDocument::OnButtonTestAllClick), NULL, this);
}


//----------------------------------------------------------------
void UnitTestDocument::OnListBoxDoubleClick(wxCommandEvent& event)
/**
 * \brief
 **/
{
    OnButtonTestSelectedClick(event);
}


//----------------------------------------------------------------
void UnitTestDocument::OnButtonTestSelectedClick(wxCommandEvent& event)
/**
 * \brief
 **/
{
    ClearHtml();
    for (size_t i=0; i<m_listbox->GetCount(); ++i)
    {
        if (m_listbox->IsSelected(i))
        {
            wxString name = m_listbox->GetString(i);
            wxString header = _T("<hr><h2>");
            header.Append(name).Append(_T("</h2>"));
            AppendHtml(header);
            (this->*((*m_unittests)[name]))();
        }
    }
    ShowHtmlInfo();
}


//----------------------------------------------------------------
void UnitTestDocument::OnButtonTestAllClick(wxCommandEvent& event)
/**
 * \brief
 **/
{
    ClearHtml();
    UnitTestHashMap::iterator itr;
    for (itr = m_unittests->begin(); itr != m_unittests->end(); ++itr)
    {
        wxString header = _T("<hr><h2>");
        header.Append(itr->first).Append(_T("</h2>"));
        AppendHtml(header);
        (this->*(itr->second))();
    }
    ShowHtmlInfo();
}




//----------------------------------------------------------------
void UnitTestDocument::AppendHtml(const wxString& strg)
/**
 * \brief
 **/
{
    m_htmltext.Append(strg).Append(_T("<br>"));
}



//----------------------------------------------------------------
void UnitTestDocument::AppendHtml(const wxString& call, const wxString& result)
/**
 * \brief
 **/
{
    m_htmltext.Append(_T("<font face=\"courier new\">Call: &nbsp;&nbsp;")).Append(call).Append(_T("</font><br>"));
    m_htmltext.Append(_T("<font face=\"courier new\">Result: ")).Append(result).Append(_T("</font><br>"));
}


//----------------------------------------------------------------
void UnitTestDocument::AppendHtml(bool resultcorrect)
/**
 * \brief
 **/
{
    if (resultcorrect) {
        AppendHtml(wxString(_T("<b>Correct result.</b><br>")));
    } else {
        AppendHtml(wxString(_T("<b><font color=\"#FF0000\">Incorrect result.</font></b><br>")));
    }
}



//----------------------------------------------------------------
void UnitTestDocument::ClearHtml()
/**
 * \brief
 **/
{
    m_htmltext = _T("");
}


//----------------------------------------------------------------
void UnitTestDocument::ShowHtmlInfo()
/**
 * \brief
 **/
{
    wxString html = _T("<html><body>");
    html.Append(m_htmltext);
    html.Append(_T("</body></html>"));
    m_htmlwin->SetPage(html);
}



//----------------------------------------------------------------
void UnitTestDocument::DisplayTestCases()
/**
 * \brief
 **/
{
    UnitTestHashMap::iterator itr;
    for (itr = m_unittests->begin(); itr != m_unittests->end(); ++itr)
    {
        m_listbox->Insert(itr->first, 0);
    }
}


//----------------------------------------------------------------
void UnitTestDocument::AddTest(const wxString& name, UnitTestFunction function)
/**
 * \brief
 **/
{
    if (unlikely(m_unittests->find(name) != m_unittests->end())) {
        wxLogError(_T("[UnitTestDocument::AddTest] Cannot add unit test a test with that name '%s' already exists."), name.c_str());
        return;
    }
    (*m_unittests)[name] = function;
}


