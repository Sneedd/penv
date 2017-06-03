/*
 * debugpagewindow.cpp - Implemenatation of the DebugPageWindows class
 *
 * Author: Steffen Ott
 * Date: 09/20/2007
 *
 */


#include "debugpagewindows.h"

#include "../windowlist.h"
#include "../environment.h"

namespace penv {

//----------------------------------------------------------------
DebugPageWindows::DebugPageWindows()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
DebugPageWindows::DebugPageWindows(wxWindow* parent, int id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
/**
 * \brief Constructor. Initialize the DebugPageWindows Object.
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
DebugPageWindows::~DebugPageWindows()
/**
 * \brief Destructor.
 **/
{
    if (m_contextmenu != NULL) delete m_contextmenu;
}



//----------------------------------------------------------------
void DebugPageWindows::InitializeUI()
/**
 * \brief Initialize all UI elements and set their properties.
 **/
{
	m_listctrl = new wxListCtrl( this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxLC_REPORT );

    // ContextMenu Stuff
    for (int i=0; i<penvDEBUGPAGEWINDOWS_MAXCONTEXTMENUIDS; ++i) {
        m_contextmenuids[i] = wxNewId();
    }
    m_contextmenu = new wxMenu();
    m_contextmenu->Append(m_contextmenuids[0], _T("Create"), _T(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[1], _T("Load"), _T(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[2], _T("Save"), _T(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[3], _T("Close"), _T(""), wxITEM_NORMAL);
    m_contextmenu->Append(m_contextmenuids[5], _T("Change visibility"), _T(""), wxITEM_NORMAL);
    m_contextmenu->AppendSeparator();
    m_contextmenu->Append(m_contextmenuids[4], _T("Refresh"), _T(""), wxITEM_NORMAL);


}


//----------------------------------------------------------------
void DebugPageWindows::DoLayout()
/**
 * \brief Updates the layout for the components.
 **/
{
	wxFlexGridSizer* m_sizer;
	m_sizer = new wxFlexGridSizer( 1, 1, 0, 0 );
	m_sizer->AddGrowableCol( 0 );
	m_sizer->AddGrowableRow( 0 );
	m_sizer->SetFlexibleDirection( wxBOTH );
	m_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	m_sizer->Add( m_listctrl, 0, wxALL|wxEXPAND, 5 );
	this->SetSizer( m_sizer );
	this->Layout();

}


//----------------------------------------------------------------
void DebugPageWindows::ConnectEvents()
/**
 * \brief Connect the callback methods with the components.
 **/
{
	m_listctrl->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
        wxListEventHandler(DebugPageWindows::OnListItemRightClick), NULL, this );
    m_contextmenu->Connect(m_contextmenuids[0], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageWindows::OnPopItemCreate), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[1], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageWindows::OnPopItemLoad), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[2], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageWindows::OnPopItemSave), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[3], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageWindows::OnPopItemClose), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[4], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageWindows::OnPopItemRefresh), NULL, this);
    m_contextmenu->Connect(m_contextmenuids[5], wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(DebugPageWindows::OnPopItemVisible), NULL, this);

}


//----------------------------------------------------------------
void DebugPageWindows::UpdateComponents()
/**
 * \brief Updates the content of the components.
 **/
{
    // Inhalt der Liste Löschen
    m_listctrl->ClearAll();

    // ListCtrl Columns anlegen
    m_listctrl->InsertColumn(0, _T("ClassID"), wxLIST_FORMAT_LEFT, 100);
    m_listctrl->InsertColumn(1, _T("ID"), wxLIST_FORMAT_LEFT, 100);
    m_listctrl->InsertColumn(2, _T("Type"), wxLIST_FORMAT_LEFT, 80);
    m_listctrl->InsertColumn(3, _T("Caption"), wxLIST_FORMAT_LEFT, 100);
    m_listctrl->InsertColumn(4, _T("WindowPtr"), wxLIST_FORMAT_LEFT, 90);
    m_listctrl->InsertColumn(5, _T("Modified"), wxLIST_FORMAT_LEFT, 75);
    m_listctrl->InsertColumn(6, _T("ProjectItem"), wxLIST_FORMAT_LEFT, 100);

    // Neuen Inhalt einfügen, zuerst die registrierten, ...
    Environment* env = Environment::Get();
    WindowList* list = env->GetWindowList();
    if (list == NULL) return;
    wxArrayString* array = list->GetRegisteredWindows();
    for (size_t i=0; i<array->Count(); ++i)
    {
        Window* window = list->GetRegistered((*array)[i]);
        this->AddItem(window, true);
    }
    delete array;

    // ... dann die offenen Fenster
    array = list->GetOpenWindows();
    for (size_t i=0; i<array->Count(); ++i)
    {
        Window* window = list->GetWindow((*array)[i]);
        this->AddItem(window, false);
    }
    delete array;

}


//----------------------------------------------------------------
void DebugPageWindows::AddItem(Window* window, bool registered)
/**
 * \brief Adds a window to the list control.
 * \param window Window.
 * \param registered True if window is a registered window.
 **/
{
    if (window == NULL) return;
    long index = m_listctrl->InsertItem(m_listctrl->GetItemCount(), 0);
    m_listctrl->SetItem(index, 0, window->GetClassId(), -1);
    m_listctrl->SetItem(index, 1, window->GetId(), -1);
    if (window->GetType() == penvWT_None)
        m_listctrl->SetItem(index, 2, _T("none"), -1);
    else if (window->GetType() == penvWT_Pane)
        m_listctrl->SetItem(index, 2, _T("pane"), -1);
    else if (window->GetType() == penvWT_Document)
        m_listctrl->SetItem(index, 2, _T("document"), -1);
    m_listctrl->SetItem(index, 3, window->GetCaption(), -1);
    m_listctrl->SetItem(index, 4, wxString::Format(_T("0x%p"), window->GetWindow()), -1);
    if (window->IsModified()) m_listctrl->SetItem(index, 5, _T("true"), -1);
    else m_listctrl->SetItem(index, 5, _T("false"), -1);
    if (window->GetProjectItem() == NULL)
        m_listctrl->SetItem(index, 6, _T("NULL"), -1);
    else
        m_listctrl->SetItem(index, 6, window->GetProjectItem()->GetName(), -1);
    if (registered) m_listctrl->SetItemTextColour(index, wxColor(0x36,0x87,0x00));
}


//----------------------------------------------------------------
void DebugPageWindows::OnListItemRightClick(wxListEvent& event)
/**
 * \brief Occurs when a list item is right clicked.
 * Opens the context menu.
 * \param event Event.
 **/
{
    // Ermittle Item Window Type ID und Window ID
    wxListItem item;
    item.SetId(event.GetIndex());
    item.SetColumn(0);
    item.SetMask(wxLIST_MASK_TEXT);
    m_listctrl->GetItem(item);
    m_windowtypeid = item.GetText();
    item.SetColumn(1);
    m_listctrl->GetItem(item);
    m_windowid = item.GetText();
    // Nur Elemente die auch verwendet werden können einschalten!
    if (m_windowid.IsEmpty())
    {
        // Registered window!
        m_contextmenu->Enable(m_contextmenuids[0], true);
        m_contextmenu->Enable(m_contextmenuids[1], true);
        m_contextmenu->Enable(m_contextmenuids[2], false);
        m_contextmenu->Enable(m_contextmenuids[3], false);
        m_contextmenu->Enable(m_contextmenuids[5], false);
    }
    else
    {
        // Open window!
        m_contextmenu->Enable(m_contextmenuids[0], false);
        m_contextmenu->Enable(m_contextmenuids[1], false);
        m_contextmenu->Enable(m_contextmenuids[2], true);
        m_contextmenu->Enable(m_contextmenuids[3], true);
        WindowList* winlist = Environment::Get()->GetWindowList();
        Window* win = winlist->GetWindow(m_windowid);
        if (win != NULL)
        {
            if (win->GetType() == penvWT_Pane) {
                m_contextmenu->Enable(m_contextmenuids[5], true);
            } else {
                m_contextmenu->Enable(m_contextmenuids[5], false);
            }
        }
    }
    // PopUp Menu anzeigen!!
    wxPoint epnt = event.GetPoint();
    this->PopupMenu(m_contextmenu, epnt);
}



//----------------------------------------------------------------
void DebugPageWindows::OnPopItemCreate(wxCommandEvent& event)
/**
 * \brief Occurs when the 'create' menu item is clicked.
 * Creates a window of the type in the list control.
 * \param event Event.
 **/
{
    WindowList* winlist = Environment::Get()->GetWindowList();
    Window* win = winlist->CreateWindow(m_windowtypeid, NULL);
    winlist->Add(win, true);
}


//----------------------------------------------------------------
void DebugPageWindows::OnPopItemLoad(wxCommandEvent& event)
/**
 * \brief Occurs when the 'load' menu item is clicked.
 * Opens a file dialog and creates a new window and calls
 * the load method for the selected file.
 * \param event Event.
 **/
{
    Environment* env = Environment::Get();
    wxFileDialog dialog(env->GetFrame(), _T("Choose a file to load ..."),
        _T(""), _T(""), _T("*.*"), wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize);
    if (dialog.ShowModal() == wxID_OK)
    {
        // Erstelle neues Fenster und lade die Informationen aus
        //  der Datei
        WindowList* winlist = Environment::Get()->GetWindowList();
        Window* win = winlist->CreateWindow(m_windowtypeid, NULL);
        winlist->Add(win, true);
        win->Load(dialog.GetFilename());
        win->Update();
    }
}


//----------------------------------------------------------------
void DebugPageWindows::OnPopItemSave(wxCommandEvent& event)
/**
 * \brief Occurs when the 'save' menu item is clicked.
 * Opens a file dialog and calls save on the opened window
 * with the selected file.
 * \param event Event.
 **/
{
    Environment* env = Environment::Get();
    wxFileDialog dialog(env->GetFrame(), _T("Choose a file to save ..."),
        _T(""), _T(""), _T("*.*"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT, wxDefaultPosition, wxDefaultSize);
    if (dialog.ShowModal() == wxID_OK)
    {
        // Erstelle neues Fenster und lade die Informationen aus
        //  der Datei
        WindowList* winlist = Environment::Get()->GetWindowList();
        Window* win = winlist->GetWindow(m_windowid);
        win->Save(dialog.GetFilename());
    }
}


//----------------------------------------------------------------
void DebugPageWindows::OnPopItemClose(wxCommandEvent& event)
/**
 * \brief Occurs when the 'close' menu item is clicked.
 * Closes the opened window.
 * \param event Event.
 **/
{
    WindowList* winlist = Environment::Get()->GetWindowList();
    Window* win = winlist->GetWindow(m_windowid);
    if (win != NULL)
    {
        winlist->Close(m_windowid);
    }
}


//----------------------------------------------------------------
void DebugPageWindows::OnPopItemVisible(wxCommandEvent& event)
/**
 * \brief Occurs when the 'change visibility' menu item is clicked.
 * Refreshes the list control.
 * \param event Event.
 **/
{
    WindowList* winlist = Environment::Get()->GetWindowList();
    Window* win = winlist->GetWindow(m_windowid);
    if (win != NULL)
    {
        if (win->GetType() != penvWT_Pane) return;
        WindowPane* winpane = (WindowPane*)win;
        winpane->Visible(!winpane->IsVisible());
    }
}



//----------------------------------------------------------------
void DebugPageWindows::OnPopItemRefresh(wxCommandEvent& event)
/**
 * \brief Occurs when the 'refresh' menu item is clicked.
 * Refreshes the list control.
 * \param event Event.
 **/
{
    UpdateComponents();
}




} // namespace penv



