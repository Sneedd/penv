/*
 * applicationframe.h - Declaration of the ApplicationFrame class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */

#ifndef _PENV_APPLICATIONFRAME_H_
#define _PENV_APPLICATIONFRAME_H_

#include "wxincludes.h"
#include "statusbar.h"
#include "windowdocument.h"
#include "windowpane.h"
#include "toolbar.h"
#include "expect.h"
#include "ui/messagewindow.h"
#include "ui/debugwindow.h"
#include "ui/projecttreectrl.h"


namespace penv {

/**
 * \brief The ApplicationFrame class represents the TopLevelWindow
 * of this Framework. This class will be initialized by the Application
 * class. This frame already has an window pane manager and a notebook
 * as central window.<br>
 * You get access to the debug window, message window and project tree window
 * which are spezialized windows, who cannot be accessed via the WindowList
 * class. To show these windows just use the appropriate ToggleXXXXVisibility()
 * method.
 * \todo Implement a method to veto the exit of application in OnClose().
 * \todo Implement the message and debug window like project tree control.
 * \todo Implement a inherited ApplicationCenterWindow, where the developer
 * can choose between a single WindowDocument and a wxAuiNotebook with
 * multible WindowDocuments.
 **/
class ApplicationFrame : public wxFrame
{
    private:
        ApplicationFrame();
    public:
        ApplicationFrame(wxWindow* parent, const wxString& caption);
        ~ApplicationFrame();
    private:
        void InitalizeUI();
        void DoLayout();
        void ConnectEvents();

    public:
        wxAuiManager* GetManager();
        wxAuiNotebook* GetNotebook();
        wxMenuBar* GetMenuBar();
        StatusBar* GetStatusBar();
        DebugWindow* GetDebugWindow();
        MessageWindow* GetMessageWindow();
        void SetProjectTreeCtrl(ProjectTreeCtrl* treectrl);
        ProjectTreeCtrl* GetProjectTreeCtrl();

        void ToogleDebugWindow();
        bool IsDebugWindowVisible();

        void ToogleMessageWindow();
        bool IsMessageWindowVisible();

        void ToogleProjectTreeCtrl();
        bool IsProjectTreeCtrlVisible();

    private:
        void OnClose(wxCloseEvent& event);

        void OnAuiPaneButton(wxAuiManagerEvent& event);
        void OnAuiPaneClose(wxAuiManagerEvent& event);
        void OnAuiPaneMaximize(wxAuiManagerEvent& event);
        void OnAuiPaneRestore(wxAuiManagerEvent& event);
        void OnAuiPaneRender(wxAuiManagerEvent& event);
        void OnAuiPaneFindManager(wxAuiManagerEvent& event);

        void OnAuiNotebookAllowDND(wxAuiNotebookEvent& event);
		void OnAuiNotebookBeginDrag(wxAuiNotebookEvent& event);
		void OnAuiNotebookButton(wxAuiNotebookEvent& event);
		void OnAuiNotebookDragMotion(wxAuiNotebookEvent& event);
		void OnAuiNotebookEndDrag(wxAuiNotebookEvent& event);
		void OnAuiNotebookPageChanged(wxAuiNotebookEvent& event);
		void OnAuiNotebookPageChanging(wxAuiNotebookEvent& event);
		void OnAuiNotebookPageClose(wxAuiNotebookEvent& event);

    private:
        wxAuiManager* m_manager;
        wxAuiNotebook* m_notebook;
        wxMenuBar* m_menubar;
        StatusBar* m_statusbar;
        MessageWindow* m_msgwindow;
        DebugWindow* m_dbgwindow;
        ProjectTreeCtrl* m_prjtreectrl;
        bool m_prjtreectrlshown;
};

} // namespace penv

#endif // _PENV_APPLICATIONFRAME_H_
