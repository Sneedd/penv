/*
 * auiinfopane.cpp - Implementation of the AuiInfoPane class
 *
 * Author: Steffen Ott
 * Date: 04/23/2008
 *
 */


#include "auiinfopane.h"


//----------------------------------------------------------------
AuiInfoPane::AuiInfoPane()
    : penv::WindowPane(_T("TESTING_AUI_INFORMATION_PANE"))
/**
 * \brief Constructor.
 **/
{
}

//----------------------------------------------------------------
AuiInfoPane::~AuiInfoPane()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
penv::Window* AuiInfoPane::OnCreate(penv::ProjectItem* item)
/**
 * \brief Creates a ToolBarPane object.
 **/
{
    AuiInfoPane* win = new AuiInfoPane();
    win->Initialize(_T("Aui Info Pane"), item);
    win->SetPaneSize(wxSize(300,-1));
    win->InitializeUI();
    win->Layout();
    return (win);
}




//----------------------------------------------------------------
bool AuiInfoPane::OnInitializeUI(wxWindow* parent)
/**
 * \brief Initializeses the UI components.
 **/
{
    AuiInfoPanePanel* panel = new AuiInfoPanePanel(parent);
    panel->InitializeUI(_T("Refresh output of toolbars."));
    m_window = (wxWindow*)panel;
    return (true);
}



//----------------------------------------------------------------
bool AuiInfoPane::OnLayout()
/**
 * \brief Updates the layout.
 **/
{
    AuiInfoPanePanel* panel = (AuiInfoPanePanel*)m_window;
    panel->DoLayout();
    return (true);
}

//================================================================
//----------------------------------------------------------------
void AuiInfoPanePanel::OnButtonClick(wxCommandEvent& event)
/**
 * \brief Executed when clicked an refresh.
 **/
{
    ClearTextbox();
    penv::Environment* env = penv::Environment::Get();
    wxAuiNotebook* notebook = env->GetFrame()->GetNotebook();
    WriteLine(wxString((wxChar)'=', 80));
    WriteLine(wxString::Format(_T("wxAuiNotebook - Ptr: '%p'"), notebook));
    WriteLine(wxString((wxChar)'-', 80));
    for (size_t i=0; i<notebook->GetPageCount(); ++i)
    {
        WriteNotebookPageInfo(notebook, i);
    }
    wxAuiManager* manager = env->GetFrame()->GetManager();
    WriteLine(wxString((wxChar)'=', 80));
    WriteLine(wxString::Format(_T("wxAuiManager - Ptr: '%p'"), notebook));
    WriteLine(wxString((wxChar)'-', 80));
    for (size_t i=0; i<manager->GetAllPanes().Count(); ++i)
    {
        WritePaneInfo(manager->GetAllPanes()[i]);
    }
}


//----------------------------------------------------------------
void AuiInfoPanePanel::WriteNotebookPageInfo(wxAuiNotebook* notebook, size_t index)
/**
 * \brief Writes information about toolbar into textctrl.
 * \param toolbar ToolBar.
 * \param hashid Id of toolbar in hashtable.
 **/
{
    wxString boolean = penv::PenvHelper::CreateBoolean((int)index == notebook->GetSelection());
    WriteLine(wxString::Format(_T("Page = '%u'; Window Ptr = '%p'; Window Name = '%s'; IsSelected = '%s'"),
            index, notebook->GetPage(index), notebook->GetPageText(index).c_str(), boolean.c_str()));
}


//----------------------------------------------------------------
void AuiInfoPanePanel::WritePaneInfo(wxAuiPaneInfo& paneinfo)
/**
 * \brief Writes information about toolbar into textctrl.
 * \param toolbar ToolBar.
 * \param hashid Id of toolbar in hashtable.
 **/
{
    WriteLine(wxString::Format(_T("Pane name = '%s'; caption = '%s'; window ptr = '%p'"),
        paneinfo.name.c_str(), paneinfo.caption.c_str(), paneinfo.window));
    WriteLine(wxString::Format(_T("\tdock dir = '%i'; layer = '%i'; row = '%i'; pos = '%i', porportion = '%i'"),
        paneinfo.dock_direction, paneinfo.dock_layer, paneinfo.dock_row, paneinfo.dock_pos, paneinfo.dock_proportion));
    WriteLine(wxString::Format(_T("\tbest size width = '%i'; height = '%i'"),
        paneinfo.best_size.GetWidth(), paneinfo.best_size.GetHeight()));
    WriteLine(wxString::Format(_T("\tmin size width = '%i'; height = '%i'"),
        paneinfo.min_size.GetWidth(), paneinfo.min_size.GetHeight()));
    WriteLine(wxString::Format(_T("\tmax size width = '%i'; height = '%i'"),
        paneinfo.max_size.GetWidth(), paneinfo.max_size.GetHeight()));
    WriteLine(wxString::Format(_T("\tfloating pos x = '%i'; y = '%i'; size width = '%i'; height = '%i'"),
        paneinfo.floating_pos.x, paneinfo.floating_pos.y, paneinfo.floating_size.GetWidth(), paneinfo.floating_size.GetHeight()));
    wxString states;
    if (paneinfo.HasFlag(1 << 0)) states.Append(_T("Floating "));
    if (paneinfo.HasFlag(1 << 1)) states.Append(_T("Hidden "));
    if (paneinfo.HasFlag(1 << 2)) states.Append(_T("LeftDockable "));
    if (paneinfo.HasFlag(1 << 3)) states.Append(_T("RightDockable "));
    if (paneinfo.HasFlag(1 << 4)) states.Append(_T("TopDockable "));
    if (paneinfo.HasFlag(1 << 5)) states.Append(_T("BottomDockable "));
    if (paneinfo.HasFlag(1 << 6)) states.Append(_T("Floatable "));
    if (paneinfo.HasFlag(1 << 7)) states.Append(_T("Movable "));
    if (paneinfo.HasFlag(1 << 8)) states.Append(_T("Resizable "));
    if (paneinfo.HasFlag(1 << 9)) states.Append(_T("PaneBorder "));
    if (paneinfo.HasFlag(1 << 10)) states.Append(_T("Caption "));
    if (paneinfo.HasFlag(1 << 11)) states.Append(_T("Gripper "));
    if (paneinfo.HasFlag(1 << 12)) states.Append(_T("DestroyOnClose "));
    if (paneinfo.HasFlag(1 << 13)) states.Append(_T("Toolbar "));
    if (paneinfo.HasFlag(1 << 14)) states.Append(_T("Active "));
    if (paneinfo.HasFlag(1 << 15)) states.Append(_T("GripperTop "));
    if (paneinfo.HasFlag(1 << 16)) states.Append(_T("Maximized "));

    if (paneinfo.HasFlag(1 << 21)) states.Append(_T("CloseButton "));
    if (paneinfo.HasFlag(1 << 22)) states.Append(_T("MaximizeButton "));
    if (paneinfo.HasFlag(1 << 23)) states.Append(_T("MinimizeButton "));
    if (paneinfo.HasFlag(1 << 24)) states.Append(_T("PinButton "));

    if (paneinfo.HasFlag(1 << 26)) states.Append(_T("Custom1 "));
    if (paneinfo.HasFlag(1 << 27)) states.Append(_T("Custom1 "));
    if (paneinfo.HasFlag(1 << 28)) states.Append(_T("Custom1 "));

    if (paneinfo.HasFlag(1 << 30)) states.Append(_T("SavedHiddenState "));
    if (paneinfo.HasFlag(1 << 31)) states.Append(_T("ActionPane "));
    WriteLine(wxString::Format(_T("\t%s"), states.c_str()));
}



