/*
 * applicationframe.cpp - Implementation of the ApplicationFrame class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#include "applicationframe.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"
#include "windowlist.h"


namespace penv {

//----------------------------------------------------------------
ApplicationFrame::ApplicationFrame()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
ApplicationFrame::ApplicationFrame(wxWindow* parent, const wxString& caption)
    : wxFrame(parent, wxID_ANY, caption, wxDefaultPosition, wxSize(800,600))
/**
 * \brief Constructor, initializes the ApplicationFrame object.
 * \param parent Parent window for application frame, always NULL.
 * \param caption Caption of the applciation frame.
 **/
{
    m_dbgwindow = NULL;
    m_msgwindow = NULL;
    m_prjtreectrl = NULL;
    m_prjtreectrlshown = false;

    InitalizeUI();
    DoLayout();
    ConnectEvents();
}


//----------------------------------------------------------------
ApplicationFrame::~ApplicationFrame()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
void ApplicationFrame::InitalizeUI()
/**
 * \brief Initialize all user interface components.
 **/
{
    // Initialisiere wxAuiManager
    m_manager = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
    if (m_manager == NULL) {
        wxLogError(_T("[ApplicationFrame::InitalizeUI] Could not initialize wxAuiManager."));
        wxASSERT(false);
        return;
    }

    // Initialisiere wxAuiNotebook
    m_notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER  | wxAUI_NB_WINDOWLIST_BUTTON );
    if (m_notebook == NULL) {
        wxLogError(_T("[ApplicationFrame::InitalizeUI] Could not initialize wxAuiNotebook."));
        wxASSERT(false);
        return;
    }

    // Initalisiere MenuBar und hinzufügen zum Frame
    m_menubar = new wxMenuBar();
    if (m_menubar != NULL) {
        this->SetMenuBar(m_menubar);
    } else {
        wxLogError(_T("[ClientGUIFrame::InitializeGUI] Could not get menu resource."));
        wxASSERT(false);
    }

    // Initalisiere Statusbar und hinzufügen zum Frame
    m_statusbar = new StatusBar(this, wxID_ANY);
    if (m_statusbar != NULL) {
        this->SetStatusBar(m_statusbar);
    } else {
        wxLogError(_T("[ClientGUIFrame::InitializeGUI] Could not initialize statusbar."));
        wxASSERT(false);
    }

    // Füge Notebook zum Aui Manager hinzu
    m_manager->AddPane(m_notebook, wxAuiPaneInfo().
        Name(_T("penv_auinotebook")).
        CenterPane().PaneBorder(true));

    #ifdef __DEBUG__
    // Initalisiere Nachrichtenfenster und DebugFenster
    ToogleMessageWindow();
    ToogleDebugWindow();
    #endif
}


//----------------------------------------------------------------
void ApplicationFrame::DoLayout()
/**
 * \brief Updates the layout of this window.
 **/
{
    m_manager->Update();
    this->Layout();
}




//----------------------------------------------------------------
void ApplicationFrame::ConnectEvents()
/**
 * \brief Connects events to the components in this window.
 **/
{
    Connect(wxEVT_CLOSE_WINDOW,
        wxCloseEventHandler(ApplicationFrame::OnClose));
    //m_manager->Connect(wxEVT_AUI_PANE_BUTTON,
    //    wxAuiManagerEventHandler(ApplicationFrame::OnAuiPaneButton), NULL, this);
    m_manager->Connect(wxEVT_AUI_PANE_CLOSE,
        wxAuiManagerEventHandler(ApplicationFrame::OnAuiPaneClose), NULL, this);
    //m_manager->Connect(wxEVT_AUI_PANE_MAXIMIZE,
    //    wxAuiManagerEventHandler(ApplicationFrame::OnAuiPaneMaximize), NULL, this);
    //m_manager->Connect(wxEVT_AUI_PANE_RESTORE,
    //    wxAuiManagerEventHandler(ApplicationFrame::OnAuiPaneRestore), NULL, this);
    //m_manager->Connect(wxEVT_AUI_RENDER,
    //    wxAuiManagerEventHandler(ApplicationFrame::OnAuiPaneRender), NULL, this);
    //m_manager->Connect(wxEVT_AUI_FIND_MANAGER,
    //    wxAuiManagerEventHandler(ApplicationFrame::OnAuiPaneFindManager), NULL, this);
	//m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_ALLOW_DND,
    //    wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookAllowDND), NULL, this);
	//m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_BEGIN_DRAG,
    //    wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookBeginDrag), NULL, this);
	//m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_BUTTON,
    //    wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookButton), NULL, this);
	//m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_DRAG_MOTION,
    //    wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookDragMotion), NULL, this);
	//m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_END_DRAG,
    //    wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookEndDrag), NULL, this);
	m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED,
        wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookPageChanged), NULL, this);
	m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING,
        wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookPageChanging), NULL, this);
	m_notebook->Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE,
        wxAuiNotebookEventHandler(ApplicationFrame::OnAuiNotebookPageClose), NULL, this);
}




//----------------------------------------------------------------
wxAuiManager* ApplicationFrame::GetManager()
/**
 * \brief Returns the aui frame manager.
 * \return Aui frame manager.
 **/
{
    return (m_manager);
}


//----------------------------------------------------------------
wxAuiNotebook* ApplicationFrame::GetNotebook()
/**
 * \brief Returns the center pane aui notebook.
 * \return Aui notebook.
 **/
{
    return (m_notebook);
}


//----------------------------------------------------------------
wxMenuBar* ApplicationFrame::GetMenuBar()
/**
 * \brief Returns the menubar of this frame.
 * \return Menubar of this frame.
 **/
{
    return (m_menubar);
}


//----------------------------------------------------------------
StatusBar* ApplicationFrame::GetStatusBar()
/**
 * \brief Returns the statusbar of this frame.
 * \return Statusbar of this frame.
 **/
{
    return (m_statusbar);
}


//----------------------------------------------------------------
DebugWindow* ApplicationFrame::GetDebugWindow()
/**
 * \brief Returns the debug window. Can return NULL if
 * window is not visible.
 * \return Debug window.
 **/
{
    return (m_dbgwindow);
}




//----------------------------------------------------------------
MessageWindow* ApplicationFrame::GetMessageWindow()
/**
 * \brief Returns the message window. Can return NULL if
 * window is not visible.
 * \return Message window.
 **/
{
    return (m_msgwindow);
}


//----------------------------------------------------------------
void ApplicationFrame::SetProjectTreeCtrl(ProjectTreeCtrl* treectrl)
/**
 * \brief Sets the project tree control. It's possible to override
 * the project tree control and give it your own functionality.
 * You can set the project tree control here, to override the default
 * implementation.
 * \param treectrl Project tree control.
 **/
{
    if (treectrl == NULL)
    {
        wxLogError(_T("[penv::ApplicationFrame::SetProjectTreeCtrl] Argument 'treectrl' is NULL."));
    }
    // Wenn das ProjectTreeCtrl angezeigt wird, dann muss es gelöscht werden
    if (m_prjtreectrlshown)
    {
        if (m_prjtreectrl == NULL)
        {
            m_prjtreectrl = treectrl;
        }
        else
        {
            m_manager->DetachPane(m_prjtreectrl);
            m_prjtreectrl->Close();
            delete m_prjtreectrl;
            m_prjtreectrl = NULL;
            m_manager->Update();
        }
        m_manager->AddPane(m_prjtreectrl, wxAuiPaneInfo().
            Name(_T("PENV_WINDOW_PROJECTTREECTRL")).Caption(_("Workspaces")).
            Dockable(true).Left().Dock().Movable(true).
            CloseButton(true).MaximizeButton(false).BestSize(250,-1));
    }
    else
    {
        delete m_prjtreectrl;
        m_prjtreectrl = NULL;
        m_prjtreectrl = treectrl;
    }
}


//----------------------------------------------------------------
ProjectTreeCtrl* ApplicationFrame::GetProjectTreeCtrl()
/**
 * \brief Returns the project tree control.
 * \return Project tree control.
 **/
{
    return (m_prjtreectrl);
}


//----------------------------------------------------------------
void ApplicationFrame::ToogleDebugWindow()
/**
 * \brief Toogles the debug window's visibility. The debug window
 * can only be shown or hide by the Application Frame class.
 **/
{
    if (m_dbgwindow == NULL)
    {
        m_dbgwindow = new DebugWindow(this, wxID_ANY);
        m_manager->AddPane(m_dbgwindow, wxAuiPaneInfo().
            Name(_T("PENV_WINDOW_DEBUG")).Caption(_("Debug")).
            Dockable(true).Bottom().Dock().Movable(true).
            CloseButton(true).MaximizeButton(true).BestSize(-1,250));
        m_manager->Update();
    }
    else
    {
        m_manager->DetachPane(m_dbgwindow);
        m_dbgwindow->Destroy();
        m_dbgwindow = NULL;
        m_manager->Update();
    }
}


//----------------------------------------------------------------
bool ApplicationFrame::IsDebugWindowVisible()
/**
 * \brief Checks if debug window is visible.
 * \return True if debug window is visible; false otherwise.
 **/
{
    return (m_dbgwindow != NULL);
}


//----------------------------------------------------------------
void ApplicationFrame::ToogleMessageWindow()
/**
 * \brief Toogles the message window's visibility. The message window
 * can only be shown or hide by the Application Frame class.
 **/
{
    if (m_msgwindow == NULL)
    {
        m_msgwindow = new MessageWindow(this, wxID_ANY);
        m_manager->AddPane(m_msgwindow, wxAuiPaneInfo().
            Name(_T("PENV_WINDOW_MESSAGE")).Caption(_("Messages")).
            Dockable(true).Bottom().Dock().Movable(true).
            CloseButton(true).MaximizeButton(true).BestSize(-1,250));
        m_manager->Update();
    }
    else
    {
        m_manager->DetachPane(m_msgwindow);
        m_msgwindow->Destroy();
        m_msgwindow = NULL;
        m_manager->Update();
    }
}


//----------------------------------------------------------------
bool ApplicationFrame::IsMessageWindowVisible()
/**
 * \brief Checks if message window is visible.
 * \return True if message window is visible; false otherwise.
 **/
{
    return (m_msgwindow != NULL);
}


//----------------------------------------------------------------
void ApplicationFrame::ToogleProjectTreeCtrl()
/**
 * \brief Toogles the project tree control's visibility. The project
 * tree control can only be shown or hide by the Application Frame class.
 **/
{
    if (m_prjtreectrlshown)
    {
        m_manager->DetachPane(m_prjtreectrl);
        m_prjtreectrl->Hide();
        m_prjtreectrlshown = false;
        m_manager->Update();
    }
    else
    {
        if (unlikely(m_prjtreectrl == NULL))
        {
            m_prjtreectrl = new ProjectTreeCtrl(this, wxID_ANY);
        }
        m_prjtreectrl->UpdateComponents();
        m_prjtreectrl->Show();
        m_manager->AddPane(m_prjtreectrl, wxAuiPaneInfo().
            Name(_T("PENV_WINDOW_PROJECTTREECTRL")).Caption(_("Workspaces")).
            Dockable(true).Left().Dock().Movable(true).
            CloseButton(true).MaximizeButton(false).BestSize(250,-1));
        m_manager->Update();
        m_prjtreectrlshown = true;
    }
}


//----------------------------------------------------------------
bool ApplicationFrame::IsProjectTreeCtrlVisible()
/**
 * \brief Checks if project tree control is visible.
 * \return True if project tree control is visible; false otherwise.
 **/
{
    return (m_prjtreectrlshown);
}





//----------------------------------------------------------------
void ApplicationFrame::OnClose(wxCloseEvent& event)
/**
 * \brief Occures if the applciation is closed. Saves configuration
 * and exits application.
 * \param event Event.
 **/
{
    // Die PaneInfos in den Panes (WindowPane und ToolBar)
    //  müssen upgedated werden. Damit ihre Positionen nach dem
    //  start wiederhergestellt werden können. Da in der App::OnExit()
    //  Methode nicht mehr die Positionen abgefragt werden können
    //  müssen sie in der jeweiligen Klasse gesichert werden!!
    Environment* env = Environment::Get();
    env->GetCommandList()->GetToolBarList()->RefreshPaneInfos();
    env->GetWindowList()->RefreshPaneInfos();

    if (!env->SaveConfiguration()) {
        wxLogError(_T("[penv::ApplicationFrame::OnClose] The application could not save the configuration file."));
    }

    // Close spezial windows first
    if (m_prjtreectrl != NULL) {
        if (IsProjectTreeCtrlVisible()) {
            m_manager->DetachPane(m_prjtreectrl);
        }
        m_prjtreectrl->Destroy();
        m_prjtreectrl = NULL;
    }
    if (m_dbgwindow != NULL) {
        if (IsDebugWindowVisible()) {
            m_manager->DetachPane(m_dbgwindow);
        }
        m_dbgwindow->Destroy();
        m_dbgwindow = NULL;
    }
    if (m_msgwindow != NULL) {
        if (IsMessageWindowVisible()) {
            m_manager->DetachPane(m_msgwindow);
        }
        m_msgwindow->Destroy();
        m_msgwindow = NULL;
    }
    // Close all windows
    env->GetWindowList()->CloseAll(false, false);
    // Destroy main window
    if (m_manager != NULL) m_manager->UnInit();
    Destroy();
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiPaneButton(wxAuiManagerEvent& event)
/**
 * \brief Occurs when a window pane button (like close, maximize, ...)
 * is pressed.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}



//----------------------------------------------------------------
void ApplicationFrame::OnAuiPaneClose(wxAuiManagerEvent& event)
/**
 * \brief Occurs when a window pane will be closed.
 **/
{
    wxAuiPaneInfo* paneinfo = event.GetPane();
    // First handle the spezial windows
    if (paneinfo->name == _T("PENV_WINDOW_PROJECTTREECTRL")) {
        // Name is defined in ToogleProjectTreeCtrl()
        event.Veto();
        ToogleProjectTreeCtrl();
        return;
    }
    else if (paneinfo->name == _T("PENV_WINDOW_DEBUG")) {
        // Name is defined in ToogleDebugWindow()
        event.Veto();
        ToogleDebugWindow();
        return;
    }
    else if (paneinfo->name == _T("PENV_WINDOW_MESSAGE")) {
        // Name is defined in ToogleMessageWindow()
        event.Veto();
        ToogleMessageWindow();
        return;
    }
    // Then check if pane is a toolbar or window
    else if (paneinfo->IsToolbar()) {
        ToolBar* toolbar = Environment::Get()->GetCommandList()->GetToolBarList()->GetToolBar(paneinfo->name);
        if (toolbar == NULL) {
            wxLogError(_T("[penv::ApplicationFrame::OnAuiPaneClose] Cannot find toolbar with the id '%s'."), paneinfo->name.c_str());
            return;
        }
        toolbar->Visible(false);
        event.Veto();
    }
    else
    {
        // Window wird geschlossen
        WindowList* winlist = Environment::Get()->GetWindowList();
        wxString id = paneinfo->name;
        if (!winlist->ExistsWindow(id)) {
            wxLogWarning(_T("[penv::ApplicationFrame::OnAuiNotebookPageClose] The window document could not be found in the windowlist."));
        } else {
            winlist->Close(id);
            event.Veto();
        }
    }
}



//----------------------------------------------------------------
void ApplicationFrame::OnAuiPaneMaximize(wxAuiManagerEvent& event)
/**
 * \brief Occurs when a window is maximized.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // NOTE: Maybe all pane informations should be stored and
    //       in OnAuiPaneRestore restored, because maximize and
    //       and minimize will result in a uncorrect behaivor for
    //       the windows. May a bug in wxAuiManager?
    // Not connected
}



//----------------------------------------------------------------
void ApplicationFrame::OnAuiPaneRestore(wxAuiManagerEvent& event)
/**
 * \brief Occurs when a window is restored from its maximized
 * information.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiPaneRender(wxAuiManagerEvent& event)
/**
 * \brief Occurs when the AuiMangers renders a window pane.
 * This event is not connected, but prepared for use.
 * \param event The aui manager event.
 **/
{
    // Not connected
}



//----------------------------------------------------------------
void ApplicationFrame::OnAuiPaneFindManager(wxAuiManagerEvent& event)
/**
 * \brief What does this event do? Find the manager? Will never be
 * called.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}



//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookAllowDND(wxAuiNotebookEvent& event)
/**
 * \brief Occurs when a notebook is asked to allow a drag and drop
 * operation.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookBeginDrag(wxAuiNotebookEvent& event)
/**
 * \brief Occurs when a notebook drag operation begins.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookButton(wxAuiNotebookEvent& event)
/**
 * \brief Occurs when a notebook button (like close, ...) is pressed
 * by the user.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookDragMotion(wxAuiNotebookEvent& event)
/**
 * \brief Occurs when a notebook page is draged over this notebook.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookEndDrag(wxAuiNotebookEvent& event)
/**
 * \brief Occurs when a notebook page is draged and released over this
 * notebook.
 * This event is not connected, but prepared for use.
 * \param event Event.
 **/
{
    // Not connected
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookPageChanged(wxAuiNotebookEvent& event)
/**
 * \brief Occurs when a notebook page has changed.
 * \param event Event.
 * \todo Optimize: Get Window Id and the call CallPageChanged()
 * because window id is needed in SearchItem().
 **/
{
    Environment* env = Environment::Get();
    // Check if selections exists in notebook, they do not exist
    //   if window is closed!
    int count = m_notebook->GetPageCount();
    wxWindow* selection_window = NULL;
    wxWindow* oldselection_window = NULL;
    if (likely(event.GetSelection() >= 0 && event.GetSelection() < count)) {
        selection_window = m_notebook->GetPage(event.GetSelection());
    }
    if (likely(event.GetOldSelection() >= 0 && event.GetOldSelection() < count)) {
        oldselection_window = m_notebook->GetPage(event.GetOldSelection());
    }
    env->GetWindowList()->CallPageChanged(selection_window, oldselection_window);
    // Setting the project item to bold
    if (m_prjtreectrlshown && m_prjtreectrl != NULL && selection_window != NULL) {
        wxString windowid = env->GetWindowList()->GetWindowId(selection_window);
        ProjectTreeItemId treeitemid = m_prjtreectrl->SearchItem(windowid);
        if (!treeitemid.IsOk()) {
            wxLogWarning(_T("[penv::ApplicationFrame::OnAuiNotebookPageChanged] "
                "A tree item with windowid '%s' could not be found."), windowid.c_str());
        } else {
            treeitemid.SetSelection(penvSM_SelectSingle);
        }
    }
    // Last skip the event because it is handled.
    event.Skip();
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookPageChanging(wxAuiNotebookEvent& event)
/**
 * \brief Occures when a notebook page begins to change.
 * \param event Event.
 **/
{
    // NOTE: AuiNotebookPageChanging is called twice, but without a
    //       skipping the event the Changed event is not called.
    //       Maybe a bug in WxAuiNotebook??
    Environment* env = Environment::Get();
    // Check if selections exists in notebook, they do not exist
    //   if window is closed!
    int count = m_notebook->GetPageCount();
    wxWindow* selection_window = NULL;
    wxWindow* oldselection_window = NULL;
    if (likely(event.GetSelection() >= 0 && event.GetSelection() < count)) {
        selection_window = m_notebook->GetPage(event.GetSelection());
    }
    if (likely(event.GetOldSelection() >= 0 && event.GetOldSelection() < count)) {
        oldselection_window = m_notebook->GetPage(event.GetOldSelection());
    }
    env->GetWindowList()->CallPageChanging(selection_window, oldselection_window);
    event.Skip();
}


//----------------------------------------------------------------
void ApplicationFrame::OnAuiNotebookPageClose(wxAuiNotebookEvent& event)
/**
 * \brief Occures when a notebook page will be closed.
 * \param event Event.
 **/
{
    int index = event.GetSelection();
    WindowList* winlist = Environment::Get()->GetWindowList();
    wxString id = winlist->GetWindowId(m_notebook->GetPage(index));
    if (id.IsEmpty()) {
        wxLogWarning(_T("[penv::ApplicationFrame::OnAuiNotebookPageClose] The window document could not be found in the windowlist."));
    } else {
        // Close the window
        winlist->Close(id);
        // Veto event because window will be closed by windowlist
        event.Veto();
    }
}




} // namespace penv





