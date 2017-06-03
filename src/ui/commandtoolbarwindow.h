/*
 * toolbarwindow.h - Deklaration of CommandToolBarWindow class
 *
 * Author: Steffen Ott
 * Date: 03/18/2008
 *
 */


#ifndef _PENV_UI_TOOLBARWINDOW_H_
#define _PENV_UI_TOOLBARWINDOW_H_


#include "../wxincludes.h"
#include "../environment.h"

namespace penv {


/**
 * \brief Defines the number of context menu ids
 * for the CommandToolBarWindow window.
 **/
#define penvCOMMANDTOOLBARWINDOW_MAXCONTEXTMENUIDS 5


/**
 * \brief The CommandToolBarWindow class is a panel where the
 * user could change the toolbars and items. This class is used in
 * the CommandDialog class, which is the easiest way to give your
 * user the chance to change toolbars.
 * \todo Drag toolbar items into toe command list control to remove them.
 * \todo Allow reordering via drag and drop in the tree control.
 * \todo Implement the filter functionality for the combobox.
 * \todo Icons for every toolbar item should be displayed in tree control.
 * \todo Sorting of the commands, must be implemented.
 **/
class CommandToolBarWindow : public wxPanel
{
    private:
        CommandToolBarWindow();
    public:
		CommandToolBarWindow(wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition,
            wxSize size = wxDefaultSize, int style = wxTAB_TRAVERSAL);
        virtual ~CommandToolBarWindow();

    public:
        void InitializeUI();
        void DoLayout();
        void ConnectEvents();
        void UpdateComponents();
        void SaveAndUpdate();


    protected:
		void OnToolbarsTreeBeginDrag(wxTreeEvent& event);
		void OnToolbarsTreeBeginLabelEdit(wxTreeEvent& event);
		void OnToolbarsTreeDeleteItem(wxTreeEvent& event);
		void OnToolbarsTreeEndDrag(wxTreeEvent& event);
		void OnToolbarsTreeEndLabelEdit(wxTreeEvent& event);
		void OnToolbarsItemCollapsed(wxTreeEvent& event);
		void OnToolbarsItemExpanding(wxTreeEvent& event);
		void OnToolbarsTreeItemGetTooltip(wxTreeEvent& event);
		void OnToolbarsTreeItemMenu(wxTreeEvent& event);
		void OnToolbarsTreeItemRightClick(wxTreeEvent& event);
		void OnToolbarsTreeKeyDown(wxTreeEvent& event);
		void OnToolbarsCombobox(wxCommandEvent& event);
		void OnToolbarsListBeginDrag(wxListEvent& event);
		void OnToolbarsListBeginLabelEdit(wxListEvent& event);
		void OnToolbarsListItemRightClick(wxListEvent& event);

        void OnContextMenuItemAddCommand(wxCommandEvent& event);
        void OnContextMenuItemAddToolBar(wxCommandEvent& event);
        void OnContextMenuItemAddSeparator(wxCommandEvent& event);
        void OnContextMenuItemRenameToolbar(wxCommandEvent& event);
        void OnContextMenuItemRemove(wxCommandEvent& event);


    protected:
		wxPanel* m_leftpanel;
		wxStaticText* m_labeltoolbars;
		wxTreeCtrl* m_treectrl;
		wxPanel* m_rightpanel;
		wxStaticText* m_labelcommands;
		wxComboBox* m_comboboxfilter;
		wxListCtrl* m_listctrl;
		wxMenu* m_contextmenu;
		int m_contextmenuids[penvCOMMANDTOOLBARWINDOW_MAXCONTEXTMENUIDS];
		wxArrayString m_removedtoolbars;
};


} // namespace penv

#endif // _PENV_UI_TOOLBARWINDOW_H_
