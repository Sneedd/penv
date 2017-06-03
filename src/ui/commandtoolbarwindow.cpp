/*
 * commandtoolbarwindow.cpp - Implementation of CommandToolBarWindow class
 *
 * Author: Steffen Ott
 * Date: 03/18/2008
 *
 */



#include "commandtoolbarwindow.h"
#include "../environment.h"
#include "../penvhelper.h"
#include "commandtoolbartreeviewdroptarget.h"
#include "commandtoolbartreedata.h"


namespace penv {



//----------------------------------------------------------------
CommandToolBarWindow::CommandToolBarWindow()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
CommandToolBarWindow::CommandToolBarWindow(wxWindow* parent, int id, wxPoint pos, wxSize size, int style)
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
CommandToolBarWindow::~CommandToolBarWindow()
/**
 * \brief Destructor.
 **/
{
    if (m_contextmenu != NULL) delete m_contextmenu;
    if (m_listctrl->GetImageList(wxIMAGE_LIST_SMALL) != NULL) delete m_listctrl->GetImageList(wxIMAGE_LIST_SMALL);
}



//----------------------------------------------------------------
void CommandToolBarWindow::InitializeUI()
/**
 * \brief Initialize all UI elements and set their properties.
 **/
{
	m_leftpanel = new wxPanel(this, wxID_ANY);
    m_labeltoolbars = new wxStaticText(m_leftpanel, wxID_ANY, wxT("Toolbars:"));
	m_labeltoolbars->Wrap(-1);
	m_treectrl = new wxTreeCtrl(m_leftpanel, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_SINGLE);
    m_treectrl->SetDropTarget(new CommandToolBarTreeViewDropTarget(m_treectrl));
	m_rightpanel = new wxPanel(this, wxID_ANY);
	m_labelcommands = new wxStaticText(m_rightpanel, wxID_ANY, wxT("Commands:"));
	m_labelcommands->Wrap(-1);
	m_comboboxfilter = new wxComboBox(m_rightpanel, wxID_ANY, wxT("<All Commands>"),
        wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_comboboxfilter->Append(_("<All Commands>"));
	m_comboboxfilter->SetValue(_("<All Commands>"));
	m_listctrl = new wxListCtrl(m_rightpanel, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);

    // Context Menu initalisation
    for (int i=0; i<penvCOMMANDTOOLBARWINDOW_MAXCONTEXTMENUIDS; ++i) {
        m_contextmenuids[i] = wxNewId();
    }
    // Note: Do not change the index of a m_contextmenuids because you must
    //       change the id in every method where it will be used. Instead
    //       resize the m_contextmenuids via penvCOMMANDTOOLBARWINDOW_MAXCONTEXTMENUIDS
    //       and use other ids.
    m_contextmenu = new wxMenu();
    m_contextmenu->Append(m_contextmenuids[0], _("Add selected command"), _(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[4], _("Add toolbar"), _(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[1], _("Add separator"), _(""), wxITEM_NORMAL);
    m_contextmenu->AppendSeparator();
    m_contextmenu->Append(m_contextmenuids[2], _("Rename toolbar"), _(""), wxITEM_NORMAL);
    m_contextmenu->AppendSeparator();
    m_contextmenu->Append(m_contextmenuids[3], _("Remove"), _(""), wxITEM_NORMAL);




}


//----------------------------------------------------------------
void CommandToolBarWindow::DoLayout()
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
	sizerleft->Add( m_labeltoolbars, 0, wxALL, 5 );
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
void CommandToolBarWindow::ConnectEvents()
/**
 * \brief Connect the callback methods with the components.
 **/
{
    if (m_treectrl == NULL || m_comboboxfilter == NULL)
    {
        wxLogError(_T("[penv::ToolBarWindow::ConnectEvents] Can't connect events because, panel components not initialized."));
        return;
    }
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler(
    //    CommandToolBarWindow::OnToolbarsTreeBeginDrag), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, wxTreeEventHandler(
        CommandToolBarWindow::OnToolbarsTreeBeginLabelEdit), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_DELETE_ITEM, wxTreeEventHandler(
        CommandToolBarWindow::OnToolbarsTreeDeleteItem), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler(
    //    CommandToolBarWindow::OnToolbarsTreeEndDrag), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler(
    //    CommandToolBarWindow::OnToolbarsTreeEndLabelEdit), NULL, this);
    //m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_COLLAPSED, wxTreeEventHandler(
    //    CommandToolBarWindow::OnToolbarsItemCollapsed), NULL, this);
    //m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_EXPANDING, wxTreeEventHandler(
    //    CommandToolBarWindow::OnToolbarsItemExpanding), NULL, this);
	//m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP, wxTreeEventHandler(
    //    CommandToolBarWindow::OnToolbarsTreeItemGetTooltip), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler(
        CommandToolBarWindow::OnToolbarsTreeItemMenu), NULL, this);
	m_treectrl->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler(
        CommandToolBarWindow::OnToolbarsTreeItemRightClick), NULL, this);
    m_treectrl->Connect(wxEVT_COMMAND_TREE_KEY_DOWN, wxTreeEventHandler(
        CommandToolBarWindow::OnToolbarsTreeKeyDown), NULL, this);
	m_listctrl->Connect(wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler(
        CommandToolBarWindow::OnToolbarsListBeginDrag), NULL, this);
	m_listctrl->Connect(wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT, wxListEventHandler(
        CommandToolBarWindow::OnToolbarsListBeginLabelEdit), NULL, this);
	//m_listctrl->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(
    //    CommandToolBarWindow::OnToolbarsListItemRightClick), NULL, this);
	//m_comboboxfilter->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(
    //    CommandToolBarWindow::OnToolbarsCombobox), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[0], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandToolBarWindow::OnContextMenuItemAddCommand), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[1], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandToolBarWindow::OnContextMenuItemAddSeparator), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[2], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandToolBarWindow::OnContextMenuItemRenameToolbar), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[3], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandToolBarWindow::OnContextMenuItemRemove), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[4], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(CommandToolBarWindow::OnContextMenuItemAddToolBar), NULL, this);

}


//----------------------------------------------------------------
int wxCALLBACK ListComparer(long item1, long item2, long sortdata)
/**
 * \brief
 **/
{
    // TODO SO: Sorting
    //    wxListCtrl* list = this->m_listctrl;
    //    wxListItem listitem1;
    //    wxListItem listitem2;
    //    listitem1.SetId(item1);
    //    listitem2.SetId(item2);
    //    list->GetItem(listitem1);
    //    list->GetItem(listitem2);
    //    return (listitem1.GetText().Cmp(listitem2.GetText()));
    return (0);
}



//----------------------------------------------------------------
void CommandToolBarWindow::UpdateComponents()
/**
 * \brief Updates the content of the components.
 **/
{
    CommandList* cmdlist = Environment::Get()->GetCommandList();
    ToolBarList* tblist = cmdlist->GetToolBarList();
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
        if (!cmd->GetIcon().IsOk()) continue;
        int imageindex = imagelist->GetImageCount();
        imagelist->Add(cmd->GetIcon());
        int index = m_listctrl->GetItemCount();
        m_listctrl->InsertItem(index, cmd->GetName(), imageindex);
        m_listctrl->SetItem(index, 1, cmd->GetId(), -1);
    }
    m_listctrl->SortItems(&ListComparer, 1);
    if (strgarray != NULL) delete strgarray;
    // Treeview füllen
    // Zuerst das Wurzel Element erstellen
    wxTreeItemId rootid = m_treectrl->AddRoot(_T("ToolBars"), -1, -1, NULL);
    // Toolbar IDs besorgen und durchloopen
    strgarray = tblist->GetToolBars();
    for (size_t i=0; i<strgarray->Count(); ++i)
    {
        // Gefundene Toolbar hinzufügen
        ToolBar* tbar = tblist->GetToolBar((*strgarray)[i]);
        wxTreeItemId toolbarid = m_treectrl->AppendItem(rootid, tbar->GetName(),
            -1, -1, new CommandToolBarTreeData(tbar->GetId()));
        // Durch die Toolbar Items loopen und hinzufügen
        for (size_t j=0; j<tbar->Count(); ++j)
        {
            // TODO SO: Image
            int tbimageindex = -1;
            // ToolBarItem holen und hinzufügen
            ToolBarItem* item = tbar->GetItem(j);
            if (item->IsSeparator())
            {
                m_treectrl->AppendItem(toolbarid, _T("------------------"), -1, -1, NULL);
            }
            else
            {
                m_treectrl->AppendItem(toolbarid, item->GetName(), tbimageindex,
                    tbimageindex, new CommandToolBarTreeData(item->GetId()));
            }
        }
    }
    m_treectrl->Expand(rootid);
    if (strgarray != NULL) delete strgarray;
}


//----------------------------------------------------------------
void CommandToolBarWindow::SaveAndUpdate()
/**
 * \brief Saves and updates the adjusted toolbar.
 **/
{
    CommandList* cmdlist = Environment::Get()->GetCommandList();
    ToolBarList* tblist = cmdlist->GetToolBarList();

    wxArrayString* toolbarstringids = tblist->GetToolBars();

    wxTreeItemId rootid = m_treectrl->GetRootItem();
    wxTreeItemIdValue cookie = (wxTreeItemIdValue)0xC00C1E;
    wxTreeItemId toolbarid = m_treectrl->GetFirstChild(rootid, cookie);
    while (toolbarid.IsOk())
    {
        // Check if toolbar exists, newly added toolbars have empty ids
        CommandToolBarTreeData* data = (CommandToolBarTreeData*)m_treectrl->GetItemData(toolbarid);
        ToolBar* toolbar = NULL;
        if (data->GetToolBarId().IsEmpty())
        {
            // NOTE: ToolBar ID must be empty to generate a new unique ID
            toolbar = new ToolBar();
            wxString name = m_treectrl->GetItemText(toolbarid);
            toolbar->SetName(name);
            tblist->Add(toolbar);
        }
        else
        {
            toolbar = tblist->GetToolBar(data->GetToolBarId());
        }
        // Refresh toolbar items
        // Because it is hard to check which items are edited,
        //  removed or added update the whole toolbar
        wxTreeItemIdValue cookieitem = (wxTreeItemIdValue)0xC00C1E2;
        wxTreeItemId itemid = m_treectrl->GetFirstChild(toolbarid, cookieitem);
        toolbar->Clear();
        while (itemid.IsOk())
        {
            CommandToolBarTreeData* itemdata = (CommandToolBarTreeData*)m_treectrl->GetItemData(itemid);
            if (itemdata == NULL)
            {
                // Add Separator
                ToolBarItem* item = new ToolBarItem(NULL);
                toolbar->Add(item);
            }
            else
            {
                Command* cmd = cmdlist->GetCommand(itemdata->GetCommandId());
                ToolBarItem* item = new ToolBarItem(cmd);
                toolbar->Add(item);
            }
            // Move to the next item
            itemid = m_treectrl->GetNextChild(itemid, cookieitem);
        }
        // Move to the next toolbar
        toolbarid = m_treectrl->GetNextChild(toolbarid, cookie);
    }
    delete toolbarstringids;
    // Remove the 'removed' toolbars
    for (size_t i=0; i<m_removedtoolbars.Count(); ++i)
    {
        tblist->Remove(m_removedtoolbars[i]);
    }
    m_removedtoolbars.Clear();
    tblist->Update();
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeBeginDrag(wxTreeEvent& event)
/**
 * \brief Occurs when a tree control item will be started to dragged.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeBeginLabelEdit(wxTreeEvent& event)
/**
 * \brief Occurs when a item in the tree control starts
 * to be edited. Simply checks if this item is a toolbar.
 * The root and toolbar items are not allowed to be edited.
 * \param event Event.
 **/
{
    wxTreeItemId id = event.GetItem();
    if (id.IsOk() && m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
    {
        return;
    }
    event.Veto();
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeDeleteItem(wxTreeEvent& event)
/**
 * \brief Occurs when a tree control item will be removed.
 * Saves the deleted toolbar id, if a toolbar is deleted.
 * \param event Event.
 **/
{
    wxTreeItemId id = event.GetItem();
    if (id.IsOk() && m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
    {
        CommandToolBarTreeData* data = (CommandToolBarTreeData*)m_treectrl->GetItemData(id);
        if (!data->GetToolBarId().IsEmpty()) {
            m_removedtoolbars.Add(data->GetToolBarId());
        }
    }
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeEndDrag(wxTreeEvent& event)
/**
 * \brief Occurs, when a drag and drop operation from the tree control ends.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeEndLabelEdit(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item ends the editing.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}



//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsItemCollapsed(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item is collapsed.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsItemExpanding(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item is expanding.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeItemGetTooltip(wxTreeEvent& event)
/**
 * \brief Occurs, when a tree control item should return its tooltip.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeItemMenu(wxTreeEvent& event)
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
    m_contextmenu->Enable(m_contextmenuids[1], false);
    m_contextmenu->Enable(m_contextmenuids[2], false);
    m_contextmenu->Enable(m_contextmenuids[3], false);
    // 'Add selected command' depends on if a item is selected
    //   in the listview and if the item is a toolbar or toolbaritem
    // 'Add separator' depends on if a item is a toolbar or toolbaritem
    // 'Rename toolbar' depends on if a item is a toolbar
    // 'Remove' depends on if a item is a toolbar or toolbaritem
    if (!id.IsOk()) return;
    if (m_treectrl->GetRootItem() != id)
    {
        if (m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
        {
            // Selection is a toolbar
            m_contextmenu->Enable(m_contextmenuids[1], true);
            m_contextmenu->Enable(m_contextmenuids[2], true);
            m_contextmenu->Enable(m_contextmenuids[3], true);
            if (m_listctrl->GetSelectedItemCount() >= 1)
                m_contextmenu->Enable(m_contextmenuids[0], true);
        }
        else
        {
            // Selection is a toolbar item
            m_contextmenu->Enable(m_contextmenuids[1], true);
            m_contextmenu->Enable(m_contextmenuids[3], true);
            if (m_listctrl->GetSelectedItemCount() >= 1)
                m_contextmenu->Enable(m_contextmenuids[0], true);
        }
    }
    // Show context menu
    this->PopupMenu(m_contextmenu, location);
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsTreeItemRightClick(wxTreeEvent& event)
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
void CommandToolBarWindow::OnToolbarsTreeKeyDown(wxTreeEvent& event)
/**
 * \brief Occurs, when a key is pressed in the tree
 * control. This method supports the keys: F2 for edit
 * a toolbar name; DEL for removing a toolbar or toolbaritem.
 * \param event Event.
 **/
{
    if (event.GetKeyEvent().GetKeyCode() == WXK_F2)
    {
        wxTreeItemId id = m_treectrl->GetSelection();
        if (id.IsOk() && m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
        {
            m_treectrl->EditLabel(id);
        }
    }
    else if (event.GetKeyEvent().GetKeyCode() == WXK_DELETE)
    {
        wxTreeItemId id = m_treectrl->GetSelection();
        if (id.IsOk() && m_treectrl->GetRootItem() != id)
        {
            if (m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
            {
                // Remove ToolBar
                int result = wxMessageBox(_("Are you sure to remove the whole toolbar?"),
                    _("Remove toolbar ...?"), wxYES_NO|wxICON_QUESTION, this);
                if (result == wxYES)
                {
                    m_treectrl->DeleteChildren(id);
                    m_treectrl->Delete(id);
                }
            }
            else
            {
                // Remove ToolBar item
                m_treectrl->Delete(id);
            }
        }
    }
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsCombobox(wxCommandEvent& event)
/**
 * \brief Occurs when another item in the combobox is selected.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}



//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsListBeginDrag(wxListEvent& event)
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
void CommandToolBarWindow::OnToolbarsListBeginLabelEdit(wxListEvent& event)
/**
 * \brief Occurs when the user begin to edit a list control item.
 * Always vetoes the event.
 * \param event Event.
 **/
{
    event.Veto();
}



//----------------------------------------------------------------
void CommandToolBarWindow::OnToolbarsListItemRightClick(wxListEvent& event)
/**
 * \brief Occurs when the user right clicks on the list control.
 * This callback is not registered and will not be used.
 * \param event Event.
 **/
{
    // Not used
}



//----------------------------------------------------------------
void CommandToolBarWindow::OnContextMenuItemAddCommand(wxCommandEvent& event)
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
    // Slightly differen functionality if we are
    //  on a toolbar or toolbar item
    if (m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
    {
        // Add command at the end of the toolbar
        int imageindex = -1;
        m_treectrl->AppendItem(id, command->GetName(), imageindex,
            imageindex, new CommandToolBarTreeData(command->GetId()));
    }
    else
    {
        // Insert command at the selected position
        int imageindex = -1;
        m_treectrl->InsertItem(m_treectrl->GetItemParent(id),
            id, command->GetName(), imageindex, imageindex,
            new CommandToolBarTreeData(command->GetId()));
    }
}




//----------------------------------------------------------------
void CommandToolBarWindow::OnContextMenuItemAddToolBar(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'add toolbar' is clicked.
 * Adds a toolbar but only to the root node or inserts under a toolbar.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk()) return;
    if (m_treectrl->GetRootItem() == id)
    {
        // Append toolbar
        // NOTE: ToolBar ID must be empty to generate a new unique ID
        wxTreeItemId newid = m_treectrl->AppendItem(id, _T("new toolbar"), -1, -1,
            new CommandToolBarTreeData(wxEmptyString));
        m_treectrl->Expand(id);
        m_treectrl->EditLabel(newid);

    }
    else if (m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
    {
        // Inserts toolbar under actual node
        // NOTE: ToolBar ID must be empty to generate a new unique ID
        wxTreeItemId newid = m_treectrl->InsertItem(m_treectrl->GetItemParent(id),
            id, _T("new toolbar"), -1, -1,
            new CommandToolBarTreeData(wxEmptyString));
        m_treectrl->EditLabel(newid);
    }
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnContextMenuItemAddSeparator(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'add separator' is clicked.
 * Adds a separator to the toolbar at the selected position.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (!id.IsOk()) return;
    if (m_treectrl->GetRootItem() == id) return;
    if (m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
    {
        // Add separator at the end of the toolbar
        m_treectrl->AppendItem(id, _T("------------------"), -1, -1, NULL);
    }
    else
    {
        // Insert separtor at the selected position
        m_treectrl->InsertItem(m_treectrl->GetItemParent(id),
            id, _T("------------------"), -1, -1, NULL);
    }
}




//----------------------------------------------------------------
void CommandToolBarWindow::OnContextMenuItemRenameToolbar(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'rename' is clicked.
 * Begins editing of the toolbar name.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (id.IsOk() && m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
    {
        m_treectrl->EditLabel(id);
    }
}


//----------------------------------------------------------------
void CommandToolBarWindow::OnContextMenuItemRemove(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'remove' is clicked.
 * Deletes a toolbar or toolbar item.
 * \param event Event.
 **/
{
    wxTreeItemId id = m_treectrl->GetSelection();
    if (id.IsOk() && m_treectrl->GetRootItem() != id)
    {
        if (m_treectrl->GetRootItem() == m_treectrl->GetItemParent(id))
        {
            // Remove ToolBar
            int result = wxMessageBox(_("Are you sure to remove the whole toolbar?"),
                _("Remove toolbar ...?"), wxYES_NO|wxICON_QUESTION, this);
            if (result == wxYES)
            {
                m_treectrl->DeleteChildren(id);
                m_treectrl->Delete(id);
            }
        }
        else
        {
            // Remove ToolBar item
            m_treectrl->Delete(id);
        }
    }
}



} // namespace penv



