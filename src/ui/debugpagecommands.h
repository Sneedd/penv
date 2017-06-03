/*
 * debugpagecommands.h - Declaration of the DebugPageCommands class
 *
 * Author: Steffen Ott
 * Date: 11/15/2007
 *
 */

#ifndef _PENV_UI_DEBUGPAGECOMMANDS_H_
#define _PENV_UI_DEBUGPAGECOMMANDS_H_


#include "../wxincludes.h"
#include "../command.h"


namespace penv {



/**
 * \brief Defines the number of context menu ids
 * for the DebugPageCommands window.
 **/
#define penvDEBUGPAGECOMMANDS_MAXCONTEXTMENUIDS 3

/**
 * \brief The DebugPageCommands class is a panel with a
 * list control which displays the internally stored commands.
 * With a right click on a command or event you will be able
 * to execute or manipulate a command.
 **/
class DebugPageCommands : public wxPanel
{
    private:
        DebugPageCommands() {}
    public:
        DebugPageCommands(wxWindow* parent, int id = wxID_ANY);
        virtual ~DebugPageCommands();

        void InitializeUI();
        void DoLayout();
        void ConnectEvents();
        void UpdateComponents();
        void AddItem(Command* cmd);

    protected:
        void OnListItemRightClick(wxListEvent& event);
        void OnContextMenuItemExecute(wxCommandEvent& event);
        void OnContextMenuItemToogleEnabled(wxCommandEvent& event);
        void OnContextMenuItemRefresh(wxCommandEvent& event);


    protected:
        wxString m_commandid;
        wxListCtrl* m_listctrl;
        wxMenu* m_contextmenu;
        int m_contextmenuids[penvDEBUGPAGECOMMANDS_MAXCONTEXTMENUIDS];

};

} // namespace penv

#endif // _PENV_UI_DEBUGPAGECOMMANDS_H_
