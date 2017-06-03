/*
 * commandmenuwindow.h - Declaration of the CommandMenuWindow class
 *
 * Author: Steffen Ott
 * Date: 04/23/2008
 *
 */


#include "commandmenuwindow.h"
#include "../penvhelper.h"
#include "../commandlist.h"
#include "../environment.h"
#include "../menubar.h"
#include "commandmenutreedata.h"
#include "commandmenutreeviewdroptarget.h"


namespace penv {


//----------------------------------------------------------------
CommandMenuWindow::CommandMenuWindow()
    : wxPanel()
/**
 * \brief Constructor (private).
 **/
{
}



//----------------------------------------------------------------
CommandMenuWindow::CommandMenuWindow(wxWindow* parent, int id, wxPoint pos, wxSize size, int style)
    : wxPanel(parent, id, pos, size, style)
/**
 * \brief Constructor. Initialize the components in this panel.
 * \param parent Parent window of this panel.
 * \param id Window id for this panel.
 * \param pos Position of this panel.
 * \param size Size of this panel.
 * \param style Style of this panel.
 **/
{
    InitializeUI();
    DoLayout();
    ConnectEvents();
    UpdateComponents();
}



//----------------------------------------------------------------
CommandMenuWindow::~CommandMenuWindow()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
void CommandMenuWindow::InitializeUI()
/**
 * \brief Initialize all UI elements and set their properties.
 **/
{
	m_leftpanel = new wxPanel( this, wxID_ANY);
	m_labelmenu = new wxStaticText(m_leftpanel, wxID_ANY, wxT("Menu:"));
	m_labelmenu->Wrap( -1 );
	m_treectrl = new wxTreeCtrl( m_leftpanel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_SINGLE);
    m_treectrl->SetDropTarget(new CommandMenuTreeViewDropTarget(m_treectrl));
	m_rightpanel = new wxPanel(this, wxID_ANY);
	m_labelcommands = new wxStaticText(m_rightpanel, wxID_ANY, wxT("Commands:"));
	m_labelcommands->Wrap( -1 );
	m_comboboxfilter = new wxComboBox(m_rightpanel, wxID_ANY, wxT("<All Commands>"),
        wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_comboboxfilter->Append(_("<All Commands>"));
	m_comboboxfilter->SetValue(_("<All Commands>"));
	m_listctrl = new wxListCtrl(m_rightpanel, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);

    // Context Menu initalisation
    for (int i=0; i<penvCOMMANDMENUWINDOW_MAXCONTEXTMENUIDS; ++i) {
        m_contextmenuids[i] = wxNewId();
    }
    // Note: Do not change the index of a m_contextmenuids because you must
    //       change the id in every method where it will be used. Instead
    //       resize the m_contextmenuids via penvCOMMANDTOOLBARWINDOW_MAXCONTEXTMENUIDS
    //       and use other ids.
    m_contextmenu = new wxMenu();
    m_contextmenu->Append(m_contextmenuids[0], _("Add selected command"), _(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[1], _("Add sub menu"), _(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[2], _("Add separator"), _(""), wxITEM_NORMAL);
    m_contextmenu->AppendSeparator();
    m_contextmenu->Append(m_contextmenuids[3], _("Rename sub menu"), _(""), wxITEM_NORMAL);
    m_contextmenu->AppendSeparator();
    m_contextmenu->Append(m_contextmenuids[4], _("Remove"), _(""), wxITEM_NORMAL);
}


//----------------------------------------------------------------
void CommandMenuWindow::DoLayout()
/**
 * \brief Updates the layout for the components.
 **/
{
    wxFlexGridSizer* mainsizer;
	mainsizer = new wxFlexGridSizer( 1, 2, 0, 0 );
	mainsizer->AddGrowableCol( 0 );
	mainsizer->AddGrowableCol( 1 );
	mainsizer->AddGrowableRow( 0 );
	mainsizer->SetFlexibleDirection( wxBOTH );
	mainsizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* sizerleft;
	sizerleft = new wxFlexGridSizer( 2, 1, 0, 0 );
	sizerleft->AddGrowableCol( 0 );
	sizerleft->AddGrowableRow( 1 );
	sizerleft->SetFlexibleDirection( wxBOTH );
	sizerleft->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizerleft->Add( m_labelmenu, 0, wxALL, 5 );
	sizerleft->Add( m_treectrl, 0, wxALL|wxEXPAND, 5 );

	m_leftpanel->SetSizer( sizerleft );
	m_leftpanel->Layout();
	sizerleft->Fit( m_leftpanel );
	mainsizer->Add( m_leftpanel, 1, wxEXPAND | wxALL, 5 );
	wxFlexGridSizer* sizerright;
	sizerright = new wxFlexGridSizer( 3, 1, 0, 0 );
	sizerright->AddGrowableCol( 0 );
	sizerright->AddGrowableRow( 2 );
	sizerright->SetFlexibleDirection( wxBOTH );
	sizerright->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	sizerright->Add( m_labelcommands, 0, wxALL|wxEXPAND, 5 );
	sizerright->Add( m_comboboxfilter, 0, wxALL|wxEXPAND, 5 );
	sizerright->Add( m_listctrl, 0, wxALL|wxEXPAND, 5 );
	m_rightpanel->SetSizer( sizerright );
	m_rightpanel->Layout();
	sizerright->Fit( m_rightpanel );
	mainsizer->Add( m_rightpanel, 1, wxEXPAND | wxALL, 5 );
	this->SetSizer( mainsizer );
	this->Layout();
}


//----------------------------------------------------------------
void CommandMenuWindow::ConnectEvents()
/**
 * \brief Connect the callback methods with the components.
 **/
{
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_BEGIN_DRAG,
    //    wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeBeginDrag), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT,
        wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeBeginLabelEdit), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_DELETE_ITEM,
    //    wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeDeleteItem), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_END_DRAG,
    //    wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeEndDrag), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_END_LABEL_EDIT,
    //    wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeEndLabelEdit), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_COLLAPSED,
    //    wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeItemCollapsed), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_EXPANDING,
    //    wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeItemExpanding), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP,
    //    wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeItemGetTooltip), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_MENU,
        wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeItemMenu), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
        wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeItemRightClick), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_KEY_DOWN,
        wxTreeEventHandler(CommandMenuWindow::OnMenubarTreeKeyDown), NULL, this);
	m_comboboxfilter->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,
        wxCommandEventHandler(CommandMenuWindow::OnMenubarCombobox), NULL, this);
	m_listctrl->Connect(wxEVT_COMMAND_LIST_BEGIN_DRAG,
        wxListEventHandler(CommandMenuWindow::OnMenubarListBeginDrag), NULL, this);
	m_listctrl->Connect(wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT,
        wxListEventHandler(CommandMenuWindow::OnMenubarListBeginLabelEdit), NULL, this);
	//m_listctrl->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
    //    wxListEventHandler(CommandMenuWindow::OnMenubarListItemRightClick), NULL, this);

    m_contextmenu->Connect(m_contextmenuids[0], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandMenuWindow::OnContextMenuItemAddCommand), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[1], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandMenuWindow::OnContextMenuItemAddSubMenu), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[2], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandMenuWindow::OnContextMenuItemAddSeparator), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[3], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandMenuWindow::OnContextMenuItemRenameSubMenu), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[4], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandMenuWindow::OnContextMenuItemRemove), NULL, this);

}


//----------------------------------------------------------------
void CommandMenuWindow::UpdateComponents()
/**
 * \brief Updates the content of the components.
 **/
{
    CommandList* cmdlist = Environment::Get()->GetCommandList();
    MenuBar* menubar = cmdlist->GetMenuBar();
    // Commandliste füllen
    wxArrayString* strgarray = cmdlist->GetCommands();
    m_listctrl->InsertColumn(0, _("Command"), 0, 300);
    m_listctrl->InsertColumn(1, _("ID"), 0, 100);
    wxImageList* imagelist = new wxImageList(
        penvCOMMON_TOOLBARICONSIZE.GetWidth(), penvCOMMON_TOOLBARICONSIZE.GetHeight());
    m_listctrl->SetImageList(imagelist, wxIMAGE_LIST_SMALL);
    for (size_t i=0; i<strgarray->Count(); ++i)
    {
        Command* cmd = cmdlist->GetCommand((*strgarray)[i]);
        if (cmd->IsEvent()) continue;
        int imageindex = -1;
        if (cmd->GetIcon().IsOk())
        {
            imageindex = imagelist->GetImageCount();
            imagelist->Add(cmd->GetIcon());
        }
        int index = m_listctrl->GetItemCount();
        m_listctrl->InsertItem(index, cmd->GetName(), imageindex);
        m_listctrl->SetItem(index, 1, cmd->GetId(), -1);
    }
    // TODO SO
    // m_listctrl->SortItems(&ListComparer, 1);
    if (strgarray != NULL) delete strgarray;
    // Treeview füllen
    //Update Menubar into treectrl
    m_treectrl->DeleteAllItems();
    // Zuerst das Wurzel Element erstellen
    wxTreeItemId rootid = m_treectrl->AddRoot(_T("MenuBar"), -1, -1, NULL);
    // Toolbar IDs besorgen und durchloopen
    for(size_t i = 0; i < menubar->Count(); i++)
    {
        MenuItem* menuitem = menubar->GetItem(i);
        AddTreeItem(rootid, menuitem);
    }
    m_treectrl->Expand(rootid);
    m_treectrl->Update();
}


//----------------------------------------------------------------
void CommandMenuWindow::AddTreeItem(wxTreeItemId id, MenuItem* item)
/**
 * \brief Adds a treeitem to the tree control.
 * \param id Parent id of the tree item.
 * \param item Menu item which should be added.
 **/
{
    if(item->GetType() == penvMI_Submenu)
    {
        int imageindex = -1;
        wxTreeItemId menuitemid = m_treectrl->AppendItem(id, item->GetName(),
            imageindex, imageindex, new CommandMenuTreeData(penvMI_Submenu, item));
        for(size_t i = 0; i<item->Count(); i++)
        {
            MenuItem* menuitem = item->GetItem(i);
            AddTreeItem(menuitemid, menuitem);
        }
    }
    else if(item->GetType() == penvMI_Command)
    {
        int imageindex = -1;
        m_treectrl->AppendItem(id, item->GetName(), imageindex, imageindex,
            new CommandMenuTreeData(penvMI_Command, item));
    }
    else if(item->GetType() == penvMI_Checkbox)
    {
        NOT_IMPLEMENTED_YET();
    }
    else if(item->GetType() == penvMI_Radio)
    {
        NOT_IMPLEMENTED_YET();
    }
    else if(item->GetType() == penvMI_Separator)
    {
        m_treectrl->AppendItem(id, _T("------------------"), -1, -1, new CommandMenuTreeData(penvMI_Separator, item));
    }

}



//----------------------------------------------------------------
void CommandMenuWindow::SaveAndUpdate()
/**
 * \brief Saves and updates the adjusted menubar.
 **/
{
    // Remove all items from menubar
    MenuBar* menubar = Environment::Get()->GetCommandList()->GetMenuBar();
    menubar->Clear();

    // Insert all items
    wxTreeItemId rootid = m_treectrl->GetRootItem();
    unsigned int cookievalue = 0xC00C1E0;
    wxTreeItemIdValue cookie = (wxTreeItemIdValue)cookievalue;
    wxTreeItemId menuitemid = m_treectrl->GetFirstChild(rootid, cookie);
    while (menuitemid.IsOk())
    {
        // Get Tree Item Data
        CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(menuitemid);
        if (data == NULL) {
            wxLogError(_T("[penv::CommandMenuWindow::SaveAndUpdate] Tree item data is NULL for item '%s'."), m_treectrl->GetItemText(menuitemid).c_str());
            menuitemid = m_treectrl->GetNextChild(menuitemid, cookie);
            continue;
        }
        if (data->GetType() != penvMI_Submenu) {
            wxLogError(_T("[penv::CommandMenuWindow::SaveAndUpdate] Tree item data must be a submenu in the first level of the tree."));
            menuitemid = m_treectrl->GetNextChild(menuitemid, cookie);
            continue;
        }
        MenuItem* item = new MenuItem(penvMI_Submenu, NULL);
        item->SetName(m_treectrl->GetItemText(menuitemid));
        menubar->Add(item);
        SaveAndUpdate(item, menuitemid, (unsigned int)cookie);
        // Move to the next menuitem
        menuitemid = m_treectrl->GetNextChild(menuitemid, cookie);
    }
    menubar->Update();
}


//----------------------------------------------------------------
void CommandMenuWindow::SaveAndUpdate(MenuItem* item, wxTreeItemId node, unsigned int lastcookie)
/**
 * \brief Saves and updates a single menuitem.
 * \param item Actual submenu item to add items.
 * \param node The actual treenode.
 * \param lastcookie The last cookie number.
 **/
{
    wxTreeItemIdValue cookie = (wxTreeItemIdValue)(lastcookie+1);
    wxTreeItemId menuitemid = m_treectrl->GetFirstChild(node, cookie);
    while (menuitemid.IsOk())
    {
        // Get Tree Item Data
        CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(menuitemid);
        if (data == NULL) {
            wxLogError(_T("[penv::CommandMenuWindow::SaveAndUpdate] Tree item data is NULL for item '%s'."), m_treectrl->GetItemText(menuitemid).c_str());
            menuitemid = m_treectrl->GetNextChild(menuitemid, cookie);
            continue;
        }
        if (data->GetType() == penvMI_Command)
        {
            Command* cmd = Environment::Get()->GetCommandList()->GetCommand(data->GetCommandId());
            if (cmd == NULL) {
                wxLogError(_T("[penv::CommandMenuWindow::SaveAndUpdate] Command with id '%s' does not exist for tree item '%s'."), data->GetCommandId().c_str(), m_treectrl->GetItemText(menuitemid).c_str());
                menuitemid = m_treectrl->GetNextChild(menuitemid, cookie);
                continue;
            }
            MenuItem* cmditem = new MenuItem(penvMI_Command, cmd);
            item->Add(cmditem);
        }
        else if (data->GetType() == penvMI_Checkbox)
        {
            NOT_IMPLEMENTED_YET();
        }
        else if (data->GetType() == penvMI_Radio)
        {
            NOT_IMPLEMENTED_YET();
        }
        else if (data->GetType() == penvMI_Separator)
        {
            MenuItem* cmditem = new MenuItem(penvMI_Separator, NULL);
            item->Add(cmditem);
        }
        else if (data->GetType() == penvMI_Submenu)
        {
            MenuItem* subitem = new MenuItem(penvMI_Submenu, NULL);
            subitem->SetName(m_treectrl->GetItemText(menuitemid));
            item->Add(subitem);
            SaveAndUpdate(subitem, menuitemid, (unsigned int)cookie);
        }
        // Move to the next menuitem
        menuitemid = m_treectrl->GetNextChild(menuitemid, cookie);
    }
}





//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeBeginDrag(wxTreeEvent& event)
/**
 * \brief Occurs when a tree control item will be started to dragged.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeBeginLabelEdit(wxTreeEvent& event)
/**
 * \brief Occurs when a item in the tree control starts
 * to be edited. Simply checks if this item is a submenu
 * item. The root and other menu items are not allowed to
 * be edited.
 * \param event Event.
 **/
{
    wxTreeItemId id = event.GetItem();
    if (!id.IsOk()) return;
    CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
    if (data != NULL) {
        if (data->GetType() == penvMI_Submenu) {
            return;
        }
    }
    event.Veto();
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeDeleteItem(wxTreeEvent& event)
/**
 * \brief Occurs when a tree control item will be removed.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}



//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeEndDrag(wxTreeEvent& event)
/**
 * \brief Occurs, when a drag and drop operation from the tree control ends.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}



//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeEndLabelEdit(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item ends the editing.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeItemCollapsed(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item is collapsed.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeItemExpanding(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item is expanding.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeItemGetTooltip(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item should return its tooltip.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeItemMenu(wxTreeEvent& event)
/**
 * \brief Occurs when the menu of the tree control should be opend.
 * This method enables or disables context menu items and shows
 * the context menu.
 * \param event Event.
 **/
{
    wxPoint location = event.GetPoint();
    location.x += 10;
    location.y += 30;
    wxTreeItemId id = m_treectrl->GetSelection();
    // Enable or disable context menu items
    m_contextmenu->Enable(m_contextmenuids[0], false);
    m_contextmenu->Enable(m_contextmenuids[1], true);
    m_contextmenu->Enable(m_contextmenuids[2], false);
    m_contextmenu->Enable(m_contextmenuids[3], false);
    m_contextmenu->Enable(m_contextmenuids[4], false);
    // 'Add selected command' depends on if a item is selected
    //   in the listview and if the item is a menuitem
    // 'Add sub menu' depends on if a item is a menuitem or tree root
    // 'Add separator' depends on if a item is a menuitem
    // 'Rename sub menu' depends on if a item is a sub menuitem
    // 'Remove' depends on if a item is a menuitem
    if (!id.IsOk()) return;
    if (m_treectrl->GetRootItem() != id)
    {
        CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
        if (data->GetType() == penvMI_Submenu)
        {
            m_contextmenu->Enable(m_contextmenuids[3], true);
        }
        m_contextmenu->Enable(m_contextmenuids[2], true);
        m_contextmenu->Enable(m_contextmenuids[4], true);
        if (m_listctrl->GetSelectedItemCount() >= 1)
                m_contextmenu->Enable(m_contextmenuids[0], true);
    }
    // Show context menu
    this->PopupMenu(m_contextmenu, location);
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarTreeItemRightClick(wxTreeEvent& event)
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
void CommandMenuWindow::OnMenubarTreeKeyDown(wxTreeEvent& event)
/**
 * \brief Occurs, when a key is pressed in the tree
 * control. This method supports the keys: F2 for edit
 * a submenu name; DEL for removing a menuitem.
 **/
{
    if (event.GetKeyEvent().GetKeyCode() == WXK_F2)
    {
        wxTreeItemId id = m_treectrl->GetSelection();
        if (!id.IsOk() || m_treectrl->GetRootItem() == id) return;
        CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
        if (data->GetType() == penvMI_Submenu)
        {
            m_treectrl->EditLabel(id);
        }
    }
    else if (event.GetKeyEvent().GetKeyCode() == WXK_DELETE)
    {
        wxTreeItemId id = m_treectrl->GetSelection();
        if (id.IsOk() && m_treectrl->GetRootItem() != id)
        {
            CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
            if (data->GetType() == penvMI_Submenu)
            {
                // Remove MenuItem Submenu
                int result = wxMessageBox(_("Are you sure to remove the whole sub menu item?"),
                    _("Remove sub menu item ...?"), wxYES_NO|wxICON_QUESTION, this);
                if (result == wxYES)
                {
                    m_treectrl->DeleteChildren(id);
                    m_treectrl->Delete(id);
                    m_treectrl->Update();
                }
            }
            else
            {
                // Remove Menu Item
                m_treectrl->Delete(id);
                m_treectrl->Update();
            }
        }
    }
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarCombobox(wxCommandEvent& event)
/**
 * \brief Occurs when another item in the combobox is selected.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}



//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarListBeginDrag(wxListEvent& event)
/**
 * \brief Occurs when the user begin to drag an item from the list control.
 * Stores the command id in the data object and begins the drag and
 * drop operation.
 * \param event Event.
 **/
{
    if (event.GetIndex() >= 0)
    {
        wxListItem item;
        item.SetColumn(1);
        item.SetId(event.GetIndex());
        item.SetMask(wxLIST_MASK_TEXT);
        m_listctrl->GetItem(item);
        wxString cmdid = item.GetText();
        wxTextDataObject object(cmdid);
        wxDropSource source(object, m_treectrl);
        source.DoDragDrop(true);
    }
}


//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarListBeginLabelEdit(wxListEvent& event)
/**
 * \brief Occurs when the user begin to edit a list control item.
 * Always vetoes the event.
 * \param event Event.
 **/
{
    event.Veto();
}

//----------------------------------------------------------------
void CommandMenuWindow::OnMenubarListItemRightClick(wxListEvent& event)
/**
 * \brief Occurs when the user right clicks on the list control.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandMenuWindow::OnContextMenuItemAddCommand(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'add selected command' is clicked.
 * Adds the selected command from the list control to the toolbar at the
 * selected position.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk()) return;
    if (m_treectrl->GetRootItem() == id) return;
    // Get the selected command id from list control
    long itemid = m_listctrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    wxListItem item;
    item.SetColumn(1);
    item.SetMask(wxLIST_MASK_TEXT);
    item.SetId(itemid);
    m_listctrl->GetItem(item);
    wxString cmdid = item.GetText();
    Command* command = Environment::Get()->GetCommandList()->GetCommand(cmdid);
    CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
    if (data->GetType() == penvMI_Submenu)
    {
        // Add command at the end of the sub menu item
        int imageindex = -1;
        m_treectrl->AppendItem(id, command->GetName(), imageindex,
            imageindex, new CommandMenuTreeData(penvMI_Command, command->GetId()));
        m_treectrl->Update();
    }
    else
    {
        // Insert command at the selected position
        int imageindex = -1;
        m_treectrl->InsertItem(m_treectrl->GetItemParent(id),
            id, command->GetName(), imageindex, imageindex,
            new CommandMenuTreeData(penvMI_Command, command->GetId()));
        m_treectrl->Update();
    }
}


//----------------------------------------------------------------
void CommandMenuWindow::OnContextMenuItemAddSubMenu(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'add sub menu' is clicked.
 * Adds a sub menu item to the menubar at the selected position.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk()) return;
    if (m_treectrl->GetRootItem() == id)
    {
        MenuItem* newmenuitem = new MenuItem(penvMI_Submenu, NULL);
        newmenuitem->SetName(_("new sub menu"));
        wxTreeItemId newitem;
        newitem = m_treectrl->AppendItem(id, newmenuitem->GetName(), -1, -1,
            new CommandMenuTreeData(penvMI_Submenu, newmenuitem));
        m_treectrl->EditLabel(newitem);
    }
    else
    {
        CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
        MenuItem* newmenuitem = new MenuItem(penvMI_Submenu, NULL);
        newmenuitem->SetName(_("new sub menu"));
        wxTreeItemId newitem;
        if (data->GetType() == penvMI_Submenu)
        {
            // Add submenu at the end of the menubar
            newitem = m_treectrl->AppendItem(id, newmenuitem->GetName(), -1, -1,
                new CommandMenuTreeData(penvMI_Submenu, newmenuitem));
            m_treectrl->Update();
        }
        else
        {
            // Insert submenu at the selected position
            newitem = m_treectrl->InsertItem(m_treectrl->GetItemParent(id),
                id, newmenuitem->GetName(), -1, -1,
                new CommandMenuTreeData(penvMI_Submenu, newmenuitem));
            m_treectrl->Update();
        }
        m_treectrl->EditLabel(newitem);
    }
}


//----------------------------------------------------------------
void CommandMenuWindow::OnContextMenuItemAddSeparator(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'add separator' is clicked.
 * Adds a separator to the menubar at the selected position.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk()) return;
    if (m_treectrl->GetRootItem() == id) return;
    CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
    if (data->GetType() == penvMI_Submenu)
    {
        // Add separator at the end of the toolbar
        m_treectrl->AppendItem(id, _T("------------------"), -1, -1,
            new CommandMenuTreeData(penvMI_Separator, NULL));
        m_treectrl->Update();
    }
    else
    {
        // Insert separtor at the selected position
        m_treectrl->InsertItem(m_treectrl->GetItemParent(id),
            id, _T("------------------"), -1, -1,
            new CommandMenuTreeData(penvMI_Separator, NULL));
        m_treectrl->Update();
    }
}


//----------------------------------------------------------------
void CommandMenuWindow::OnContextMenuItemRenameSubMenu(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'rename' is clicked.
 * Begins editing of the submenu item name.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk()) return;
    CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
    if (data->GetType() == penvMI_Submenu)
    {
        m_treectrl->EditLabel(id);
    }
}


//----------------------------------------------------------------
void CommandMenuWindow::OnContextMenuItemRemove(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'remove' is clicked.
 * Deletes a menuitem.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk() || m_treectrl->GetRootItem() == id) return;
    CommandMenuTreeData* data = (CommandMenuTreeData*)m_treectrl->GetItemData(id);
    if (data->GetType() == penvMI_Submenu)
    {
        // Remove ToolBar
        int result = wxMessageBox(_("Are you sure to remove the whole sub menu item?"),
            _("Remove sub menu item ...?"), wxYES_NO|wxICON_QUESTION, this);
        if (result == wxYES)
        {
            m_treectrl->DeleteChildren(id);
            m_treectrl->Delete(id);
            m_treectrl->Update();
        }
    }
    else
    {
        // Remove ToolBar item
        m_treectrl->Delete(id);
        m_treectrl->Update();
    }
}




} // namespace penv



