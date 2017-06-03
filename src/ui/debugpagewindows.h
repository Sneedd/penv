/*
 * debugpagewindow.h - Declaration of the DebugPageWindows class
 *
 * Author: Steffen Ott
 * Date: 09/20/2007
 *
 */



#ifndef _PENV_UI_DEBUGPAGEWINDOWS_H_
#define _PENV_UI_DEBUGPAGEWINDOWS_H_


#include "../wxincludes.h"
#include "../window.h"

namespace penv {


/**
 * \brief Defines the number of context menu ids
 * for the DebugPageWindows window.
 **/
#define penvDEBUGPAGEWINDOWS_MAXCONTEXTMENUIDS 6



/**
 * \brief The DebugPageWindows class is a panel with a
 * list control which displays the internally stored windows.
 * With a right click on a window you will be able
 * to create or manipulate a window.
 **/
class DebugPageWindows : public wxPanel
{
    private:
        DebugPageWindows();
    public:
        DebugPageWindows(wxWindow* parent, int id = wxID_ANY);
        ~DebugPageWindows();

        void InitializeUI();
        void DoLayout();
        void ConnectEvents();
        void UpdateComponents();
        void AddItem(Window* window, bool registered);

    protected:
        void OnListItemRightClick(wxListEvent& event);
        void OnPopItemCreate(wxCommandEvent& event);
        void OnPopItemLoad(wxCommandEvent& event);
        void OnPopItemSave(wxCommandEvent& event);
        void OnPopItemClose(wxCommandEvent& event);
        void OnPopItemVisible(wxCommandEvent& event);
        void OnPopItemRefresh(wxCommandEvent& event);

    protected:
        wxMenu* m_contextmenu;
        int m_contextmenuids[penvDEBUGPAGEWINDOWS_MAXCONTEXTMENUIDS];
        wxListCtrl* m_listctrl;
        wxString m_windowtypeid;
        wxString m_windowid;
};

} // namespace penv

#endif // _PENV_UI_DEBUGPAGEWINDOWS_H_
