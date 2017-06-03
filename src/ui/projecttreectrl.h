/*
 * projecttreectrl.h - Declaration of the ProjectTreeCtrl class
 *
 * Author: Steffen Ott
 * Date: 03/18/2007
 *
 */


#ifndef _PENV_UI_PROJECTTREECTRL_H_
#define _PENV_UI_PROJECTTREECTRL_H_

#include "../wxincludes.h"
#include "../workspacelist.h"
#include "../workspace.h"
#include "../projectitemlist.h"
#include "../projectitem.h"
#include "../projectitemfile.h"
#include "../projectitemdirectory.h"
#include "../projectitemlinkeditems.h"
#include "../projectitemsubproject.h"
#include "projecttreectrlapperance.h"
#include "projecttreeitemid.h"
#include "projecttreeaction.h"


namespace penv {

/**
 * \brief Defines the maximal events used in the ProjectTreeCtrl
 * control.
 **/
#define penvPROJECTTREECTRL_MAXEVENTCOUNT 30


/**
 * \brief The ProjectTreeCtrl class is a control for handling workspaces, projects
 * and projectitems in the user interface.
 * This class should not be created manually instead, this class will be created
 * in ApplicationFrame and can be accessed also throught the ApplicationFrame. It
 * will be possible to inherit from ProjectTreeCtrl to change the behaivour of the
 * ProjectTreeCtrl, but this could end up in a very complicated task.<br>
 * The ProjectTreeCtrl has a buildin toolbar, which for default is not visible
 * this toolbar can be connected to buildin commands and normal commands.<br>
 * There are Methods to change the toolbar:<br>
 * Note: These methods could be exchanged in futher versions with a single
 * SetToolBar(ToolBar* toolbar) and ToolBar* GetToolBar().<br>
 * <ul>
 * <li><b>AddToolBarItem():</b> Adds a toolbar item, default items and spearators
 * could also be added.</li>
 * <li><b>ClearToolBarItems():</b> Clears the toolbar.</li>
 * <li><b>SetDefaultToolBarItems():</b> Sets a default set of toolbar items.</li>
 * <li><b>ShowToolBar():</b> Makes the toolbar visible or give the option to hide
 * it.</li>
 * </ul>
 * <br><br>
 * Like the ToolBar the context menu could also be changed via the same methods
 * but with a ContextMenu in their names instead of ToolBar. The methods are the
 * same. Because this feature will change when the ContextMenu class is created
 * the documenation will not be so good.<br>
 * <br><br>
 * To change the apperance of the ProjectTreeCtrl the class ProjectTreeCtrlApperance
 * is available. See the ProjectTreeCtrlApperance for more information.<br>
 * For the TreeCtrl in this control, there are some Keys registered which the
 * user can press to make a spezial action. See the list for possible keys and
 * combinations:<br>
 * <ul>
 * <li><b>Key F2:</b> Rename a item in the tree ctrl.</li>
 * <li><b>Key Del:</b> Delete a item in the tree ctrl.</li>
 * <li><b>Key Enter:</b> Activates a item, same as double click.</li>
 * <li>TODO there should be more like STRG+C, ...</li>
 * </ul>
 * <br><br>
 * There are some events which will be executed when the user uses the
 * ProjectTreeCtrl control:<br>
 * <ul>
 * <li><b>PENV_EVT_PROJECTTREECTRL_ADDCLICKED</b> Occurs when add is clicked.
 * Will maybe removed in futher versions.</li>
 * <li><b>PENV_EVT_PROJECTTREECTRL_ITEM_ACTIVATED</b> Occurs when a item in the
 * tree is double clicked. Sends a ProjectTreeItemData* as void* in the wxVariant
 * with the information which item is activated.</li>
 * <li>TODO There are more undocumented events.</li>
 * </ul>
 * \todo Activated Project should be marked bold.
 * \todo Additional BuildInCommand should be 'activate selected project'.
 * \todo Methods to return a selection in the TreeCtrl.
 * \todo Use penv::ContextMenu and penv::ToolBar internally if possible.
 * \todo Use internal BuildInCommands class for ProjectTreeCtrl, instead of
 * implementing them here.
 * \todo Check if a transparent icon will correct the wrong intendation in
 * the menu.
 * \todo Use for Properties the additional wxPropertyGrid class.
 * \todo Allow Multiselect, but only for one Level of a tree node.
 **/
class ProjectTreeCtrl : public wxPanel
{
    private:
        friend class ProjectTreeAction;
        ProjectTreeCtrl();
    public:
        ProjectTreeCtrl(wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition,
            wxSize size = wxDefaultSize, int style = wxTAB_TRAVERSAL);
        virtual ~ProjectTreeCtrl();

        void InitializeUI();
        void DoLayout();
        void ConnectEvents();
        void UpdateComponents();

        void SetProjectTreeCtrlApperance(ProjectTreeCtrlApperance* apperance);
        ProjectTreeCtrlApperance* GetProjectTreeCtrlApperance();

        void SetActionHandler(ProjectTreeAction* actionhandler);
        ProjectTreeAction* GetActionHandler();

        void SetDefaultToolBarItems();
        bool AddToolBarItem(const wxString& commandid, bool defaultitem = false);
        void ClearToolBarItems();
        void ShowToolBar(bool show = true);
        bool IsToolBarVisible() const;

        void SetDefaultContextMenuItems();
        bool AddContextMenuItem(const wxString& commandid,
            const wxString& submenu = wxEmptyString, bool defaultitem = false);
        void ClearContextMenuItems();
        void EnableContextMenu(bool enable = true);
        bool IsContextMenuEnabled() const;


        void SetToolTip(bool tooltip);
        void SetExpandOnDClick(bool expand);


        wxToolBar* GetToolBar();
        wxTreeCtrl* GetTreeCtrl();
        // TODO SO: Zugriff auf toolbar und tree ctrl

        void LogTreeView();

        ProjectTreeItemId SearchItem(const wxString& windowid);
        ProjectTreeItemId SearchItem(const Workspace* workspace);
        ProjectTreeItemId SearchItem(const Project* project);
        ProjectTreeItemId SearchItem(const ProjectItem* item);

    protected:
        wxArrayTreeItemIds* GetAllTreeItemIds() const;

    private:
        void GetAllTreeItemIds(wxArrayTreeItemIds* ids, wxTreeItemId id, unsigned int lastcookie) const;



    protected:
        virtual void AddTreeItems(bool projectitems);
        virtual wxTreeItemId AddWorkspace(Workspace* workspace);
        virtual wxTreeItemId AddProject(wxTreeItemId workspaceid, Project* project);
        virtual void AddProjectItemList(wxTreeItemId id, ProjectItemList* list, bool recursiv);
        virtual wxTreeItemId AddFile(wxTreeItemId id, ProjectItemFile* file);
        virtual wxTreeItemId AddDirectory(wxTreeItemId id, ProjectItemDirectory* directory, bool recursiv);
        virtual wxTreeItemId AddLinkedItems(wxTreeItemId id, ProjectItemLinkedItems* linkeditems);
        virtual wxTreeItemId AddSubProject(wxTreeItemId id, ProjectItemSubProject* subproject, bool recursiv);

    protected:
        virtual void OnClose(wxCloseEvent& event);
		virtual void OnTreeBeginDrag(wxTreeEvent& event);
		virtual void OnTreeBeginLabelEdit(wxTreeEvent& event);
		virtual void OnTreeDeleteItem( wxTreeEvent& event );
		virtual void OnTreeEndDrag( wxTreeEvent& event );
		virtual void OnTreeEndLabelEdit( wxTreeEvent& event );
		virtual void OnTreeItemActivated( wxTreeEvent& event );
		virtual void OnTreeItemCollapsed( wxTreeEvent& event );
		virtual void OnTreeItemCollapsing( wxTreeEvent& event );
		virtual void OnTreeItemExpanded( wxTreeEvent& event );
		virtual void OnTreeItemExpanding( wxTreeEvent& event );
		virtual void OnTreeItemGetTooltip( wxTreeEvent& event );
		virtual void OnTreeItemRightClick( wxTreeEvent& event );
		virtual void OnTreeItemMenu( wxTreeEvent& event );
		virtual void OnTreeItemKeyDown( wxTreeEvent& event );

		virtual void OnToolBarItemAddClicked(wxCommandEvent& event);
        virtual void OnToolBarItemDeleteClicked(wxCommandEvent& event);
        virtual void OnToolBarItemRefreshClicked(wxCommandEvent& event);
        virtual void OnToolBarItemShowAllClicked(wxCommandEvent& event);

        virtual void OnContextMenuItemAddFileClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemAddDirectoryClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemAddProjectClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemAddWorkspaceClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemAddExistingClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemOpenExternalClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemOpenClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemPasteClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemCutClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemCopyClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemDeleteClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemRenameClicked(wxCommandEvent& event);
        virtual void OnContextMenuItemPropertiesClicked(wxCommandEvent& event);


    protected:
        wxToolBar* m_toolbar;
        wxMenu* m_contextmenu;
		wxTreeCtrl* m_treectrl;
		wxTreeItemId m_rootid;
        ProjectTreeCtrlApperance* m_apperance;
        ProjectTreeAction* m_actionhandler;
        int m_eventids[penvPROJECTTREECTRL_MAXEVENTCOUNT];
        bool m_justclear;
        bool m_deletionyestoall;
        bool m_showall;
        bool m_showtoolbar;
        bool m_contextmenuenabled;
        bool m_showtooltip;
        bool m_expandondoubleclick;

};

} // namespace penv

#endif // _PENV_UI_PROJECTTREECTRL_H_
