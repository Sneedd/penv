/*
 * projecttreectrl.cpp - Implementation of the ProjectTreeCtrl class
 *
 * Author: Steffen Ott
 * Date: 03/18/2007
 *
 */


#include "projecttreectrl.h"
#include "../penvhelper.h"
#include "../environment.h"
#include "projecttreectrldroptarget.h"
#include "projecttreeitemdatabase.h"
#include "projecttreeitemdataobject.h"
#include "projecttreeitemdata.h"


namespace penv {


//----------------------------------------------------------------
ProjectTreeCtrl::ProjectTreeCtrl()
/**
 * \brief Standardconstructor (private).
 **/
{
    // Private cannot be called.
}


//----------------------------------------------------------------
ProjectTreeCtrl::ProjectTreeCtrl(wxWindow* parent, int id, wxPoint pos, wxSize size, int style )
    : wxPanel( parent, id, pos, size, style )
/**
 * \brief Constructor, which initialize the ProjectTreeCtrl.
 * \param parent Parent window of this control.
 * \param id Internal wxWidgets id.
 * \param pos Position of this control.
 * \param size Size of this control.
 * \param style Window style of this control.
 **/
{
    for( int i=0; i<penvPROJECTTREECTRL_MAXEVENTCOUNT; i++)
    {
        m_eventids[i] = wxNewId();
    }
    m_justclear = false;
    m_showtoolbar = false;
    m_apperance = new ProjectTreeCtrlApperance();
    m_actionhandler = new ProjectTreeAction(this);
    m_showtooltip = false;
    m_contextmenu = NULL;
    m_contextmenuenabled = true;

    InitializeUI();
    DoLayout();
    ConnectEvents();
    SetDefaultToolBarItems();
    SetDefaultContextMenuItems();
    UpdateComponents();
}



//----------------------------------------------------------------
ProjectTreeCtrl::~ProjectTreeCtrl()
/**
 * \brief Destructor.
 **/
{
    if (m_apperance != NULL) delete m_apperance;
    // TODO : Tree Item Data löschen
}


//----------------------------------------------------------------
void ProjectTreeCtrl::InitializeUI()
/**
 * \brief Initialize all user interface components.
 **/
{
	m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTB_HORIZONTAL|wxNO_BORDER|wxTB_FLAT|wxTB_NODIVIDER);
    m_toolbar->SetToolBitmapSize(wxSize(16,16));
	m_treectrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_HIDE_ROOT|penvPROJECTTREECTRL_SELECTIONMODE);
    m_treectrl->SetDropTarget(new ProjectTreeCtrlDropTarget(this, new ProjectTreeItemDataObject()));
}


//----------------------------------------------------------------
void ProjectTreeCtrl::DoLayout()
/**
 * \brief Creates the layout for the UI components.
 **/
{
    if (m_showtoolbar)
    {
        m_toolbar->Show();
        wxFlexGridSizer* sizer;
        sizer = new wxFlexGridSizer( 2, 1, 0, 0 );
        sizer->AddGrowableCol( 0 );
        sizer->AddGrowableRow( 1 );
        sizer->SetFlexibleDirection( wxBOTH );
        sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
        sizer->Add( m_toolbar, 1, wxEXPAND | wxALL, 5 );
        sizer->Add( m_treectrl, 0, wxALL|wxEXPAND, 5 );
        this->SetSizer( sizer );
        this->Layout();
    }
    else
    {
        m_toolbar->Hide();
        wxFlexGridSizer* sizer;
        sizer = new wxFlexGridSizer( 1, 1, 0, 0 );
        sizer->AddGrowableCol( 0 );
        sizer->AddGrowableRow( 0 );
        sizer->SetFlexibleDirection( wxBOTH );
        sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
        sizer->Add( m_treectrl, 0, wxALL|wxEXPAND, 5 );
        this->SetSizer( sizer );
        this->Layout();
    }
}


//----------------------------------------------------------------
void ProjectTreeCtrl::ConnectEvents()
/**
 * \brief Connects used events for internally functionality
 * of the control.
 **/
{
    //this->Connect(wxEVT_CLOSE_WINDOW,
    //    wxCloseEventHandler( ProjectTreeCtrl::OnClose ) );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_BEGIN_DRAG,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeBeginDrag ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeBeginLabelEdit ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_DELETE_ITEM,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeDeleteItem ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_END_DRAG,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeEndDrag ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_END_LABEL_EDIT,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeEndLabelEdit ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemActivated ), NULL, this );
	//m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_COLLAPSED,
    //    wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemCollapsed ), NULL, this );
	//m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_COLLAPSING,
    //    wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemCollapsing ), NULL, this );
	//m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_EXPANDED,
    //    wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemExpanded ), NULL, this );
	//m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_EXPANDING,
    //    wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemExpanding ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemGetTooltip ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemRightClick ), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_ITEM_MENU,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemMenu), NULL, this );
	m_treectrl->Connect( wxEVT_COMMAND_TREE_KEY_DOWN,
        wxTreeEventHandler( ProjectTreeCtrl::OnTreeItemKeyDown ), NULL, this );
}


//----------------------------------------------------------------
void ProjectTreeCtrl::UpdateComponents()
/**
 * \brief Updates the UI components with information.
 **/
{
    m_justclear = true;
    m_treectrl->DeleteAllItems();
    m_justclear = false;
    m_treectrl->SetImageList(m_apperance->GetImageList());
    AddTreeItems(true);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::SetProjectTreeCtrlApperance(ProjectTreeCtrlApperance* apperance)
/**
 * \brief Sets another apperance for the ProjectTreeCtrl. The
 * ProjectTreeCtrlApperance class must be inherited for this.
 * See ProjectTreeCtrlApperance class information.
 * \param apperance New apperance for the ProjectTreeCtrl.
 **/
{
    if (apperance == NULL)
    {
        wxLogError(_T("[penv::ProjectTreeCtrl::SetProjectTreeCtrlApperance] Argument 'apperance' cannot be NULL."));
        return;
    }
    m_apperance = apperance;
    m_treectrl->CollapseAll();
    UpdateComponents();
}


//----------------------------------------------------------------
ProjectTreeCtrlApperance* ProjectTreeCtrl::GetProjectTreeCtrlApperance()
/**
 * \brief Returns the ProjectTreeCtrl apperance.
 * \return Apperance of the ProjectTreeCtrl.
 **/
{
    return (m_apperance);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::SetActionHandler(ProjectTreeAction* actionhandler)
/**
 * \brief Sets the action handler for a actions on the tree control
 * and therefore also on the project hierarchy.
 * \param actionhandler Action handler for various actions on the tree control.
 **/
{
    if (actionhandler == NULL) {
        wxLogError(_T("[penv::ProjectTreeCtrl::SetActionHandler] Argument 'actionhandler' is NULL."));
        return;
    }
    m_actionhandler = actionhandler;
}


//----------------------------------------------------------------
ProjectTreeAction* ProjectTreeCtrl::GetActionHandler()
/**
 * \brief Returns the action handler for a actions on the tree control
 * and therefore also on the project hierarchy. Via this method a
 * new action handler can be registered to this ProjectTreeCtrl object
 * in order to override the standard implementation what to do on
 * different actions.
 * \return Action handler for various actions on the tree control.
 **/
{
    return (m_actionhandler);
}



//----------------------------------------------------------------
void ProjectTreeCtrl::SetDefaultToolBarItems()
/**
 * \brief Sets a default set of toolbar items for the
 * ProjectTreeCtrl. The default set is an ADD, DELETE,
 * REFRESH and SHOW ALL item.
 **/
{
    if(m_toolbar == NULL)
    {
        wxLogError(_T("[penv::ProjectTreeCtrl::SetDefaultToolBarItems] UIControls not initialized ."));
        return;
    }
    m_toolbar->ClearTools();
    AddToolBarItem(_T("ADD"), true);
    AddToolBarItem(_T("DELETE"), true);
    AddToolBarItem(wxEmptyString, true);
    AddToolBarItem(_T("REFRESH"), true);
    AddToolBarItem(_T("SHOWALL"), true);


}


//----------------------------------------------------------------
bool ProjectTreeCtrl::AddToolBarItem(const wxString& commandid, bool defaultitem)
/**
 * \brief Adds an item to the toolbar.
 * There are two possible items for this toolbar, a command where defaultitem must be
 * false (this is default). The other possibility is to use an already definied
 * command, where there are 4 of them: 'ADD', 'REMOVE', 'REFRESH' and 'SHOWALL'.<br>
 * However, you can also add an separator, when giving an wxEmptyString as an commandid.
 * Here is a small example for adding an default item:<br>
 * <code>
 * AddToolBarItem(_T("ADD"), false);
 * </code><br>
 * \param commandid Command ID or an internal definied ID
 * \param defaultitem True, when inserting an default toolbar item; false when an command should be added.
 * \return True on success; false otherwise.
 **/
{
    if(commandid.IsEmpty())
    {
        m_toolbar->AddSeparator();
        return (true);
    }
    if(defaultitem)
    {
        if(commandid == _T("ADD"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxBitmap bitmap(_T("data\\add.png"), wxBITMAP_TYPE_PNG);
            wxString name = _("Add item");
            wxString help = _("Adds an item to the selected node.");
            m_toolbar->AddTool(m_eventids[0], name, bitmap, wxNullBitmap,
                wxITEM_NORMAL, name, help);
            m_toolbar->Connect(m_eventids[0], wxEVT_COMMAND_TOOL_CLICKED,
                    wxCommandEventHandler(ProjectTreeCtrl::OnToolBarItemAddClicked),
                    NULL, this);
        }
        else if(commandid == _T("DELETE"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxBitmap bitmap(_T("data\\delete.png"), wxBITMAP_TYPE_PNG);
            wxString name = _("Delete item");
            wxString help = _("Delete the selected node.");
            m_toolbar->AddTool(m_eventids[1], name, bitmap, wxNullBitmap,
                wxITEM_NORMAL, name, help);
            m_toolbar->Connect(m_eventids[1], wxEVT_COMMAND_TOOL_CLICKED,
                wxCommandEventHandler(ProjectTreeCtrl::OnToolBarItemDeleteClicked),
                NULL, this);
        }
        else if(commandid == _T("REFRESH"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxBitmap bitmap(_T("data\\refresh.png"), wxBITMAP_TYPE_PNG);
            wxString name = _("Refresh the workspaces");
            wxString help = _("Refresh the whole workspaces, their projects and items.");
            m_toolbar->AddTool(m_eventids[2], name, bitmap, wxNullBitmap,
                wxITEM_NORMAL, name, help);
            m_toolbar->Connect(m_eventids[2], wxEVT_COMMAND_TOOL_CLICKED,
                wxCommandEventHandler(ProjectTreeCtrl::OnToolBarItemRefreshClicked),
                NULL, this);
        }
        else if(commandid == _T("SHOWALL"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxBitmap bitmap(_T("data\\showall.png"), wxBITMAP_TYPE_PNG);
            wxString name = _("Show all");
            wxString help = _("Show all, everthing. This is a *GODLIKE* button.");
            m_toolbar->AddTool(m_eventids[3], name, bitmap, wxNullBitmap,
                wxITEM_NORMAL, name, help);
            m_toolbar->Connect(m_eventids[3], wxEVT_COMMAND_TOOL_CLICKED,
                wxCommandEventHandler(ProjectTreeCtrl::OnToolBarItemShowAllClicked),
                NULL, this);
        }
        else
        {
            wxLogWarning(_T("[penv::ProjectTreeCtrl::AddToolBarItem] Unknown default item."));
            return (false);
        }
    }
    else
    {
        Environment* env = Environment::Get();
        Command* cmd = env->GetCommandList()->GetCommand(commandid);
        if(cmd == NULL)
        {
            wxLogError(_T("[penv::ProjectTreeCtrl::AddToolBarItem] Command with id '%s' is not registered."), commandid.c_str());
            return(false);
        }
        int newid = wxNewId();
        m_toolbar->AddTool(newid, cmd->GetName(), cmd->GetIcon(),
            wxNullBitmap, wxITEM_NORMAL, cmd->GetName(), cmd->GetHelp());
        env->GetFrame()->Connect(newid, wxEVT_COMMAND_TOOL_CLICKED,
            wxCommandEventHandler(Command::OnExecute), NULL, cmd);
    }
    m_toolbar->Realize();
    return(true);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::ClearToolBarItems()
/**
 * \brief Remove all toolbar items.
 **/
{
    // TODO SO: Müssen die Events deregistriert werden??
    m_toolbar->ClearTools();
    m_toolbar->Realize();
}




//----------------------------------------------------------------
void ProjectTreeCtrl::ShowToolBar(bool show)
/**
 * \brief Shows or hides the toolbar in this panel.
 * \param show True to show the toolbar; false to hide it.
 **/
{
    if (m_showtoolbar == show) return;
    m_showtoolbar = show;
    DoLayout();
}


//----------------------------------------------------------------
bool ProjectTreeCtrl::IsToolBarVisible() const
/**
 * \brief Returns if the toolbar is visible.
 * \return True if toolbar is visible; false otherwise.
 **/
{
    return (m_showtoolbar);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::SetDefaultContextMenuItems()
/**
 * \brief Sets a default set of context menu items for the
 * ProjectTreeCtrl.
 **/
{
    m_contextmenuenabled = true;
    this->ClearContextMenuItems();
    AddContextMenuItem(_T("ADDDIRECTORY"), _T("Add"), true);
    AddContextMenuItem(_T("ADDFILE"), _T("Add"), true);
    AddContextMenuItem(_T("ADDPROJECT"), _T("Add"), true);
    AddContextMenuItem(_T("ADDWORKSPACE"), _T("Add"), true);
    AddContextMenuItem(_T("ADDEXISTING"), wxEmptyString, true);
    AddContextMenuItem(wxEmptyString);
    AddContextMenuItem(_T("OPEN"), wxEmptyString, true);
    AddContextMenuItem(_T("OPENEXTERNAL"), wxEmptyString, true);
    AddContextMenuItem(wxEmptyString);
    AddContextMenuItem(_T("PASTE"), wxEmptyString, true);
    AddContextMenuItem(_T("CUT"), wxEmptyString, true);
    AddContextMenuItem(_T("COPY"), wxEmptyString, true);
    AddContextMenuItem(_T("DELETE"), wxEmptyString, true);
    AddContextMenuItem(wxEmptyString);
    AddContextMenuItem(_T("RENAME"), wxEmptyString, true);
    AddContextMenuItem(wxEmptyString);
    AddContextMenuItem(_T("PROPERTIES"), wxEmptyString, true);
}



//----------------------------------------------------------------
bool ProjectTreeCtrl::AddContextMenuItem(const wxString& commandid, const wxString& submenu, bool defaultitem)
/**
 * \brief Adds an item to the context menu.
 * There are two possible items for this context menu, a command where
 * default context menu must be false (this is default).
 * TODO Update Information when ContextMenu is finished!!!
 * \param commandid Command ID or an internal definied ID.
 * \param submenu The Name of the submenu in the ContextMenu.
 * \param defaultitem True, when inserting an default toolbar item;
 * false when an command should be added.
 * \return True on success; false otherwise.
 **/
{
    if (commandid.IsEmpty()) {
        m_contextmenu->AppendSeparator();
        return (true);
    }
    wxMenu* menu = NULL;
    if (submenu.IsEmpty()) {
        menu = m_contextmenu;
    } else {
        int id = m_contextmenu->FindItem(submenu);
        if (id == wxNOT_FOUND) {
            wxMenu* submenupointer = new wxMenu();
            wxMenuItem* submenuitem = m_contextmenu->AppendSubMenu(submenupointer,
                submenu, wxEmptyString);
            submenuitem->SetBitmap(wxNullBitmap);
            menu = submenuitem->GetSubMenu();
        } else {
            wxMenuItem* submenuitem = m_contextmenu->FindItem(id);
            menu = submenuitem->GetSubMenu();
        }
    }
    if (defaultitem)
    {
        if (commandid == _T("ADDFILE"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Add file ...");
            wxString help = _("Adds an file to the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[10], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[10], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemAddFileClicked),
                NULL, this);
        }
        else if (commandid == _T("ADDDIRECTORY"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Add directory ...");
            wxString help = _("Adds a directory to the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[11], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[11], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemAddDirectoryClicked),
                NULL, this);
        }
        else if (commandid == _T("ADDPROJECT"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Add project ...");
            wxString help = _("Adds a project to the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[12], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[12], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemAddProjectClicked),
                NULL, this);
        }
        else if (commandid == _T("ADDWORKSPACE"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Add workspace ...");
            wxString help = _("Adds a workspace to the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[13], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[13], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemAddWorkspaceClicked),
                NULL, this);
        }
        else if (commandid == _T("ADDEXISTING"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Add existing files ...");
            wxString help = _("Adds existing files to the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[14], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[14], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemAddExistingClicked),
                NULL, this);
        }
        else if (commandid == _T("OPENEXTERNAL"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Open external");
            wxString help = _("Opens the file with an external application.");
            wxMenuItem* item = menu->Append(m_eventids[15], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[15], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemOpenExternalClicked),
                NULL, this);
        }
        else if (commandid == _T("OPEN"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Open");
            wxString help = _("Opens the selected item in the application.");
            wxMenuItem* item = menu->Append(m_eventids[16], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[16], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemOpenClicked),
                NULL, this);
        }
        else if (commandid == _T("PASTE"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Paste");
            wxString help = _("Paste a item from the clipboard to the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[17], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[17], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemPasteClicked),
                NULL, this);
        }
        else if (commandid == _T("CUT"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Cut");
            wxString help = _("Cuts the selected node into the clipboard.");
            wxMenuItem* item = menu->Append(m_eventids[18], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[18], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemCutClicked),
                NULL, this);
        }
        else if (commandid == _T("COPY"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Copy");
            wxString help = _("Copy the selected node into the clipboard.");
            wxMenuItem* item = menu->Append(m_eventids[19], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[19], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemCopyClicked),
                NULL, this);
        }
        else if (commandid == _T("DELETE"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Delete");
            wxString help = _("Delete the selected node into the clipboard.");
            wxMenuItem* item = menu->Append(m_eventids[20], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[20], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemDeleteClicked),
                NULL, this);
        }
        else if (commandid == _T("RENAME"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Rename");
            wxString help = _("Rename the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[21], name, help, wxITEM_NORMAL);
            this->Connect(m_eventids[21], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemRenameClicked),
                NULL, this);
        }
        else if (commandid == _T("PROPERTIES"))
        {
            // TODO SO: Put Image and Text in ProjectTreeCtrlApperance
            wxString name = _("Properties");
            wxString help = _("Shows the properties for the selected node.");
            wxMenuItem* item = menu->Append(m_eventids[22], name, help, wxITEM_NORMAL);
            item->SetBitmap(wxNullBitmap);
            this->Connect(m_eventids[22], wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(ProjectTreeCtrl::OnContextMenuItemPropertiesClicked),
                NULL, this);
        }
    }
    else
    {
        Environment* env = Environment::Get();
        Command* cmd = env->GetCommandList()->GetCommand(commandid);
        if(cmd == NULL) {
            wxLogError(_T("[penv::ProjectTreeCtrl::AddContextMenuItem] Command with id '%s' is not registered."), commandid.c_str());
            return(false);
        }
        int newid = wxNewId();
        m_contextmenu->Append(newid, cmd->GetName(), cmd->GetHelp());
        env->GetFrame()->Connect(newid, wxEVT_COMMAND_TOOL_CLICKED,
            wxCommandEventHandler(Command::OnExecute), NULL, cmd);
    }
    return(true);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::ClearContextMenuItems()
/**
 * \brief Remove all context menu items.
 **/
{
    if (m_contextmenu != NULL) {
        // TODO SO: Deregister wxWidgest events
        delete m_contextmenu;
    }
    m_contextmenu = new wxMenu();
}


//----------------------------------------------------------------
void ProjectTreeCtrl::EnableContextMenu(bool enable)
/**
 * \brief Enables or disables the contextmenu.
 * \param enable True to enable the contextmenu; false to disable.
 **/
{
    m_contextmenuenabled = enable;
    if (m_contextmenuenabled && m_contextmenu == NULL) {
        m_contextmenu = new wxMenu();
    }
}


//----------------------------------------------------------------
bool ProjectTreeCtrl::IsContextMenuEnabled() const
/**
 * \brief Checks if the context menu is enabled.
 * \return True if the context menu is enabled; false if disabled.
 **/
{
    return (m_contextmenuenabled);
}




//----------------------------------------------------------------
void ProjectTreeCtrl::SetToolTip(bool tooltip)
/**
 * \brief Sets if the tree ctrls should display a tooltip
 * when hovering over a project tree item. Additional
 * information will be displayed, when this switch is on.
 * The default value setting is false.
 * \param tooltip True if the tooltip should be visible.
 **/
{
   m_showtooltip = tooltip;
}



//----------------------------------------------------------------
void ProjectTreeCtrl::SetExpandOnDClick(bool expand)
/**
 * \brief Sets if a tree node should expand on a double click,
 * another action is to activate the tree item. If this
 * property is set to false, then the tree nodes can furthermore
 * be expanded on the plus button. The default value setting is
 * false.
 * \param expand True if the tree node should expand on
 * double click.
 **/
{
    m_expandondoubleclick = expand;
}

//----------------------------------------------------------------
wxToolBar* ProjectTreeCtrl::GetToolBar()
/**
 * \brief Returns the inner ToolBar
 * \return wxToolBar
 **/
{
    return(m_toolbar);
}


//----------------------------------------------------------------
wxTreeCtrl* ProjectTreeCtrl::GetTreeCtrl()
/**
 * \brief Returns the inner TreeCtrl
 * \return wxTreeCtrl
 **/
{
    return(m_treectrl);
}



//----------------------------------------------------------------
void ProjectTreeCtrl::LogTreeView()
/**
 * \brief Logs all TreeView Items in the tree control into the
 * standard log target.
 **/
{
    wxArrayTreeItemIds* array = GetAllTreeItemIds();
    for (size_t i=0; i<array->Count(); ++i)
    {
        wxTreeItemId treeitemid = (*array)[i];
        ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(treeitemid);
        if (data == NULL)
        {
            wxString strg;
            for (int i=0; i<m_treectrl->GetIndent(); i++)
                strg.Append(_T("  "));
            strg.Append(m_treectrl->GetItemText(treeitemid));
            strg.Append(_T(" / Data is NULL."));
            wxLogMessage(_T("[penv::ProjectTreeCtrl::LogTreeView] %s"),strg.c_str());
        }
        else
        {
            wxString strg;
            for (int i=0; i<m_treectrl->GetIndent(); i++)
                strg.Append(_T("  "));
            strg.Append(m_treectrl->GetItemText(treeitemid));
            strg.Append(_T(" / Data available"));
            wxLogMessage(_T("[penv::ProjectTreeCtrl::LogTreeView] %s"),strg.c_str());
        }
    }
    delete array;
}


//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeCtrl::SearchItem(const wxString& windowid)
/**
 * \brief Searches the tree control for a item which is connected
 * to a spezified window. If the item does not exists this method
 * will return a invalid ProjectTreeItemId (can be tested with IsOk()).
 * \param windowid The item with the given windowid to search for.
 * \return Valid project tree item id if found; invalid if not found.
 **/
{
    wxArrayTreeItemIds* array = GetAllTreeItemIds();
    for (size_t i=0; i<array->Count(); ++i)
    {
        wxTreeItemId id = (*array)[i];
        if (id.IsOk())
        {
            ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
            if (data != NULL)
            {
                if (data->GetType() == penvTI_ProjectItem && data->GetProjectItem() != NULL)
                {
                    wxString prjitemwinid = data->GetProjectItem()->GetWindowId();
                    if (prjitemwinid == windowid)
                    {
                        delete array;
                        return (ProjectTreeItemId(id));
                    }
                }
            }
        }
    }
    delete array;
    return (ProjectTreeItemId());
}


//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeCtrl::SearchItem(const Workspace* workspace)
/**
 * \brief Searches the tree control for a workspace which is connected
 * to a spezified window. If the workspace does not exists this method
 * will return a invalid ProjectTreeItemId (can be tested with IsOk()).
 * \param workspace The item with the given workspace.
 * \return Valid project tree item id if found; invalid if not found.
 **/
{
    wxArrayTreeItemIds* array = GetAllTreeItemIds();
    for (size_t i=0; i<array->Count(); ++i)
    {
        wxTreeItemId id = (*array)[i];
        if (id.IsOk())
        {
            ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
            if (data != NULL)
            {
                if (data->GetType() == penvTI_Workspace && data->GetWorkspace() == workspace)
                {
                    delete array;
                    return (ProjectTreeItemId(id));
                }
            }
        }
    }
    delete array;
    return (ProjectTreeItemId());
}

//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeCtrl::SearchItem(const Project* project)
/**
 * \brief Searches the tree control for a project which is connected
 * to a spezified window. If the project does not exists this method
 * will return a invalid ProjectTreeItemId (can be tested with IsOk()).
 * \param windowid The item with the given project.
 * \return Valid project tree item id if found; invalid if not found.
 **/
{
    wxArrayTreeItemIds* array = GetAllTreeItemIds();
    for (size_t i=0; i<array->Count(); ++i)
    {
        wxTreeItemId id = (*array)[i];
        if (id.IsOk())
        {
            ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
            if (data != NULL)
            {
                if (data->GetType() == penvTI_Project && data->GetProject() == project)
                {
                    delete array;
                    return (ProjectTreeItemId(id));
                }
            }
        }
    }
    delete array;
    return (ProjectTreeItemId());
}



//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeCtrl::SearchItem(const ProjectItem* item)
/**
 * \brief Searches the tree control for a project which is connected
 * to a spezified window. If the project does not exists this method
 * will return a invalid ProjectTreeItemId (can be tested with IsOk()).
 * \param windowid The item with the given project.
 * \return Valid project tree item id if found; invalid if not found.
 **/
{
    wxArrayTreeItemIds* array = GetAllTreeItemIds();
    for (size_t i=0; i<array->Count(); ++i)
    {
        wxTreeItemId id = (*array)[i];
        if (id.IsOk())
        {
            ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
            if (data != NULL)
            {
                if (data->GetType() == penvTI_ProjectItem && data->GetProjectItem() == item)
                {
                    delete array;
                    return (ProjectTreeItemId(id));
                }
            }
        }
    }
    delete array;
    return (ProjectTreeItemId());
}



//----------------------------------------------------------------
wxArrayTreeItemIds* ProjectTreeCtrl::GetAllTreeItemIds() const
/**
 * \brief Returns all tree item ids in the tree control.
 * Does not include the root item. The Array must be destroyed
 * by the caller, after use.
 * \return All tree item ids in the tree control.
 **/
{
    wxArrayTreeItemIds* array = new wxArrayTreeItemIds();
    // Insert all items
    wxTreeItemId rootid = m_treectrl->GetRootItem();
    unsigned int cookievalue = 0xC00C1E0;
    wxTreeItemIdValue cookie = (wxTreeItemIdValue)cookievalue;
    wxTreeItemId treeitemid = m_treectrl->GetFirstChild(rootid, cookie);
    while (treeitemid.IsOk())
    {
        // Add Tree Item
        array->Add(treeitemid);
        // Recusive call
        GetAllTreeItemIds(array, treeitemid, cookievalue);
        // Move to the next menuitem
        treeitemid = m_treectrl->GetNextChild(treeitemid, cookie);
    }
    return (array);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::GetAllTreeItemIds(wxArrayTreeItemIds* ids,
    wxTreeItemId id, unsigned int lastcookie) const
/**
 * \brief Recursiv function for GetAllTreeItemIds.
 * \param ids Array for the tree item ids to store in.
 * \param id Id of the last node.
 * \param lascookie The last used cookie.
 **/
{
    wxTreeItemIdValue cookie = (wxTreeItemIdValue)(lastcookie+1);
    wxTreeItemId treeitemid = m_treectrl->GetFirstChild(id, cookie);
    while (treeitemid.IsOk())
    {
        // Add Tree Item
        ids->Add(treeitemid);
        // Recusive call
        GetAllTreeItemIds(ids, treeitemid, (unsigned int)cookie);
        // Move to the next menuitem
        treeitemid = m_treectrl->GetNextChild(treeitemid, cookie);
    }
}



//----------------------------------------------------------------
void ProjectTreeCtrl::AddTreeItems(bool projectitems)
/**
 * \brief Adds everything recursivly. Exept projectitems is false, then
 * only workspaces and projects will be added.
 * \param projectitems True if all tree items should be filled.
 **/
{
    m_rootid = m_treectrl->AddRoot(_T("Workspaces"), -1, -1, NULL);
    WorkspaceList* list = Environment::Get()->GetWorkspaceList();
    for (size_t i=0; i<list->Count(); ++i)
    {
        Workspace* workspace = list->GetItem(i);
        if (workspace == NULL)
        {
            wxLogWarning(_T("[penv::ProjectTreeCtrl::AddTreeItems] WorkspaceList not correctly filled at index '%u'."), i);
            continue;
        }
        wxTreeItemId wsid = AddWorkspace(workspace);
        for(size_t j = 0; j < workspace->GetProjectList()->Count(); j++)
        {
            Project* project = workspace->GetProjectList()->GetItem(j);
            if (project == NULL)
            {
                wxLogWarning(_T("[penv::ProjectTreeCtrl::AddTreeItems] ProjectList not correctly filled ar index '%u'"), j);
                continue;
            }
            wxTreeItemId prjid = AddProject(wsid, project);
            if (projectitems) AddProjectItemList(prjid, project->GetProjectItemList(), true);
        }
    }
}


//----------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::AddWorkspace(Workspace* workspace)
/**
 * \brief Adds a workspace to the tree, a check if workspace is
 * NULL will not be done.
 * \param workspace Workspace to add to the tree control.
 * \return Returns the newly added workspaces tree item ID.
 **/
{
    int imageindex = m_apperance->GetImageIndexWorkspace();
    wxTreeItemId id = m_treectrl->AppendItem(m_rootid,
        workspace->GetName(), imageindex, -1, new ProjectTreeItemData(workspace));
    workspace->SetTreeItemId(id);
    return (id);
}


//----------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::AddProject(wxTreeItemId workspaceid, Project* project)
/**
 * \brief Adds an project to the tree, a check if project is
 * NULL will not be done. And check if tree item ID will also not
 * be done.
 * \param workspaceid The tree item id of the workspace.
 * \param project Project to add to the tree control.
 * \return Returns the newly added project tree item ID.
 **/
{
    int imageindex = m_apperance->GetImageIndexProject();
    wxTreeItemId id = m_treectrl->AppendItem(workspaceid,
        project->GetName(), imageindex, -1, new ProjectTreeItemData(project));
    project->SetTreeItemId(id);
    return (id);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::AddProjectItemList(wxTreeItemId id, ProjectItemList* list, bool recursiv)
/**
 * \brief Adds an project item list to the tree, a check if list is
 * NULL will not be done. Also a check if tree item ID is correct
 * will not be done. The tree item id cannot be a workspace, however
 * all other tree item IDs might work.<br>
 * If implementing an own project tree ctrl for own project items this
 * method can be overridden in order to show these project items.
 * \param id Tree item id of a project or project item who have a ProjectItemList.
 * \param list ProjectItemList which should be added to the tree control.
 * \param recursiv Adds every item recursiv.
 **/
{
    if (list == NULL)
    {
        wxLogError(_T("[penv::ProjectTreeCtrl::AddProjectItemList] ProjectItemList is NULL."));
        return;
    }
    for (size_t i=0; i<list->Count(); ++i)
    {
        ProjectItem* item = list->GetItem(i);
        if (item == NULL)
        {
            wxLogWarning(_T("[penv::ProjectTreeCtrl::AddProjectItemList] ProjectItem is NULL."));
            continue;
        }
        ProjectItemList* itemlist = NULL;
        wxTreeItemId itemid;
        if (item->GetType() == penvPI_File) {
            itemid = AddFile(id, (ProjectItemFile*)item);
        }
        else if (item->GetType() == penvPI_Directory) {
            itemid = AddDirectory(id, (ProjectItemDirectory*)item, recursiv);
            itemlist = item->GetItemList();
        }
        else if (item->GetType() == penvPI_LinkedItems) {
            itemid = AddLinkedItems(id,  (ProjectItemLinkedItems*)item);
            // LinkedItems werden sowieso alle rekursiv eingefügt
        }
        else if (item->GetType() == penvPI_SubProject) {
            itemid = AddSubProject(id,  (ProjectItemSubProject*)item, recursiv);
            itemlist = item->GetItemList();
        }
        else
        {
            wxLogWarning(_T("[penv::ProjectTreeCtrl::AddProjectItemList] Unknown item type."));
            continue;
        }
    }
}



//----------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::AddFile(wxTreeItemId id, ProjectItemFile* file)
/**
 * \brief Adds an project item of type 'file' to the given tree node,
 * a check if file is NULL will not be done.
 * \param id Parent id of the project item.
 * \param file The project item which should be added.
 * \return Returns the newly added project tree item ID.
 **/
{
    int imageindex = m_apperance->GetImageIndexFile();
    wxTreeItemId fileid = m_treectrl->AppendItem(id,
        file->GetName(), imageindex, -1, new ProjectTreeItemData(file));
    file->SetTreeItemId(fileid);
    return (fileid);
}

//----------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::AddDirectory(wxTreeItemId id, ProjectItemDirectory* directory, bool recursiv)
/**
 * \brief Adds an project item of type 'directory' to the given tree node,
 * a check if file is NULL will not be done.
 * \param id Parent id of the project item.
 * \param file The project item which should be added.
 * \param recursiv True if all project items should be recursively added.
 * \return Returns the newly added project tree item ID.
 **/
{
    int imageindex = m_apperance->GetImageIndexDirectory();
    wxTreeItemId dirid = m_treectrl->AppendItem(id,
        directory->GetName(), imageindex, -1, new ProjectTreeItemData(directory));
    directory->SetTreeItemId(dirid);
    // Alle Element eine Ebene darunter einfügen
    AddProjectItemList(dirid, directory->GetItemList(), recursiv);
    return (dirid);
}


//----------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::AddLinkedItems(wxTreeItemId id, ProjectItemLinkedItems* linkeditems)
/**
 * \brief Adds an project item of type 'linkeditems' to the given tree node,
 * a check if file is NULL will not be done.
 * \param id Parent id of the project item.
 * \param file The project item which should be added.
 * \return Returns the newly added project tree item ID.
 **/
{
    int imageindex = m_apperance->GetImageIndexLinkedItems();
    wxTreeItemId liid = m_treectrl->AppendItem(id,
        linkeditems->GetName(), imageindex, -1, new ProjectTreeItemData(linkeditems));
    linkeditems->SetTreeItemId(liid);
    // Alle linked Items recursiv hinzufügen
    AddProjectItemList(liid, linkeditems->GetItemList(), true);
    return (liid);
}



//----------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::AddSubProject(wxTreeItemId id, ProjectItemSubProject* subproject, bool recursiv)
/**
 * \brief Adds an project item of type 'subproject' to the given tree node,
 * a check if file is NULL will not be done.
 * \param id Parent id of the project item.
 * \param file The project item which should be added.
 * \param recursiv True if all project items should be recursively added.
 * \return Returns the newly added project tree item ID.
 **/
{
    int imageindex = m_apperance->GetImageIndexSubProject();
    // TODO SO: Cast zu Project* entfernen
    wxTreeItemId subid = m_treectrl->AppendItem(id,
        ((Project*)subproject)->GetName(), imageindex, -1, new ProjectTreeItemData((ProjectItem*)subproject));
    ((Project*)subproject)->SetTreeItemId(subid);
    // Alle Element eine Ebene darunter einfügen
    AddProjectItemList(subid, subproject->GetItemList(), recursiv);
    return (subid);
}



//----------------------------------------------------------------
void ProjectTreeCtrl::OnClose(wxCloseEvent& event)
/**
 * \brief Occurs when the window is closed.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeBeginDrag(wxTreeEvent& event)
/**
 * \brief This event occurs when the user begins an drag'n'drop
 * operation on an tree item. When an tree item will be dragged,
 * the user can copy that item by pressing CTRL. Or move the item,
 * if he simply drag it to a new location. The behavior for the
 * various project items can be found in the following list:<br>
 * <b>Workspace:</b> Workspaces cannot be dragged.<br>
 * <b>Project:</b> Projects can only be dragged to another workspace.<br>
 * <b>ProjectItemFile:</b> Files can be dragged to another project, directory or subproject.<br>
 * <b>ProjectItemDirectory:</b> Same as ProjectItemFile.<br>
 * <b>ProjectItemLinkedItems:</b> Same as ProjectItemFile.<br>
 * <b>ProjectItemSubProject:</b> Same as ProjectItemFile.<br>
 * \param event TreeEvent.
 **/
{
    wxTreeItemId id = event.GetItem();
    if (!id.IsOk()) return;
    ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
    if (data == NULL)
    {
        wxLogError(_T("[penv::ProjectTreeCtrl::OnTreeBeginDrag] data in DragDropOperation is NULL."));
        return;
    }

    ProjectTreeItemDataObject* objdata = NULL;
    if (data->GetType() == penvTI_Project) {
        objdata = new ProjectTreeItemDataObject(data->GetProject());
    }
    else if (data->GetType() == penvTI_ProjectItem) {
        objdata = new ProjectTreeItemDataObject(data->GetProjectItem());
    }
    else if (data->GetType() == penvTI_Workspace) {
        // Workspaces dürfen nicht verschoben werden!
        return;
    }
    if (objdata == NULL) {
        wxLogError(_T("[penv::ProjectTreeCtrl::OnTreeBeginDrag] Unknow ProjectTreeItemData type."));
        return;
    }
    m_treectrl->GetDropTarget()->SetDataObject(objdata);
    wxDropSource dragsource(*objdata, m_treectrl);
	wxDragResult result = dragsource.DoDragDrop(true);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeBeginLabelEdit(wxTreeEvent& event)
/**
 * \brief This event occurs when the user begins an drag'n'drop
 * operation on an tree item.
 * \param event TreeEvent.
 **/
{
    //wxLogMessage(_T("abc"));
    // TODO
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeDeleteItem(wxTreeEvent& event )
/**
 * \brief This event occurs when the user deletes an tree item.
 * The programatically deletetion of an tree item also calls
 * this method. Here the method checks what item will be deleted
 * and do the appropriate behavior as follow:<br>
 * <b>Workspace:</b> A workspace can be deleted, but we don't delete the whole
 * workspace. We only remove the workspace from the workspace list.<br>
 * <b>Project:</b> Same as a workspace.<br>
 * <b>ProjectItemFile:</b> If a project file will be deleted. The member variable
 * m_deletefromdisk spezifies if a project item will be deleted also from disk.<br>
 * <b>ProjectItemDirectory:</b> Same<br>
 * <b>ProjectItemLinkedItems:</b><br>
 * <b>ProjectItemSubProject:</b><br>
 * Additional note: If more than one file will be deleted, than the user can
 * use 'Yes, to all' or 'No, to all'. These will force the method to delete
 * everything or nothing without asking. The variable m_deletionyestoall
 * spezifies the deletion of everything.
 * \param event Event.
 **/
{
    // If the tree control will just be cleared (for example on a refresh),
    //  then this method will skip.
    if (m_justclear) return;
    // TODO
    //NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeEndDrag(wxTreeEvent& event )
/**
 * \brief
 **/
{
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeEndLabelEdit(wxTreeEvent& event )
/**
 * \brief Occurs when a tree item label edit has ended.
 * Changes the name of the appropriate tree item.
 * \param event Event.
 **/
{
    wxTreeItemId itemid = event.GetItem();
    if(!itemid.IsOk()) return;
    ProjectTreeItemData* itemdata = (ProjectTreeItemData*)m_treectrl->GetItemData(itemid);
    if(itemdata == NULL)
    {
        wxLogError(_T("[penv::ProjectTreeCtrl::OnTreeEndLabelEdit] TreeItemData must be not NULL."));
        return;
    }
    if(itemdata->GetType() == penvTI_Project)
    {
        Project* project = itemdata->GetProject();
        if(project == NULL)
        {
            wxLogError(_T("[penv::ProjectTreeCtrl::OnTreeEndLabelEdit] Project must be not NULL."));
            return;
        }
        project->SetName(event.GetLabel());
    }
    else if (itemdata->GetType() == penvTI_Workspace)
    {
        Workspace* workspace = itemdata->GetWorkspace();
        if(workspace == NULL)
        {
            wxLogError(_T("[penv::ProjectTreeCtrl::OnTreeEndLabelEdit] Workspace must be not NULL."));
            return;
        }
        workspace->SetName(event.GetLabel());
    }
    else if (itemdata->GetType() == penvTI_ProjectItem)
    {
        ProjectItem* projectitem = itemdata->GetProjectItem();
        if(projectitem == NULL)
        {
            wxLogError(_T("[penv::ProjectTreeCtrl::OnTreeEndLabelEdit] ProjectItem must be not NULL."));
            return;
        }
        projectitem->SetName(event.GetLabel());
    }
    else
    {
        wxLogWarning(_T("[penv::ProjectTreeCtrl::OnTreeEndLabelEdit] Type of TreeItemData unknown."));
        return;
    }
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemActivated( wxTreeEvent& event )
/**
 * \brief Occurs when a project tree item was activated, for
 * example by double click on the item. This will just
 * execute the PENV_EVT_PROJECTTREECTRL_ITEM_ACTIVATED, with
 * a variant data, where a ProjectTreeItemData Pointer is stored as
 * void pointer.
 * \param event Event.
 **/
{
    wxTreeItemId id = event.GetItem();
    if (!id.IsOk()) return;
    ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
    wxVariant variant = (void*)data;
    Environment::Get()->GetCommandList()
        ->Execute(_T("PENV_EVT_PROJECTTREECTRL_ITEM_ACTIVATED"), variant);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemCollapsed( wxTreeEvent& event )
/**
 * \brief Occurs when a project tree branch has collapsed.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemCollapsing( wxTreeEvent& event )
/**
 * \brief Occurs before a project tree branch is going to collapse.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemExpanded(wxTreeEvent& event)
/**
 * \brief Occurs when a project tree branch has expanded.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemExpanding( wxTreeEvent& event )
/**
 * \brief Occurs before a project tree branch is going to expand.
 * (Updates the underlying branch items.)
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemGetTooltip( wxTreeEvent& event )
/**
 * \brief Occurs when the tooltip content will be requested.
 * \param event Event.
 **/
{
    if (!m_showtooltip) return;
    wxTreeItemId id = event.GetItem();
    if (id.IsOk())
    {
        ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
        if (data == NULL) return;
        wxString strg;
        if (data->GetType() == penvTI_Workspace) {
            Workspace* workspace = data->GetWorkspace();
            strg.Append(wxString::Format(_T("%s\nWorkspace\n%s"), workspace->GetName().c_str(), Path::GetLastEntry(workspace->GetFileNameString()).c_str()));
        }
        else if (data->GetType() == penvTI_Project) {
            Project* project = data->GetProject();
            strg.Append(wxString::Format(_T("%s\nProject\n%s"), project->GetName().c_str(), Path::GetLastEntry(project->GetFileNameString()).c_str()));
        }
        else if (data->GetType() == penvTI_ProjectItem) {
            ProjectItem* item = data->GetProjectItem();
            if (item->GetType() == penvPI_File) {
                strg.Append(wxString::Format(_T("%s\nFile\n%s"), item->GetName().c_str(), Path::GetLastEntry(item->GetPathString()).c_str()));
            }
            else if (item->GetType() == penvPI_Directory) {
                strg.Append(wxString::Format(_T("%s\nDirectory\n%s"), item->GetName().c_str(), Path::GetLastEntry(item->GetPathString()).c_str()));
            }
            else if (item->GetType() == penvPI_LinkedItems) {
                strg.Append(wxString::Format(_T("%s\nLinked items"), item->GetName().c_str()));
            }
            else if (item->GetType() == penvPI_SubProject) {
                strg.Append(wxString::Format(_T("%s\nSub project\n%s"), item->GetName().c_str(), Path::GetLastEntry(item->GetPathString()).c_str()));
            }
        }
        event.SetToolTip(strg);
    }
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemRightClick( wxTreeEvent& event )
/**
 * \brief Occurs when the right mouse button is clicked on a tree
 * item. Selects the right clicked item.
 * \param event Event.
 **/
{
    if (!event.GetItem().IsOk()) return;
    m_treectrl->SelectItem(event.GetItem(), true);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemMenu( wxTreeEvent& event )
/**
 * \brief Occurs when the menu of the tree control should be opend.
 * This method enables or disables context menu items and shows
 * the context menu.
 * \param event Event.
 **/
{
    m_deletionyestoall = false;
    if(m_contextmenuenabled)
    {
        if(m_contextmenu != NULL)
        {
            wxTreeItemId id = m_treectrl->GetSelection();
            ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
            CommandList* list = Environment::Get()->GetCommandList();
            // Enable or diable AddFile
            m_contextmenu->Enable(m_eventids[10], data->GetType() != penvTI_Workspace);
            m_contextmenu->Enable(m_eventids[11], data->GetType() != penvTI_Workspace);
            m_contextmenu->Enable(m_eventids[12], data->GetType() != penvTI_ProjectItem);
            m_contextmenu->Enable(m_eventids[13], true);
            m_contextmenu->Enable(m_eventids[14], false); // Add existing
            m_contextmenu->Enable(m_eventids[15], false); // Open external
            // Checks for 'open' context menu item
            bool enabled = false;
            if (list->GetCommand(_T("PENV_EVT_ACTIVATED_WORKSPACE"))->GetCallbacksCount() != 0 &&
                data->GetType() == penvTI_Workspace) {
                enabled = true;
            }
            if (list->GetCommand(_T("PENV_EVT_ACTIVATED_PROJECT"))->GetCallbacksCount() != 0 &&
                data->GetType() == penvTI_Workspace) {
                enabled = true;
            }
            if (data->GetType() == penvTI_ProjectItem) {
                if (data->GetProjectItem()->GetType() == penvPI_File &&
                    list->GetCommand(_T("PENV_EVT_ACTIVATED_PROJECTITEMFILE"))->GetCallbacksCount() != 0) {
                    enabled = true;
                }
                if (data->GetProjectItem()->GetType() == penvPI_Directory &&
                    list->GetCommand(_T("PENV_EVT_ACTIVATED_PROJECTITEMDIRECTORY"))->GetCallbacksCount() != 0) {
                    enabled = true;
                }
                if (data->GetProjectItem()->GetType() == penvPI_LinkedItems &&
                    list->GetCommand(_T("PENV_EVT_ACTIVATED_PROJECTITEMLINKEDITEMS"))->GetCallbacksCount() != 0) {
                    enabled = true;
                }
                if (data->GetProjectItem()->GetType() == penvPI_SubProject &&
                    list->GetCommand(_T("PENV_EVT_ACTIVATED_PROJECTITEMSUBPROJECT"))->GetCallbacksCount() != 0) {
                    enabled = true;
                }
            }
            m_contextmenu->Enable(m_eventids[16], enabled);
            m_contextmenu->Enable(m_eventids[17], false); // Paste
            m_contextmenu->Enable(m_eventids[18], false); // Cut
            m_contextmenu->Enable(m_eventids[19], false); // Copy
            m_contextmenu->Enable(m_eventids[20], false); // Delete
            m_contextmenu->Enable(m_eventids[22], false); // Properties
            m_treectrl->PopupMenu(m_contextmenu,wxDefaultPosition);
        }
    }
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnTreeItemKeyDown( wxTreeEvent& event )
/**
 * \brief Occurs, when a key is pressed in the tree
 * control. This method supports the keys: F2 for edit
 * a toolbar name; DEL for removing a toolbar or toolbaritem.
 * \param event Event.
 **/
{
    m_deletionyestoall = false;
    if(event.GetKeyCode() == WXK_F2)
    {
        m_treectrl->EditLabel(m_treectrl->GetSelection());
    }
    else if (event.GetKeyCode() == WXK_DELETE)
    {
        // TODO
    }
    else if (event.GetKeyCode() == WXK_RETURN)
    {
        // TODO
    }
}

//----------------------------------------------------------------
void ProjectTreeCtrl::OnToolBarItemAddClicked( wxCommandEvent& event )
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    m_deletionyestoall = false;
    wxString cmdid = _T("PENV_EVT_PROJECTTREECTRL_ADDCLICKED");
    Command* cmd = Environment::Get()->GetCommandList()->GetCommand(cmdid);
    if (cmd->GetCallbacksCount() == 0)
    {
        wxLogWarning(_T("[penv::ProjectTreeCtrl::OnToolBarItemAddClicked] No Callback for command '%s' registered."), cmdid.c_str());
        return;
    }
    //TODO SO: Eigene Klasse zum verwalten von hinzuzufügenden items.
    cmd->Execute();
}



//----------------------------------------------------------------
void ProjectTreeCtrl::OnToolBarItemDeleteClicked( wxCommandEvent& event )
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    m_deletionyestoall = false;
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnToolBarItemRefreshClicked( wxCommandEvent& event )
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    m_deletionyestoall = false;
    Environment* env = Environment::Get();
    bool modified = env->GetWorkspaceList()->IsWorkspaceOrProjectModified();
    if (modified)
    {
        int result = wxMessageBox(_("One or more Workspace or Project has been modified.\n Do you want to save?"),
            _("Question"), wxYES_NO | wxICON_QUESTION , env->GetFrame());
        if (result == wxYES)
        {
            NOT_IMPLEMENTED_YET();
            //TODO MS: SaveProjectWorkspaces or/and -Projects.
        }
    }
    env->GetWorkspaceList()->ReloadWorkspaces();
    m_justclear = true;
    m_treectrl->DeleteAllItems();
    m_justclear = false;
    m_treectrl->SetImageList(m_apperance->GetImageList());
    AddTreeItems(false);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnToolBarItemShowAllClicked( wxCommandEvent& event )
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    m_deletionyestoall = false;
    if (m_showall)
    {
        m_showall = false;
        // Remove files, which are not in the project
        NOT_IMPLEMENTED_YET();
    }
    else
    {
        m_showall = true;
        // Show every file, even if they are not in the project
        NOT_IMPLEMENTED_YET();
    }
}



//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemAddFileClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
    ProjectItemList* list = NULL;
    if (data->GetType() == penvTI_Project) {
        list = data->GetProject()->GetProjectItemList();
    }
    else if (data->GetType() == penvTI_ProjectItem) {
        if (data->GetProjectItem()->GetType() == penvPI_File) {
            list = data->GetProjectItem()->GetParent();
        }
        else if (data->GetProjectItem()->GetType() == penvPI_Directory) {
            list = data->GetProjectItem()->GetItemList();
        }
        else if (data->GetProjectItem()->GetType() == penvPI_LinkedItems) {
            NOT_IMPLEMENTED_YET();
            return;
        }
        else if (data->GetProjectItem()->GetType() == penvPI_SubProject) {
            NOT_IMPLEMENTED_YET();
            return;
        }
    }
    m_actionhandler->OnAdd(list, penvPI_File);
}



//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemAddDirectoryClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
    ProjectItemList* list = NULL;
    if (data->GetType() == penvTI_Project) {
        list = data->GetProject()->GetProjectItemList();
    }
    else if (data->GetType() == penvTI_ProjectItem) {
        if (data->GetProjectItem()->GetType() == penvPI_File) {
            list = data->GetProjectItem()->GetParent();
        }
        else if (data->GetProjectItem()->GetType() == penvPI_Directory) {
            list = data->GetProjectItem()->GetItemList();
        }
        else if (data->GetProjectItem()->GetType() == penvPI_LinkedItems) {
            NOT_IMPLEMENTED_YET();
            return;
        }
        else if (data->GetProjectItem()->GetType() == penvPI_SubProject) {
            NOT_IMPLEMENTED_YET();
            return;
        }
    }
    m_actionhandler->OnAdd(list, penvPI_Directory);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemAddProjectClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
    ProjectList* list = NULL;
    if (data->GetType() == penvTI_Project) {
        list = data->GetProject()->GetParent();
    }
    else if (data->GetType() == penvTI_Workspace) {
        list = data->GetWorkspace()->GetProjectList();
    }
    m_actionhandler->OnAdd(list);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemAddWorkspaceClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    WorkspaceList* list = Environment::Get()->GetWorkspaceList();
    m_actionhandler->OnAdd(list);
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemAddExistingClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemOpenExternalClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{

}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemOpenClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk()) return;
    ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetItemData(id);
    wxVariant variant = (void*)data;
    Environment::Get()->GetCommandList()
        ->Execute(_T("PENV_EVT_PROJECTTREECTRL_ITEM_ACTIVATED"), variant);

}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemPasteClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemCutClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemCopyClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemDeleteClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemRenameClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
    m_treectrl->EditLabel(m_treectrl->GetSelection());
}


//----------------------------------------------------------------
void ProjectTreeCtrl::OnContextMenuItemPropertiesClicked(wxCommandEvent& event)
/**
 * \brief Undocumented, because this will be implemented in
 * BuildInCommands.
 * \param event Event.
 **/
{
}





} // namespace penv


