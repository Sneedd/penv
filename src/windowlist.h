/*
 * windowlist.h - Declaration of the WindowList class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#ifndef _PENV_WINDOWLIST_H_
#define _PENV_WINDOWLIST_H_

#include "wxincludes.h"
#include "window.h"
#include "applicationframe.h"
#include "windowlayout.h"

namespace penv {


/**
 * \brief The WindowList class represents all windows in the framework.
 * Two types of windows will be used: WindowPane is a window, which can
 * be used as a toolwindow; and WindowDocument, which can be used for
 * as actual working window, for example writing documents.<br>
 * There are also two list for windows in this class. There is one list
 * to describe registered windows and a second list who contains all
 * opened windows. Please read 'The difference between open window and
 * registered window' in a lower section.<br>
 * Because the window information must be saved the window and all it's
 * properties can be saved in a file. The definition for registered windows
 * will not be used, for saving because default properties should be set
 * in the inherited window class. But the xml definition for open windows
 * will be used, to save the layout of the windows.<br>
 * Here the definition of a 'windows' xml element:<br>
 * <code>
 *  &lt;windows type="registeredwindows"&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- window elements should be here --&gt;<br>
 *  &lt;/windows&gt;<br>
 * </code>
 * or<br>
 * <code>
 *  &lt;windows type="openwindows"&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- window elements should be here --&gt;<br>
 *  &lt;/windows&gt;<br>
 * </code>
 * <b>The difference between open window and registered window:</b>
 * There is a difference between open window's and registered window's
 * and therefore the id of the window's is different. The open window id
 * can always be retrieved by Window::GetId(). The registered window id
 * can always be retrieved by Window::GetClassId(). But open windows could
 * have the same ClassId if they are created from the same window class.
 * An open window is the window shown in the UI or maybe hidden, but it is
 * always created from a registered window.<br>
 * <b>Show a previous registered window:</b> Before you can show a window
 * the window must be registered. The prefered place to register a window
 * is the initialization of a package. After a window is registered you
 * can show it by using CreateWindow() and Add(), see example below.
 * To create a window the Window::OnCreate() Method in a window will be
 * called and therefore the method must be implemented, to return a new
 * instance of the window. The Window::OnCreate() method will be called in
 * CreateWindow(). Window creation example:<br>
 * <code>
 * ProjectItem* projectitem = NULL;<br>
 * // You can spezify another ProjectItem which should be connected to that window.<br>
 * Window* window = windowlist->CreateWindow(_T("REGISTERED_WINDOW_ID"), projectitem);<br>
 * // Additional changes to the window can be done here.<br>
 * windowlist->Add(window, true);<br>
 * </code>
 * \todo Try to hide window panes via the wxAuiManager::GetPane().Hide() method
 * and update, because the actual hide method will add a extra close button to the
 * pane.
 * \todo Implement functionality to restore the actual window layout, which is
 * heavily connected to the project items, who is displayed in a document or pane.
 * \todo Resolve window mapping in ShowPane() and ShowDocument() and show or hide
 * the approriate window.
 * \todo Implement the functionality for standard windows, which can only be
 * created once, but with various project items.
 **/
class WindowList
{
    private:
        WindowList();
        WindowList(const WindowList& list);
        WindowList& operator=(const WindowList& list);
    private:
        friend class Environment;
        WindowList(ApplicationFrame* frame, wxAuiManager* manager, wxAuiNotebook* notebook);
        ~WindowList();
    public:

        WindowDocument* GetSelectedNotebookPage();

        // OpenList Methoden
        wxArrayString* GetOpenWindows() const;
        Window* GetWindow(const wxString& id);
        const wxString& GetWindowId(Window* window);
        const wxString& GetWindowId(wxWindow* window);
        bool Add(Window* window, bool show = false);
        bool Remove(const wxString& id);
        bool ShowWindow(const wxString& id, bool show = true);
        bool ExistsWindow(const wxString& id);
        bool ChangeWindowIds(const wxString& oldid, const wxString& newid);
        bool Clear();
        bool CloseAll(bool documentsonly=false, bool canveto=true);
        bool Close(const wxString& id, bool canveto=true);
        wxString NewId();

        // Registered List Methoden
        wxArrayString* GetRegisteredWindows() const;
        bool Register(Window* window);
        bool Unregister(const wxString& classid);
        bool IsRegistered(const wxString& classid);
        Window* GetRegistered(const wxString& classid);
        Window* CreateWindow(const wxString& classid, ProjectItem* item);

        // Call events
        void CallPageChanged(wxWindow* newwin, wxWindow* oldwin);
        void CallPageChanging(wxWindow* newwin, wxWindow* oldwin);

        // RebuildState Methoden
        bool ReadNodeRegisteredWindows(wxXmlNode* node);
        WindowLayout* ReadNodeOpenWindows(wxXmlNode* node);
        wxXmlNode* WriteNode(bool registered);

        void RefreshPaneInfos();

    private:
        bool AddDocument(WindowDocument* window, bool show);
        bool AddPane(WindowPane* window, bool show);
        bool ShowDocument(WindowDocument* window, bool show);
        bool ShowPane(WindowPane* window, bool show);

    private:
        WX_DECLARE_STRING_HASH_MAP(Window*, WindowsHashMap);
        WindowsHashMap* m_registered;
        WindowsHashMap* m_openlist;
        wxAuiManager* m_manager;
        wxAuiNotebook* m_notebook;
        ApplicationFrame* m_frame;
        Window* m_empty;
        wxString m_emptystring;
};

} // namespace penv

#endif // _PENV_WINDOWLIST_H_
