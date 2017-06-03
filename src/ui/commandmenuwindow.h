/*
 * commandmenuwindow.h - Declaration of the CommandMenuWindow class
 *
 * Author: Steffen Ott
 * Date: 04/23/2008
 *
 */

#ifndef _PENV_UI_COMMANDMENUWINDOW_H_
#define _PENV_UI_COMMANDMENUWINDOW_H_

#include "../menuitem.h"
#include "../wxincludes.h"

namespace penv {


/**
 * \brief Defines the number of context menu ids
 * for the CommandMenuWindow window.
 **/
#define penvCOMMANDMENUWINDOW_MAXCONTEXTMENUIDS 5



/**
 * \brief The CommandMenuWindow class is a panel where the user
 * could change the menubar and it's items. This class is used in
 * the CommandDialog class, which is the easiest way to give your
 * user the chance to change the menubar.
 * \todo Drag menu items into the command list control to remove them.
 * \todo Allow reordering via drag and drop in the tree control.
 * \todo Implement the filter functionality for the combobox.
 * \todo Icons for every toolbar item should be displayed in list and tree control.
 * \todo Sorting of the commands, must be implemented.
 **/
class CommandMenuWindow : public wxPanel
{
    private:
        CommandMenuWindow();
    public:
		CommandMenuWindow(wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition,
            wxSize size = wxDefaultSize, int style = wxTAB_TRAVERSAL);
        ~CommandMenuWindow();

        void InitializeUI();
        void DoLayout();
        void ConnectEvents();
        void UpdateComponents();
        void AddTreeItem(wxTreeItemId id, MenuItem* item);
        void SaveAndUpdate();
    private:
        void SaveAndUpdate(MenuItem* item, wxTreeItemId node, unsigned int lastcookie);

    private:
		void OnMenubarTreeBeginDrag(wxTreeEvent& event);
		void OnMenubarTreeBeginLabelEdit(wxTreeEvent& event);
		void OnMenubarTreeDeleteItem(wxTreeEvent& event);
		void OnMenubarTreeEndDrag(wxTreeEvent& event);
		void OnMenubarTreeEndLabelEdit(wxTreeEvent& event);
		void OnMenubarTreeItemCollapsed(wxTreeEvent& event);
		void OnMenubarTreeItemExpanding(wxTreeEvent& event);
		void OnMenubarTreeItemGetTooltip(wxTreeEvent& event);
		void OnMenubarTreeItemMenu(wxTreeEvent& event);
		void OnMenubarTreeItemRightClick(wxTreeEvent& event);
		void OnMenubarTreeKeyDown(wxTreeEvent& event);
		void OnMenubarCombobox(wxCommandEvent& event);
		void OnMenubarListBeginDrag(wxListEvent& event);
		void OnMenubarListBeginLabelEdit(wxListEvent& event);
		void OnMenubarListItemRightClick(wxListEvent& event);

        void OnContextMenuItemAddCommand(wxCommandEvent& event);
        void OnContextMenuItemAddSubMenu(wxCommandEvent& event);
        void OnContextMenuItemAddSeparator(wxCommandEvent& event);
        void OnContextMenuItemRenameSubMenu(wxCommandEvent& event);
        void OnContextMenuItemRemove(wxCommandEvent& event);


    private:
        wxPanel* m_leftpanel;
		wxStaticText* m_labelmenu;
		wxTreeCtrl* m_treectrl;
		wxPanel* m_rightpanel;
		wxStaticText* m_labelcommands;
		wxComboBox* m_comboboxfilter;
		wxListCtrl* m_listctrl;
        int m_contextmenuids[penvCOMMANDMENUWINDOW_MAXCONTEXTMENUIDS];
		wxMenu* m_contextmenu;

};


} // namespace penv


#endif // _PENV_UI_COMMANDMENUWINDOW_H_
