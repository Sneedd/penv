/*
 * debugpagecommands.cpp - Implementation of the DebugPageCommands class
 *
 * Author: Steffen Ott
 * Date: 11/15/2007
 *
 */


#include "debugpagecommands.h"
#include "../environment.h"


namespace penv {



//----------------------------------------------------------------
DebugPageCommands::DebugPageCommands(wxWindow* parent, int id)
    : wxPanel(parent, id)
/**
 * \brief Constructor. Initialize the DebugPageCommand Object.
 * \param parent Parent window of this panel.
 * \param id Id for this panel, can be wxID_ANY.
 **/
{
    InitializeUI();
    DoLayout();
    ConnectEvents();
    UpdateComponents();
}



//----------------------------------------------------------------
DebugPageCommands::~DebugPageCommands()
/**
 * \brief Destructor.
 **/
{
    if (m_contextmenu != NULL) delete m_contextmenu;
}




//----------------------------------------------------------------
void DebugPageCommands::InitializeUI()
/**
 * \brief Initialize all UI elements and set their properties.
 **/
{
	m_listctrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT);
    // Context Menu stuff
    for (int i=0; i<penvDEBUGPAGECOMMANDS_MAXCONTEXTMENUIDS; ++i) {
        m_contextmenuids[i] = wxNewId();
    }
    m_contextmenu = new wxMenu();
    m_contextmenu->Append(m_contextmenuids[0], _("Execute"), _(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[1], _("Toogle enabled"), _(""), wxITEM_NORMAL);
    m_contextmenu->AppendSeparator();
    m_contextmenu->Append(m_contextmenuids[2], _("Refresh list"), _(""), wxITEM_NORMAL);


}


//----------------------------------------------------------------
void DebugPageCommands::DoLayout()
/**
 * \brief Updates the layout for the components.
 **/
{
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer(1, 1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizer->Add(m_listctrl, 0, wxALL|wxEXPAND, 5);
	this->SetSizer( sizer );
	this->Layout();
}


//----------------------------------------------------------------
void DebugPageCommands::ConnectEvents()
/**
 * \brief Connect the callback methods with the components.
 **/
{
	m_listctrl->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
        wxListEventHandler(DebugPageCommands::OnListItemRightClick), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[0], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageCommands::OnContextMenuItemExecute), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[1], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageCommands::OnContextMenuItemToogleEnabled), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[2], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageCommands::OnContextMenuItemRefresh), NULL, this);
}



//----------------------------------------------------------------
void DebugPageCommands::UpdateComponents()
/**
 * \brief Updates the content of the components.
 **/
{
    // Clear the whole list
    m_listctrl->ClearAll();
    // Add columns to the list
    m_listctrl->InsertColumn(0, _T("ID"), wxLIST_FORMAT_LEFT, 170);
    m_listctrl->InsertColumn(1, _T("Name"), wxLIST_FORMAT_LEFT, 100);
    m_listctrl->InsertColumn(2, _T("Event"), wxLIST_FORMAT_LEFT, 75);
    m_listctrl->InsertColumn(3, _T("Enabled"), wxLIST_FORMAT_LEFT, 65);
    m_listctrl->InsertColumn(4, _T("Visible"), wxLIST_FORMAT_LEFT, 60);
    m_listctrl->InsertColumn(5, _T("CountCallbacks"), wxLIST_FORMAT_LEFT, 85);
    m_listctrl->InsertColumn(6, _T("wxEvtHandler Ptr"), wxLIST_FORMAT_LEFT, 85);
    m_listctrl->InsertColumn(7, _T("Help"), wxLIST_FORMAT_LEFT, 200);

    // Add items to the list
    Environment* env = Environment::Get();
    CommandList* list = env->GetCommandList();
    if (list == NULL) return;
    wxArrayString* array = list->GetCommands();
    for (size_t i=0; i<array->Count(); ++i)
    {
        Command* cmd = list->GetCommand((*array)[i]);
        AddItem(cmd);
    }
    delete array;
}


//----------------------------------------------------------------
void DebugPageCommands::AddItem(Command* cmd)
/**
 * \brief Adds the given Event/Command to the ListCtrl.
 * \param cmd Event or Command which should be added.
 **/
{
    if (cmd == NULL) {
        wxLogError(_T("[penv::DebugPageCommands::AddItem] Found a command in CommandList which is null."));
        return;
    }
    long index = m_listctrl->InsertItem(m_listctrl->GetItemCount(), 0);
    m_listctrl->SetItem(index, 0, cmd->GetId(), -1);
    m_listctrl->SetItem(index, 1, cmd->GetName(), -1);
    if (cmd->IsEvent()) m_listctrl->SetItem(index, 2, _T("Event"), -1);
    else m_listctrl->SetItem(index, 2, _T("Command"), -1);
    if (cmd->IsEnabled()) m_listctrl->SetItem(index, 3, _T("true"), -1);
    else m_listctrl->SetItem(index, 3, _T("false"), -1);
    if (cmd->IsVisible()) m_listctrl->SetItem(index, 4, _T("true"), -1);
    else m_listctrl->SetItem(index, 4, _T("false"), -1);
    m_listctrl->SetItem(index, 5, wxString::Format(_T("%i"), cmd->GetCallbacksCount()), -1);
    m_listctrl->SetItem(index, 6, wxString::Format(_T("%p"), cmd), -1);
    m_listctrl->SetItem(index, 7, cmd->GetHelp(), -1);
}



//----------------------------------------------------------------
void DebugPageCommands::OnListItemRightClick(wxListEvent& event)
/**
 * \brief Event that occurs when a list item is right-clicked.
 * Opens a popup menu, for more actions on the list item.
 * \param event Just for event handling.
 **/
{
    wxListItem item;
    item.SetId(event.GetIndex());
    item.SetColumn(0);
    item.SetMask(wxLIST_MASK_TEXT);
    m_listctrl->GetItem(item);
    m_commandid = item.GetText();
    this->PopupMenu(m_contextmenu, event.GetPoint());
}



//----------------------------------------------------------------
void DebugPageCommands::OnContextMenuItemExecute(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'executes' is clicked.
 * Executes the rightclicked command or event.
 * \param event Event.
 **/
{
    Environment::Get()->GetCommandList()->Execute(m_commandid);
}


//----------------------------------------------------------------
void DebugPageCommands::OnContextMenuItemToogleEnabled(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'toogle enabled' is clicked.
 * Enable or disable the command or event.
 * \param event Event.
 **/
{
    Command* cmd = Environment::Get()->GetCommandList()->GetCommand(m_commandid);
    if (cmd == NULL) {
        wxLogWarning(_T("[penv::DebugPageCommands::OnContextMenuItemToogleEnabled] Command id '%s' does not exists."), m_commandid.c_str());
        return;
    }
    cmd->Enable(!cmd->IsEnabled());
}


//----------------------------------------------------------------
void DebugPageCommands::OnContextMenuItemRefresh(wxCommandEvent& event)
/**
 * \brief Occurs when the context menu item 'refresh' is clicked.
 * Refreshes the list control.
 * \param event Event.
 **/
{
    UpdateComponents();
}



} // namespace penv


