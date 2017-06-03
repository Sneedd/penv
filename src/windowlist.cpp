/*
 * windowlist.cpp - Implementation of the WindowList class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#include "windowlist.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"

namespace penv {


//----------------------------------------------------------------
WindowList::WindowList()
/**
 * \brief Constructor (private).
 **/
{
    m_registered = NULL;
    m_openlist = NULL;
    m_manager = NULL;
    m_notebook = NULL;
    m_frame = NULL;
}


//----------------------------------------------------------------
WindowList::WindowList(const WindowList& list)
/**
 * \brief Copyconstructor (private).
 **/
{
    // Nothing to do
}


//----------------------------------------------------------------
WindowList& WindowList::operator=(const WindowList& list)
/**
 * \brief Privater Zuweisungsoperator.
 **/
{
    // Nothing to do
    return (*this);
}


//----------------------------------------------------------------
WindowList::WindowList(ApplicationFrame* frame, wxAuiManager* manager, wxAuiNotebook* notebook)
/**
 * \brief Constructor, initializes the WindowList object. The given parameters
 * are needed by the WindowList class, for managing the windows.
 * \param frame The top level window.
 * \param manager The docking manager.
 * \param notebook The central pane notebook.
 **/
{
    m_registered = new WindowsHashMap();
    m_openlist = new WindowsHashMap();
    m_manager = manager;
    m_notebook = notebook;
    m_frame = frame;
    m_empty = NULL;
    m_emptystring = wxEmptyString;
}


//----------------------------------------------------------------
WindowList::~WindowList()
/**
 * \brief Destructor.
 **/
{
    if (m_registered != NULL) delete m_registered;
    if (m_openlist != NULL) delete m_openlist;
}



//----------------------------------------------------------------
WindowDocument* WindowList::GetSelectedNotebookPage()
/**
 * \brief Returns the actual selected notebook page. This method
 * can return NULL, when no page is available.
 * \return Actual seclected notebook page.
 **/
{
    int selection = m_notebook->GetSelection();
    if (selection < 0) return (NULL);
    wxWindow* page = m_notebook->GetPage(selection);
    WindowsHashMap::iterator itr;
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        if (unlikely(page == itr->second->GetWindow())) {
            return ((WindowDocument*)itr->second);
        }
    }
    return (NULL);
}


//----------------------------------------------------------------
wxArrayString* WindowList::GetOpenWindows() const
/**
 * \brief Returns all <b>open</b> windows ids withhin a string array.
 * The array must be destroyed after use.
 * \return String array with open window ids.
 **/
{
    WindowsHashMap::iterator itr;
    wxArrayString* array = new wxArrayString();
    array->Alloc(m_openlist->size());
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        array->Add(itr->first);
    }
    return (array);
}



//----------------------------------------------------------------
Window* WindowList::GetWindow(const wxString& id)
/**
 * \brief Returns a <b>open</b> window by it's id.
 * \param id Open window id.
 * \return The open window if found; otherwise NULL.
 **/
{
    WindowsHashMap::iterator itr = m_openlist->find(id);
    if (unlikely(itr == m_openlist->end())) {
        return (NULL);
    }
    return (itr->second);
}


//----------------------------------------------------------------
const wxString& WindowList::GetWindowId(Window* window)
/**
 * \brief Returns the <b>open</b> window id of a open window pointer.
 * \param window Pointer of the open window.
 * \return Open window id if found; otherwise empty string.
 **/
{
    WindowsHashMap::iterator itr;
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        if (unlikely(window == itr->second)) {
            return (itr->first);
        }
    }
    return (m_emptystring);
}



//----------------------------------------------------------------
const wxString& WindowList::GetWindowId(wxWindow* window)
/**
 * \brief Returns the <b>open</b> window id of a given intenally open window pointer.
 * \param window Internally open window pointer.
 * \return Open window id if found; otherwise empty string.
 **/
{
    WindowsHashMap::iterator itr;
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        if (unlikely(window == itr->second->GetWindow())) {
            return (itr->first);
        }
    }
    return (m_emptystring);
}



//----------------------------------------------------------------
bool WindowList::Add(Window* window, bool show)
/**
 * \brief Adds a <b>open</b> window to the open windows. The best method to add a
 * new window is to use an open window id that is empty, so a new
 * unique id will be generated. If you want to show the window after
 * adding then simply set parameter show to true.
 * This method will generate a PENV_EVT_WINDOW_ADDED event
 * after successful adding of the window.
 * \param window Window which should be added.
 * \param show True if window should also be shown.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(window == NULL)) {
        wxLogError(_T("[penv::WindowList::Add] Window is NULL."));
        return (false);
    }
    // Testen ob eine ID generiert werden muss
    if (unlikely(window->m_id.IsEmpty())) {
        window->m_id = NewId();
    }
    // Überprüfen ob ID schon existiert
    WindowsHashMap::iterator itr = m_openlist->find(window->m_id);
    if (unlikely(itr != m_openlist->end())) {
        wxLogError(_T("[penv::WindowList::Add] Window ID %s already exists in WindowList."), window->m_id.c_str());
        return (false);
    }
    // Hinzufügen zur Hashmap
    (*m_openlist)[window->m_id] = window;
    // Prüfen ob Fenster in der Oberfläche angezeigt werden soll
    if (show) {
        switch (window->GetType())
        {
            case penvWT_Document:
                return (AddDocument((WindowDocument*)window, show));
            break;
            case penvWT_Pane:
                return (AddPane((WindowPane*)window, show));
            break;
            default:
            case penvWT_None:
                wxLogError(_T("[penv::WindowList::Add] Cannot show window because it is not a useable type."));
            break;
        }
    }
    return (false);
}


//----------------------------------------------------------------
bool WindowList::Remove(const wxString& id)
/**
 * \brief Removes a <b>open</b> window from the open window list.
 * This method is same as calling Close(), actually this
 * method is calling Close().
 * This method will generate a PENV_EVT_WINDOW_CLOSED event
 * after successful closing of the window.
 * \param id The open window id of the window which should be
 * removed.
 * \return True on success; false otherwise.
 **/
{
    return (Close(id));
}




//----------------------------------------------------------------
bool WindowList::ShowWindow(const wxString& id, bool show)
/**
 * \brief Shows the <b>open</b> window with the given open
 * window id or hides it. Heeds the window mapping of the window.
 * \param id Open window id of the window.
 * \param show True to show window; false to hide window.
 * \return True on success; false otherwise.
 **/
{
    WindowsHashMap::iterator itr = m_openlist->find(id);
    if (unlikely(itr == m_openlist->end()))
    {
        wxLogWarning(_T("[penv::WindowList::ShowWindow] Window Document cannot be shown, because the id %i does not exists."), id.c_str());
        return (false);
    }
    Window* window = itr->second;
    switch (window->m_type)
    {
        case penvWT_Document:
            ShowDocument((WindowDocument*)window, show);
        break;
        case penvWT_Pane:
            ShowPane((WindowPane*)window, show);
        break;
        case penvWT_None:
            wxLogError(_T("[penv::WindowList::ShowWindow] Cannot show window because it is not a useable type."));
            return (false);
        break;
    }
    return (true);
}

//----------------------------------------------------------------
bool WindowList::ExistsWindow(const wxString& id)
/**
 * \brief Checks if an <b>open</b> window with this id is already opend
 * and returns true when it exists.
 * \param id Opened window id.
 * \return True if opend window already exists; false otherwise.
 **/
{
    return (GetWindow(id) != NULL);
}


//----------------------------------------------------------------
bool WindowList::ChangeWindowIds(const wxString& oldid, const wxString& newid)
/**
 * \brief Changes the <b>open</b> window id and updates the open window
 * list.
 * \param oldid This is the old id of the window.
 * \param newid This is the new id of the window.
 * \return True on success; false otherwise.
 **/
{
    Window* window = GetWindow(oldid);
    if (window == NULL)
    {
        wxLogError(_T("[penv::WindowList::ChangeWindowIds] The window id '%s' does not exists."), oldid.c_str());
        return (false);
    }
    if (ExistsWindow(newid))
    {
        wxLogError(_T("[penv::WindowList::ChangeWindowIds] The new window id '%s' already exists."), newid.c_str());
        return (false);
    }
    // Fenster aus der hash tabelle entfernen
    if (unlikely(m_openlist->erase(oldid) != 1)) {
        wxLogError(_T("[penv::WindowList::ChangeWindowIds] Cannot remove window type with id \"%s\" from registered window container."), oldid.c_str());
        return (false);
    }
    // Fenster mit neuer id einfügen.
    window->m_id = newid;  // Nicht über SetId() realisierbar!
    (*m_openlist)[window->m_id] = window;
    return (true);
}



//----------------------------------------------------------------
bool WindowList::Clear()
/**
 * \brief Clears all <b>open</b> windows. It is the same as calling
 * CloseAll(false, false), actually this method calls CloseAll(false, false).
 * \return True on success; false otherwise.
 **/
{
    return (CloseAll(false, false));
}


//----------------------------------------------------------------
bool WindowList::CloseAll(bool documentsonly, bool canveto)
/**
 * \brief Closes all <b>open</b> windows. For every window which sould be closed
 * this method calls OnClose() from the window, so that the window can do
 * an action before closing. The closing can be vetoed if can vetoed is
 * true, then this method return false if there was a veto.
 * This method will generate a PENV_EVT_WINDOW_CLOSED event for every closed
 * window after successful closing of the window.
 * \param documentsonly True if only document windows should be closed.
 * \param canveto True if closing can be vetoed.
 * \return True on success; false otherwise.
 **/
{
    // Hole erst einmal alle geöffneten Fenster
    wxArrayString strgarray;
    WindowsHashMap::iterator itr;
    strgarray.Alloc(m_openlist->size());
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        wxString id = itr->first;
        Window* win = itr->second;
        // Wenn documentonly gesetzt ist, WindowPanes überspringen!!
        if (documentsonly && win->GetType() != penvWT_Document) continue;
        strgarray.Add(id);
    }
    // Loop through arrray and close all windows
    bool result = true;
    for (size_t i=0; i<strgarray.Count(); ++i)
    {
        wxString id = strgarray[i];
        Window* win = GetWindow(id);
        if (win == NULL) {
            wxLogError(_T("[penv::WindowList::CloseAll] Open window with id '%s' failed, because the window is already NULL."), id.c_str());
            continue;
        }
        // Store variant for later execution of inner closed event
        wxVariant variant(id);
        bool cancel = false;
        if (!win->OnClose(cancel))
        {
            wxLogError(_T("[penv::WindowList::CloseAll] On close failed for window '%s'."), id.c_str());
            result = false;
        }
        if (cancel && canveto) {
            m_manager->Update();
            wxLogWarning(_T("[penv::WindowList::CloseAll] Close all failed, because close has be vetoed by window '%s'."), id.c_str());
            return (false);
        }
        if (win->GetType() == penvWT_Document)
        {
            size_t index = m_notebook->GetPageIndex(win->GetWindow());
            if (unlikely(!m_notebook->DeletePage(index)))
            {
                wxLogError(_T("[penv::WindowList::CloseAll] Window document '%s' cannot be closed, because window does not exists in notebook."), id.c_str());
                result = false;
                continue;
            }
            win->SetWindow(NULL);
            delete win;
            // Remove window in hashmap
            if (unlikely(m_openlist->erase(id) != 1)) {
                wxLogError(_T("[penv::WindowList::CloseAll] Cannot remove window from hash map with id \"%s\" from open window container."), id.c_str());
            }
        }
        else if (win->GetType() == penvWT_Pane)
        {
            WindowPane* winpane = (WindowPane*)win;
            if (winpane->GetWindow() != NULL)
            {
                if (winpane->IsDestroyOnClose())
                {
                    if (winpane->IsVisible()) {
                        m_manager->DetachPane(winpane->GetWindow());
                    }
                    wxWindow* wxwin = winpane->GetWindow();
                    wxwin->Destroy();
                    winpane->SetWindow(NULL);
                    delete winpane;
                    // Remove window in hashmap
                    if (unlikely(m_openlist->erase(id) != 1)) {
                        wxLogError(_T("[penv::WindowList::CloseAll] Cannot remove window from hash map with id \"%s\" from open window container."), id.c_str());
                    }
                }
                else
                {
                    winpane->Visible(false);
                }
            }
        }
        else
        {
            wxLogError(_T("[penv::WindowList::CloseAll] The given window type cannot be closed."));
        }
        Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_WINDOW_CLOSED"), variant);
    }
    m_manager->Update();
    return (result);
}


//----------------------------------------------------------------
bool WindowList::Close(const wxString& id, bool canveto)
/**
 * \brief Closes an <b>open</b> window by its window id. The close
 * action can be veted if canveto is true.
 * This method will generate a PENV_EVT_WINDOW_CLOSED event
 * after successful closing of the window.
 * \param id Open window id of window to close.
 * \param canveto True if close can be vetoed; false otherwise.
 * \return True on success; false otherwise.
 **/
{
    // Get window and check for existance
    Window* win = this->GetWindow(id);
    if (unlikely(win == NULL))
    {
        wxLogError(_T("[penv::WindowList::Close] The window id '%s' does not exists."), id.c_str());
        return (false);
    }
    // Store id into variant for later user in window closed event
    wxVariant variant(win->GetId());
    // Call OnClose() for the window
    bool cancel = false;
    if (unlikely(!win->OnClose(cancel)))
    {
        wxLogError(_T("[penv::WindowList::Close] The window's '%s' OnClose() method returns 'false'."), id.c_str());
    }
    if (unlikely(cancel && canveto)) {
        wxLogWarning(_T("[penv::WindowList::Close] Close failed, because close has been vetoed by window '%s'."), id.c_str());
        return (false);
    }
    if (win->GetType() == penvWT_Document)
    {
        // Find notebook page and remove page
        size_t index = m_notebook->GetPageIndex(win->GetWindow());
        if (unlikely(!m_notebook->DeletePage(index)))
        {
            wxLogError(_T("[penv::WindowList::Close] Window document '%s' cannot be closed, because window does not exists in notebook."), id.c_str());
            return (false);
        }
        win->SetWindow(NULL);
        delete win;
        // Remove window in hashmap
        if (unlikely(m_openlist->erase(id) != 1)) {
            wxLogError(_T("[penv::WindowList::Close] Cannot remove window from hash map with id \"%s\" from open window container."), id.c_str());
            return (false);
        }
    }
    else if (win->GetType() == penvWT_Pane)
    {
        WindowPane* winpane = (WindowPane*)win;
        if (winpane->GetWindow() != NULL)
        {
            if (winpane->IsDestroyOnClose())
            {
                if (winpane->IsVisible()) {
                    m_manager->DetachPane(winpane->GetWindow());
                }
                wxWindow* wxwin = winpane->GetWindow();
                wxwin->Destroy();
                winpane->SetWindow(NULL);
                delete winpane;
                m_manager->Update();
                // Remove window in hashmap
                if (unlikely(m_openlist->erase(id) != 1)) {
                    wxLogError(_T("[penv::WindowList::Close] Cannot remove window from hash map with id \"%s\" from open window container."), id.c_str());
                    return (false);
                }
            }
            else
            {
                winpane->Visible(false);
            }
        }
    }
    else
    {
        wxLogError(_T("[penv::WindowList::Close] The given window type cannot be closed."));
    }
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_WINDOW_CLOSED"), variant);
    return (true);
}



//----------------------------------------------------------------
wxString WindowList::NewId()
/**
 * \brief Generates a new unique <b>open</b> window id.
 * \return Unique open window id.
 **/
{
    wxString uniqueid = PenvHelper::GenerateRandomHexValues(4);
    while (m_openlist->find(uniqueid) != m_openlist->end())
    {
        uniqueid = PenvHelper::GenerateRandomHexValues(4);
    }
    return (uniqueid);
}


//----------------------------------------------------------------
wxArrayString* WindowList::GetRegisteredWindows() const
/**
 * \brief Returns all <b>registered</b> window ids withhin a string array.
 * The array must be destroyed after use.
 * \return String array with registered window ids.
 **/
{
    WindowsHashMap::iterator itr;
    wxArrayString* array = new wxArrayString();
    array->Alloc(m_registered->size());
    for (itr = m_registered->begin(); itr != m_registered->end(); ++itr)
    {
        array->Add(itr->first);
    }
    return (array);
}



//----------------------------------------------------------------
bool WindowList::Register(Window* window)
/**
 * \brief Register a <b>registerd</b> window in this window
 * container. The registered window id or also class id called,
 * must be definied by the programmer and it must be unique.
 * A registered window will not shown in the user interface.
 * Before a window will be shown you must register that window
 * to the registered windows and then use the CreateWindow() and
 * Add() methods. See class description for a example.
 * This method will generate a PENV_EVT_WINDOW_REGISTERED event
 * after successful registration of the window.
 * \param window The window which should be registered.
 * \return True on success; false otherwise.
 **/
{
    // Fehler abfangen
    if (unlikely(window == NULL)) {
        wxLogError(_T("[penv::WindowList::Register] Cannot Register window, parameter argument is NULL."));
        return (false);
    }
    if (unlikely(m_registered->find(window->GetClassId()) != m_registered->end())) {
        wxLogError(_T("[penv::WindowList::Register] Cannot register window, a window with the id \"%s\" already exists."), window->GetId().c_str());
        return (false);
    }
    (*m_registered)[window->GetClassId()] = window;
    wxVariant variant(window->GetClassId());
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_WINDOW_REGISTERED"), variant);
    return (true);
}



//----------------------------------------------------------------
bool WindowList::Unregister(const wxString& classid)
/**
 * \brief Unregister a <b>registered</b> window by it's id form
 * this container.
 * This method will generate a PENV_EVT_WINDOW_UNREGISTERED event
 * after successful unregistration of the window.
 * \param classid The registered window id, which should be removed.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_registered->find(classid) == m_registered->end())) {
        wxLogError(_T("[penv::WindowList::Unregister] Cannot unregister window type, window with id \"%s\" does not exists in container."), classid.c_str());
        return (false);
    }
    // Fenster löschen
    if (unlikely(m_registered->erase(classid) != 1)) {
        wxLogError(_T("[penv::WindowList::Unregister] Cannot remove window type with id \"%s\" from registered window container."), classid.c_str());
        return (false);
    }
    wxVariant variant(classid);
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_WINDOW_UNREGISTERED"), variant);
    return (true);
}


//----------------------------------------------------------------
bool WindowList::IsRegistered(const wxString& classid)
/**
 * \brief Checks if a <b>registered</b> window is already registered.
 * \param classid The registered window id.
 * \return True if registered window exists; false otherwise.
 **/
{
    return (GetRegistered(classid) != NULL);
}


//----------------------------------------------------------------
Window* WindowList::GetRegistered(const wxString& classid)
/**
 * \brief Returns a <b>registered</b> window from the container.
 * \param classid The registered window id.
 * \return The registered window if found; otherwise NULL.
 **/
{
    WindowsHashMap::iterator itr;
    if (unlikely((itr = m_registered->find(classid)) == m_registered->end())) {
        return (NULL);
    }
    return (itr->second);
}

//----------------------------------------------------------------
Window* WindowList::CreateWindow(const wxString& classid, ProjectItem* item)
/**
 * \brief Creates a new window from a registered window, to add the
 * created window to the open windows. The returned window has a empty
 * open window id, which will be filled by the Add() method when this
 * window is added. Additionally you can connect a ProjectItem to the
 * window so this window can load data from the ProjectItem included
 * file. If you don't want such a connection just set the parameter item
 * to NULL.<br>
 * This method will call OnCreate() on the window to create a new instance
 * so this method must be implemented, in the inherited class in order
 * to create a new window.
 * \param classid Registered window id to create a new window.
 * \param item Connected project item for the newly created window.
 * \return The created window or NULL.
 **/
{
    if (unlikely(classid.IsEmpty())) return (NULL);
    WindowsHashMap::iterator itr = m_registered->find(classid);
    if (unlikely(itr == m_registered->end())) {
        wxLogError(_T("[penv::WindowList::CreateWindow] There is no registered window with the id \"%s\"."), classid.c_str());
        return (NULL);
    }
    Window* window = itr->second;
    Window* newwindowinstance = window->OnCreate(item);
    if (newwindowinstance == NULL) {
        wxLogError(_T("[penv::WindowList::CreateWindow] Create window for registered window '%s' failed, possible reasons could be that the OnCreate() method is not implemented."), classid.c_str());
    }
    return (newwindowinstance);
}



//----------------------------------------------------------------
void WindowList::CallPageChanged(wxWindow* newwin, wxWindow* oldwin)
/**
 * \brief This method will be called from the framework, when a page
 * in the notebook has changed. This method is only used internally.
 * \param newwin The new window to which the notebook changed.
 * \param oldwin The old window from which the notebook changed.
 **/
{
    Window* newwindow = NULL;
    Window* oldwindow = NULL;
    WindowsHashMap::iterator itr;
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        if (itr->second->GetType() != penvWT_Document) continue;
        if (unlikely(itr->second->GetWindow() == newwin)) {
            newwindow = itr->second;
            if (oldwindow != NULL) break;
        }
        if (unlikely(itr->second->GetWindow() == oldwin)) {
            oldwindow = itr->second;
            if (newwindow != NULL) break;
        }
    }
    if (likely(newwindow != NULL)) ((WindowDocument*)newwindow)->OnPageAfterChange();
    if (likely(oldwindow != NULL)) ((WindowDocument*)oldwindow)->OnPageAfterHidden();
}


//----------------------------------------------------------------
void WindowList::CallPageChanging(wxWindow* newwin, wxWindow* oldwin)
/**
 * \brief This method will be called from the framework, when a page
 * in the notebook will changing. This method is only used internally.
 * \param newwin The new window to which the notebook want to change.
 * \param oldwin The old window from which the notebook want to change.
 **/
{
    Window* newwindow = NULL;
    Window* oldwindow = NULL;
    WindowsHashMap::iterator itr;
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        if (itr->second->GetType() != penvWT_Document) continue;
        if (unlikely(itr->second->GetWindow() == newwin)) {
            newwindow = itr->second;
            if (oldwindow != NULL) break;
        }
        if (unlikely(itr->second->GetWindow() == oldwin)) {
            oldwindow = itr->second;
            if (newwindow != NULL) break;
        }
    }
    if (likely(newwindow != NULL)) ((WindowDocument*)newwindow)->OnPageBeforeChange();
    if (likely(oldwindow != NULL)) ((WindowDocument*)oldwindow)->OnPageBeforeHidden();
}



//----------------------------------------------------------------
bool WindowList::ReadNodeRegisteredWindows(wxXmlNode* node)
/**
 * \brief Read out a 'windows' xml element node for registered
 * windows.
 * See class description for structure of such an xml node.
 * \param node The 'windows' xml element node.
 * \return True on success; false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (false);
}



//----------------------------------------------------------------
WindowLayout* WindowList::ReadNodeOpenWindows(wxXmlNode* node)
/**
 * \brief Read out a 'windows' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'windows' xml element node.
 * \return Return WindowLayout on success; NULL otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (false);
}


//----------------------------------------------------------------
wxXmlNode* WindowList::WriteNode(bool registered)
/**
 * \brief Writes a 'windows' xml element node.
 * See class description for structure of such an xml node.
 * \param registered True the registered window information will be written,
 * false the open window information will be written.
 * \return The 'windows' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("windows"));
    if (registered)
    {
        node->AddProperty(_T("listtype"), _T("registered"));
        WindowsHashMap::iterator itr;
        for (itr = m_registered->begin(); itr != m_registered->end(); ++itr)
        {
            if (itr->second->GetType() != penvWT_Pane) continue;
            WindowPane* pane = (WindowPane*)itr->second;
            wxXmlNode* winnode = PenvHelper::CreateXmlNode(_T("window"));
            winnode->AddProperty(_T("classid"), pane->GetClassId());
            winnode->AddProperty(_T("type"), _T("pane"));
            wxXmlNode* positionnode = PenvHelper::CreateXmlNode(_T("position"),
                m_manager->SavePaneInfo(pane->GetPaneInfo()));
            PenvHelper::AddXmlChildNode(winnode, positionnode);
            wxXmlNode* visiblenode = PenvHelper::CreateXmlNode(_T("visible"),
                PenvHelper::CreateBoolean(pane->IsVisible()));
            PenvHelper::AddXmlChildNode(winnode, visiblenode);
            wxXmlNode* destroyonclosenode = PenvHelper::CreateXmlNode(_T("destroyonclose"),
                PenvHelper::CreateBoolean(pane->IsDestroyOnClose()));
            PenvHelper::AddXmlChildNode(winnode, destroyonclosenode);
            PenvHelper::AddXmlChildNode(node, winnode);
        }
    }
    else
    {
        node->AddProperty(_T("listtype"), _T("openwindows"));
        WindowsHashMap::iterator itr;
        for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
        {
            NOT_IMPLEMENTED_YET();
        }
    }
    return (node);
}


//----------------------------------------------------------------
void WindowList::RefreshPaneInfos()
/**
 * \brief Refreshes all pane infos for the window panes. The pane info
 * object holds apperance information (position, floating, ...) for a
 * WindowPane.
 **/
{
    WindowsHashMap::iterator itr;
    for (itr = m_openlist->begin(); itr != m_openlist->end(); ++itr)
    {
        if (itr->second->GetType() != penvWT_Pane) continue;
        WindowPane* winpane = (WindowPane*)itr->second;
        if (unlikely(winpane == NULL)) continue;
        winpane->SetPaneInfo(winpane->GetPaneInfo());
    }
}


//----------------------------------------------------------------
bool WindowList::AddDocument(WindowDocument* window, bool show)
/**
 * \brief Adds a document window to the open window list. This method
 * is only used framework internally.
 * This method will generate a PENV_EVT_WINDOW_ADDED event
 * after successful adding of the window.
 * \param window Document window to add.
 * \param show True to show the window immediately.
 * \return True on success; false otherwise.
 **/
{
    // Testen ob wxWindow == NULL, da sonst ein SIGSEV ausgelöst wird.
    //    Kein Fehler, sondern von der API so gewollt.
    if (window->GetWindow() == NULL) {
        wxLogError(_T("[penv::WindowList::AddDocument] Cannot add a not initalized window for class id \"%s\". Window::GetWindow() is NULL."), window->GetClassId().c_str());
        return (false);
    }
    // Hinzufügen des Documents zum Notebook
    bool status = m_notebook->AddPage(window->GetWindow(), window->GetCaption(),
        show, window->GetBitmap());
    if (unlikely(!status)) {
        wxLogError(_T("[penv::WindowList::AddDocument] Could not add WindowDocument (ID=%s) to wxAuiNotebook."), window->m_id.c_str());
        return (false);
    }
    wxVariant variant(window->GetId());
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_WINDOW_ADDED"), variant);
    return (true);
}


//----------------------------------------------------------------
bool WindowList::AddPane(WindowPane* window, bool show)
/**
 * \brief Adds a window pane to the open window list. This method
 * is only used framework internally.
 * This method will generate a PENV_EVT_WINDOW_ADDED event
 * after successful adding of the window.
 * \param window Window pane to add.
 * \param show True to show the window immediately.
 * \return True on success; false otherwise.
 **/
{
    // Testen ob wxWindow == NULL, da sonst ein SIGSEV ausgelöst wird.
    //    Kein Fehler, sondern von der API so gewollt.
    if (window->GetWindow() == NULL) {
        wxLogError(_T("[penv::WindowList::AddPane] Cannot add a not initalized window for class id \"%s\". Window::GetWindow() is NULL."), window->GetClassId().c_str());
        return (false);
    }
    // Get information for wxAuiPaneInfo
    wxAuiPaneInfo paneinfo = window->GetPaneInfo();
    paneinfo.Caption(window->GetCaption());
    // Name is the window id, because this is unique
    paneinfo.Name(window->GetId());
    // Insert pane into the window manager
    bool status = m_manager->AddPane(window->GetWindow(), paneinfo);
    if (unlikely(!status)) {
        wxLogError(_T("[penv::WindowList::AddPane] Could not add WindowPane (ID=%s) to wxAuiManager."), window->m_id.c_str());
        return (false);
    }
    // Manager must be updated after adding pane
    m_manager->Update();
    wxVariant variant(window->GetId());
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_WINDOW_ADDED"), variant);
    return (true);
}


//----------------------------------------------------------------
bool WindowList::ShowDocument(WindowDocument* window, bool show)
/**
 * \brief Shows or hides a document window. This method is only
 * framework internally used. It will show the document and
 * shows or hides the approptiate window mapping for this window.
 * \param window Window to show or hide.
 * \param show True to show window; false to hide it.
 * \return True on success; false otherwise.
 **/
{
    // Check if argument is null
    if (unlikely(window == NULL)) {
        wxLogError(_T("[penv::WindowList::ShowDocument] The argument window is NULL."));
        return (false);
    }
    // Check if window exists in open list
    if (unlikely(!this->ExistsWindow(window->GetId()))) {
        wxLogError(_T("[penv::WindowList::ShowDocument] A window with the id '%s' does not exist in the open list. You must add a window to the open list before showing it."), window->GetId().c_str());
        return (false);
    }
    // Check if window is a document
    if (unlikely(window->GetType() != penvWT_Document)) {
        wxLogError(_T("[penv::WindowList::ShowDocument] Calling to show a window document, which is not a document window is invalid."));
        return (false);
    }
    if (show)
    {
        // Bring to front if show is true
        int index = m_notebook->GetPageIndex(window->GetWindow());
        if (unlikely(index == -1))
        {
            wxLogWarning(_T("[penv::WindowList::ShowDocument] The window with id '%s' is not in the notebook."), window->GetId().c_str());
            // Add Window to the Notebook, because its in the open list
            //   and not in notebook.
            if (unlikely(!m_notebook->AddPage(window->GetWindow(),
                window->GetCaption(), true, window->GetBitmap())))
            {
                wxLogError(_T("[penv::WindowList::ShowDocument] Could not add window with id to notebook. Serious failure."), window->GetId().c_str());
                return (false);
            }
        }
        else
        {
            m_notebook->SetSelection(index);
        }
    }
    else
    {
        NOT_IMPLEMENTED_YET();
    }
    return (true);
}


//----------------------------------------------------------------
bool WindowList::ShowPane(WindowPane* window, bool show)
/**
 * \brief Shows or hides a window pane. This method is only
 * framework internally used. This method just checks the
 * window mapping and give a warning if a pane is not setup
 * for the actual document window.
 * \param window Window to show or hide.
 * \param show True to show window; false to hide it.
 * \return True on success; false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (true);
}



} // namespace penv

