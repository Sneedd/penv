/*
 * toolbarpane.cpp - Implementation of the ToolBarPane class
 *
 * Author: Steffen Ott
 * Date: 04/08/2008
 *
 */


#include "toolbarpane.h"


//----------------------------------------------------------------
ToolBarPane::ToolBarPane()
    : WindowPane(_T("TESTING_TOOLBAR_INFORMATION_PANE"))
/**
 * \brief Constructor.
 **/
{
}

//----------------------------------------------------------------
ToolBarPane::~ToolBarPane()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
penv::Window* ToolBarPane::OnCreate(penv::ProjectItem* item)
/**
 * \brief Creates a ToolBarPane object.
 **/
{
    ToolBarPane* win = new ToolBarPane();
    win->Initialize(_T("ToolBar Test Pane"), item);
    win->SetPaneSize(wxSize(300,-1));
    win->InitializeUI();
    win->Layout();
    return (win);
}




//----------------------------------------------------------------
bool ToolBarPane::OnInitializeUI(wxWindow* parent)
/**
 * \brief Initializeses the UI components.
 **/
{
    ToolBarPanel* panel = new ToolBarPanel(parent);
    panel->InitializeUI(_T("Refresh output of toolbars."));
    m_window = (wxWindow*)panel;
    return (true);
}



//----------------------------------------------------------------
bool ToolBarPane::OnLayout()
/**
 * \brief Updates the layout.
 **/
{
    ToolBarPanel* panel = (ToolBarPanel*)m_window;
    panel->DoLayout();
    return (true);
}

//================================================================
//----------------------------------------------------------------
void ToolBarPanel::OnButtonClick(wxCommandEvent& event)
/**
 * \brief Executed when clicked an refresh.
 **/
{
    ClearTextbox();
    penv::Environment* env = penv::Environment::Get();
    penv::ToolBarList* list = env->GetCommandList()->GetToolBarList();
    list->RefreshPaneInfos();
    wxArrayString* array = list->GetToolBars();
    for (size_t i=0; i<array->Count(); ++i)
    {
        penv::ToolBar* toolbar = list->GetToolBar((*array)[i]);
        WriteToolBar(toolbar, (*array)[i]);
    }
}


//----------------------------------------------------------------
void ToolBarPanel::WriteToolBar(penv::ToolBar* toolbar, const wxString& hashid)
/**
 * \brief Writes information about toolbar into textctrl.
 * \param toolbar ToolBar.
 * \param hashid Id of toolbar in hashtable.
 **/
{
    WriteLine(wxString((wxChar)'=', 80));
    wxString strg;
    strg.Append(wxString::Format(_T("ToolBar name = '%s'; auilabel = '%s'\n"), toolbar->GetName().c_str(), toolbar->GetPaneInfo().caption.c_str()));
    strg.Append(wxString::Format(_T("        id = '%s'; hashid = '%s'; auiname = '%s'\n"), toolbar->GetId().c_str(), hashid.c_str(), toolbar->GetPaneInfo().name.c_str()));
    strg.Append(wxString::Format(_T("        visible = '%s'; isFloating = '%s'\n"), penv::PenvHelper::CreateBoolean(toolbar->IsVisible()).c_str(), penv::PenvHelper::CreateBoolean(toolbar->GetPaneInfo().IsFloating()).c_str()));
    strg.Append(wxString::Format(_T("        wxToolBar = '%p'; DestroyOnClose = '%s'"), toolbar->GetToolBar(), penv::PenvHelper::CreateBoolean(toolbar->GetPaneInfo().IsDestroyOnClose()).c_str()));
    WriteLine(strg);
    WriteLine(wxString((wxChar)'-', 80));
    for (size_t i=0; i<toolbar->Count(); ++i)
    {
        penv::ToolBarItem* item = toolbar->GetItem(i);
        strg = wxEmptyString;
        if (item->IsSeparator()) {
            strg.Append(_T("Item Separator"));
        } else {
            strg.Append(wxString::Format(_T("Item name = '%s'; cmdid = '%s'; wxID = '%i'"), item->GetName().c_str(), item->GetId().c_str(), item->GetTool()->GetId()));
        }
        WriteLine(strg);
    }
}




