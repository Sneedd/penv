/*
 * scintilladocument.cpp - Implementation of the ScintillaDocument class
 *
 * Author: Steffen Ott
 * Date: 05/16/2008
 *
 */


#include "scintilladocument.h"


//----------------------------------------------------------------
ScintillaDocument::ScintillaDocument()
    : penv::WindowDocument(_T("EDITOR_SCINTILLA"))
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
ScintillaDocument::~ScintillaDocument()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
penv::Window* ScintillaDocument::OnCreate(penv::ProjectItem* item)
/**
 * \brief
 **/
{
    ScintillaDocument* win = new ScintillaDocument();
    wxString caption = _T("Scintilla");
    if (item != NULL) {
        caption = penv::Path::GetLastEntry(item->GetPathString());
    }
    win->Initialize(caption, item);
    win->InitializeUI();
    win->Layout();
    return (win);
}


//----------------------------------------------------------------
bool ScintillaDocument::OnInitializeUI(wxWindow* parent)
/**
 * \brief
 **/
{
    m_panel = new wxPanel(parent, wxID_ANY);
	m_scintilla = new wxScintilla(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

    wxFont font (10, wxTELETYPE, wxNORMAL, wxNORMAL);
    m_scintilla->StyleSetFont(wxSCI_STYLE_DEFAULT, font);
    m_scintilla->SetMarginWidth(0, m_scintilla->TextWidth(wxSCI_STYLE_LINENUMBER, _T("_999999")));
    m_scintilla->SetEdgeMode(wxSCI_EDGE_NONE); // wxSCI_EDGE_LINE
    m_scintilla->Connect(wxEVT_SCI_MODIFIED,
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxScintillaEventFunction, &ScintillaDocument::OnDocumentModified),
        NULL, this);
    m_window = m_panel;
    return (true);
}


//----------------------------------------------------------------
bool ScintillaDocument::OnLayout()
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
	sizer->Add(m_scintilla, 0, wxALL|wxEXPAND, 5);
	m_panel->SetSizer( sizer );
	m_panel->Layout();
    return (true);
}


//----------------------------------------------------------------
bool ScintillaDocument::OnLoad(const wxString& filename)
/**
 * \brief
 **/
{
    bool result = m_scintilla->LoadFile(filename);
    return (result);
}


//----------------------------------------------------------------
bool ScintillaDocument::OnSave(const wxString& filename)
/**
 * \brief
 **/
{
    //if (!(m_scintilla->GetModify() && !m_scintilla->GetReadOnly())) return (true);
    bool result = m_scintilla->SaveFile(filename);
    return (result);
}



//----------------------------------------------------------------
bool ScintillaDocument::OnUpdate()
/**
 * \brief
 **/
{
    return (true);
}



//----------------------------------------------------------------
void ScintillaDocument::OnPageAfterChange()
/**
 * \brief
 **/
{
    // Enable Commands
    penv::CommandList* list = penv::Environment::Get()->GetCommandList();
    list->GetCommand(_T("EDITOR_FILE_SAVE"))->Enable(true);
    list->GetCommand(_T("EDITOR_FILE_SAVEAS"))->Enable(true);
    list->GetCommand(_T("EDITOR_FILE_CLOSE"))->Enable(true);
    list->GetCommand(_T("EDITOR_ZOOM_OUT"))->Enable(true);
    list->GetCommand(_T("EDITOR_ZOOM_IN"))->Enable(true);
    // Update Window
    Update();
}



//----------------------------------------------------------------
void ScintillaDocument::OnPageBeforeHidden()
/**
 * \brief
 **/
{
    // Disable Commands
    penv::CommandList* list = penv::Environment::Get()->GetCommandList();
    list->GetCommand(_T("EDITOR_FILE_SAVE"))->Enable(false);
    list->GetCommand(_T("EDITOR_FILE_SAVEAS"))->Enable(false);
    list->GetCommand(_T("EDITOR_FILE_CLOSE"))->Enable(false);
    list->GetCommand(_T("EDITOR_ZOOM_OUT"))->Enable(false);
    list->GetCommand(_T("EDITOR_ZOOM_IN"))->Enable(false);
}


//----------------------------------------------------------------
void ScintillaDocument::OnDocumentModified(wxScintillaEvent& event)
/**
 * \brief
 **/
{
    this->Modified();
    //wxLogMessage(_T("modified : int type = %i"), event.GetModificationType());
}


//----------------------------------------------------------------
void ScintillaDocument::ZoomIn()
/**
 * \brief
 **/
{
    if (m_scintilla != NULL) m_scintilla->ZoomIn();
}


//----------------------------------------------------------------
void ScintillaDocument::ZoomOut()
/**
 * \brief
 **/
{
    if (m_scintilla != NULL) m_scintilla->ZoomOut();
}


