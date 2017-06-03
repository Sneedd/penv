/*
 * eventdocument.cpp - Implementation of the EventDocument class
 *
 * Author: Steffen Ott
 * Date: 04/08/2008
 *
 */


#include "eventdocument.h"


//----------------------------------------------------------------
EventDocument::EventDocument()
    : penv::WindowDocument(_T("TESTING_EVENT_INFORMATION_DOCUMENT"))
/**
 * \brief Constructor.
 **/
{
}



//----------------------------------------------------------------
EventDocument::~EventDocument()
/**
 * \brief Destructor.
 **/
{
}



//----------------------------------------------------------------
penv::Window* EventDocument::OnCreate(penv::ProjectItem* item)
/**
 * \brief Creates a EventDocument object.
 **/
{
    EventDocument* win = new EventDocument();
    win->Initialize(_T("Event Document"), item);
    win->InitializeUI();
    win->Layout();
    return (win);
}



//----------------------------------------------------------------
bool EventDocument::OnInitializeUI(wxWindow* parent)
/**
 * \brief Initializeses the UI components.
 **/
{
    EventDocumentPanel* panel = new EventDocumentPanel(parent);
    panel->InitializeUI(_T("Refresh output of dynamic events."));
    m_window = (wxWindow*)panel;
    return (true);
}



//----------------------------------------------------------------
bool EventDocument::OnLayout()
/**
 * \brief Updates the layout.
 **/
{
    EventDocumentPanel* panel = (EventDocumentPanel*)m_window;
    panel->DoLayout();
    return (true);
}







//================================================================
//----------------------------------------------------------------
void EventDocumentPanel::OnButtonClick(wxCommandEvent& event)
/**
 * \brief Executed when clicked an refresh.
 **/
{
    penv::Environment* env = penv::Environment::Get();
    ClearTextbox();
    // Print out Event Handlers
    wxEvtHandler* first = env->GetFrame();
    while (first != NULL)
    {
        wxString strg = _T("NO_CLASS_INFO");
        if(first->GetClassInfo() != NULL)
        {
            if(first->GetClassInfo()->GetClassName() != NULL)
            {
                strg = first->GetClassInfo()->GetClassName();
            }
        }

        WriteEvtHandler(first, strg);
        //first = first->GetNextHandler();
        first = first->GetPreviousHandler();
    }


}


//----------------------------------------------------------------
void EventDocumentPanel::WriteEvtHandler(wxEvtHandler* handler, const wxString& information)
/**
 * \brief Prints information about an event handler.
 * \param handler The event handler.
 * \param information Info about handler will be printed before the handler information.
 **/
{
    // Print out information
    WriteLine(wxString((wxChar)'=', 80));
    WriteLine(information);
    // Print out Handler Information
    WriteLine(wxString((wxChar)'-', 80));
    wxString evtstrg;
    evtstrg.Append(wxString::Format(_T("wxEvtHandler Pointer = %p"), handler));
    WriteLine(evtstrg);
    if (handler == NULL) return;
    // Print out Handler contents
    WriteLine(wxString((wxChar)'-', 80));
    wxList* list = handler->GetDynamicEventTable();
    if (list == NULL) {
        WriteLine(_T("No DynamicEventTable."));
        return;
    }
    wxList::compatibility_iterator node = list->GetFirst();
    while (node)
    {
        wxString strg;
        #if WXWIN_COMPATIBILITY_EVENT_TYPES
        wxEventTableEntry *entry = (wxEventTableEntry*)node->GetData();
        #else
        wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)node->GetData();
        #endif
        strg.Append(wxString::Format(_T("ID = %i\t\t"), entry->m_id));
        //strg.Append(wxString::Format(_T("LastID = %i\t"), entry->m_lastId));
        strg.Append(wxString::Format(_T("Function = %p\t"), entry->m_fn));
        //strg.Append(wxString::Format(_T("Userdata = %p\t"), entry->m_callbackUserData));
        strg.Append(wxString::Format(_T("EvtSink = %p\t"), entry->m_eventSink));
        strg.Append(wxString::Format(_T("EventType = %s\t"), ResolveEventTypes(entry->m_eventType).c_str()));
        WriteLine(strg);
        node = node->GetNext();
    }
}



//----------------------------------------------------------------
wxString EventDocumentPanel::ResolveEventTypes(int eventtype)
{
    if (eventtype == wxEVT_NULL) return (_T("wxEVT_NULL"));
    else if (eventtype == wxEVT_FIRST) return (_T("wxEVT_FIRST"));
    else if (eventtype == wxEVT_USER_FIRST) return (_T("wxEVT_USER_FIRST"));

    else if (eventtype == wxEVT_COMMAND_BUTTON_CLICKED) return (_T("wxEVT_COMMAND_BUTTON_CLICKED"));
    else if (eventtype == wxEVT_COMMAND_CHECKBOX_CLICKED) return (_T("wxEVT_COMMAND_CHECKBOX_CLICKED"));
    else if (eventtype == wxEVT_COMMAND_CHOICE_SELECTED) return (_T("wxEVT_COMMAND_CHOICE_SELECTED"));
    else if (eventtype == wxEVT_COMMAND_LISTBOX_SELECTED) return (_T("wxEVT_COMMAND_LISTBOX_SELECTED"));
    else if (eventtype == wxEVT_COMMAND_LISTBOX_DOUBLECLICKED) return (_T("wxEVT_COMMAND_LISTBOX_DOUBLECLICKED"));
    else if (eventtype == wxEVT_COMMAND_CHECKLISTBOX_TOGGLED) return (_T("wxEVT_COMMAND_CHECKLISTBOX_TOGGLED"));

    else if (eventtype == wxEVT_COMMAND_TEXT_UPDATED) return (_T("wxEVT_COMMAND_TEXT_UPDATED"));
    else if (eventtype == wxEVT_COMMAND_TEXT_ENTER) return (_T("wxEVT_COMMAND_TEXT_ENTER"));
    else if (eventtype == wxEVT_COMMAND_TEXT_URL) return (_T("wxEVT_COMMAND_TEXT_URL"));
    else if (eventtype == wxEVT_COMMAND_TEXT_MAXLEN) return (_T("wxEVT_COMMAND_TEXT_MAXLEN"));

    else if (eventtype == wxEVT_COMMAND_MENU_SELECTED) return (_T("wxEVT_COMMAND_MENU_SELECTED"));
    else if (eventtype == wxEVT_COMMAND_SLIDER_UPDATED) return (_T("wxEVT_COMMAND_SLIDER_UPDATED"));
    else if (eventtype == wxEVT_COMMAND_RADIOBOX_SELECTED) return (_T("wxEVT_COMMAND_RADIOBOX_SELECTED"));
    else if (eventtype == wxEVT_COMMAND_RADIOBUTTON_SELECTED) return (_T("wxEVT_COMMAND_RADIOBUTTON_SELECTED"));

	else if (eventtype == wxEVT_COMMAND_SCROLLBAR_UPDATED) return (_T("wxEVT_COMMAND_SCROLLBAR_UPDATED"));
	else if (eventtype == wxEVT_COMMAND_VLBOX_SELECTED) return (_T("wxEVT_COMMAND_VLBOX_SELECTED"));
	else if (eventtype == wxEVT_COMMAND_COMBOBOX_SELECTED) return (_T("wxEVT_COMMAND_COMBOBOX_SELECTED"));
	else if (eventtype == wxEVT_COMMAND_TOOL_RCLICKED) return (_T("wxEVT_COMMAND_TOOL_RCLICKED"));
	else if (eventtype == wxEVT_COMMAND_TOOL_ENTER) return (_T("wxEVT_COMMAND_TOOL_ENTER"));
	else if (eventtype == wxEVT_COMMAND_SPINCTRL_UPDATED) return (_T("wxEVT_COMMAND_SPINCTRL_UPDATED"));

	else if (eventtype == wxEVT_SOCKET) return (_T("wxEVT_SOCKET"));
	else if (eventtype == wxEVT_TIMER ) return (_T("wxEVT_TIMER "));

	else if (eventtype == wxEVT_LEFT_DOWN) return (_T("wxEVT_LEFT_DOWN"));
	else if (eventtype == wxEVT_LEFT_UP) return (_T("wxEVT_LEFT_UP"));
	else if (eventtype == wxEVT_MIDDLE_DOWN) return (_T("wxEVT_MIDDLE_DOWN"));
	else if (eventtype == wxEVT_MIDDLE_UP) return (_T("wxEVT_MIDDLE_UP"));
	else if (eventtype == wxEVT_RIGHT_DOWN) return (_T("wxEVT_RIGHT_DOWN"));
	else if (eventtype == wxEVT_RIGHT_UP) return (_T("wxEVT_RIGHT_UP"));
	else if (eventtype == wxEVT_MOTION) return (_T("wxEVT_MOTION"));
	else if (eventtype == wxEVT_ENTER_WINDOW) return (_T("wxEVT_ENTER_WINDOW"));
	else if (eventtype == wxEVT_LEAVE_WINDOW) return (_T("wxEVT_LEAVE_WINDOW"));
	else if (eventtype == wxEVT_LEFT_DCLICK) return (_T("wxEVT_LEFT_DCLICK"));
	else if (eventtype == wxEVT_MIDDLE_DCLICK) return (_T("wxEVT_MIDDLE_DCLICK"));
	else if (eventtype == wxEVT_RIGHT_DCLICK) return (_T("wxEVT_RIGHT_DCLICK"));
	else if (eventtype == wxEVT_SET_FOCUS) return (_T("wxEVT_SET_FOCUS"));
	else if (eventtype == wxEVT_KILL_FOCUS) return (_T("wxEVT_KILL_FOCUS"));
	else if (eventtype == wxEVT_CHILD_FOCUS) return (_T("wxEVT_CHILD_FOCUS"));
	else if (eventtype == wxEVT_MOUSEWHEEL) return (_T("wxEVT_MOUSEWHEEL"));

	else if (eventtype == wxEVT_NC_LEFT_DOWN) return (_T("wxEVT_NC_LEFT_DOWN"));
	else if (eventtype == wxEVT_NC_LEFT_UP) return (_T("wxEVT_NC_LEFT_UP"));
	else if (eventtype == wxEVT_NC_MIDDLE_DOWN) return (_T("wxEVT_NC_MIDDLE_DOWN"));
	else if (eventtype == wxEVT_NC_MIDDLE_UP) return (_T("wxEVT_NC_MIDDLE_UP"));
	else if (eventtype == wxEVT_NC_RIGHT_DOWN) return (_T("wxEVT_NC_RIGHT_DOWN"));
	else if (eventtype == wxEVT_NC_RIGHT_UP) return (_T("wxEVT_NC_RIGHT_UP"));
	else if (eventtype == wxEVT_NC_MOTION) return (_T("wxEVT_NC_MOTION"));
	else if (eventtype == wxEVT_NC_ENTER_WINDOW) return (_T("wxEVT_NC_ENTER_WINDOW"));
	else if (eventtype == wxEVT_NC_LEAVE_WINDOW) return (_T("wxEVT_NC_LEAVE_WINDOW"));
	else if (eventtype == wxEVT_NC_LEFT_DCLICK) return (_T("wxEVT_NC_LEFT_DCLICK"));
	else if (eventtype == wxEVT_NC_MIDDLE_DCLICK) return (_T("wxEVT_NC_MIDDLE_DCLICK"));
	else if (eventtype == wxEVT_NC_RIGHT_DCLICK) return (_T("wxEVT_NC_RIGHT_DCLICK"));

	else if (eventtype == wxEVT_CHAR) return (_T("wxEVT_CHAR"));
	else if (eventtype == wxEVT_CHAR_HOOK) return (_T("wxEVT_CHAR_HOOK"));
	else if (eventtype == wxEVT_NAVIGATION_KEY) return (_T("wxEVT_NAVIGATION_KEY"));
	else if (eventtype == wxEVT_KEY_DOWN) return (_T("wxEVT_KEY_DOWN"));
	else if (eventtype == wxEVT_KEY_UP) return (_T("wxEVT_KEY_UP"));

	else if (eventtype == wxEVT_HOTKEY) return (_T("wxEVT_HOTKEY"));

	else if (eventtype == wxEVT_SET_CURSOR) return (_T("wxEVT_SET_CURSOR"));

	else if (eventtype == wxEVT_SCROLL_TOP) return (_T("wxEVT_SCROLL_TOP"));
	else if (eventtype == wxEVT_SCROLL_BOTTOM) return (_T("wxEVT_SCROLL_BOTTOM"));
	else if (eventtype == wxEVT_SCROLL_LINEUP) return (_T("wxEVT_SCROLL_LINEUP"));
	else if (eventtype == wxEVT_SCROLL_LINEDOWN) return (_T("wxEVT_SCROLL_LINEDOWN"));
	else if (eventtype == wxEVT_SCROLL_PAGEUP) return (_T("wxEVT_SCROLL_PAGEUP"));
	else if (eventtype == wxEVT_SCROLL_PAGEDOWN) return (_T("wxEVT_SCROLL_PAGEDOWN"));
	else if (eventtype == wxEVT_SCROLL_THUMBTRACK) return (_T("wxEVT_SCROLL_THUMBTRACK"));
	else if (eventtype == wxEVT_SCROLL_THUMBRELEASE) return (_T("wxEVT_SCROLL_THUMBRELEASE"));
	else if (eventtype == wxEVT_SCROLL_CHANGED) return (_T("wxEVT_SCROLL_CHANGED"));

	else if (eventtype == wxEVT_SCROLLWIN_TOP) return (_T("wxEVT_SCROLLWIN_TOP"));
	else if (eventtype == wxEVT_SCROLLWIN_BOTTOM) return (_T("wxEVT_SCROLLWIN_BOTTOM"));
	else if (eventtype == wxEVT_SCROLLWIN_LINEUP) return (_T("wxEVT_SCROLLWIN_LINEUP"));
	else if (eventtype == wxEVT_SCROLLWIN_LINEDOWN) return (_T("wxEVT_SCROLLWIN_LINEDOWN"));
	else if (eventtype == wxEVT_SCROLLWIN_PAGEUP) return (_T("wxEVT_SCROLLWIN_PAGEUP"));
	else if (eventtype == wxEVT_SCROLLWIN_PAGEDOWN) return (_T("wxEVT_SCROLLWIN_PAGEDOWN"));
	else if (eventtype == wxEVT_SCROLLWIN_THUMBTRACK) return (_T("wxEVT_SCROLLWIN_THUMBTRACK"));
	else if (eventtype == wxEVT_SCROLLWIN_THUMBRELEASE) return (_T("wxEVT_SCROLLWIN_THUMBRELEASE"));

	else if (eventtype == wxEVT_SIZE) return (_T("wxEVT_SIZE"));
	else if (eventtype == wxEVT_MOVE) return (_T("wxEVT_MOVE"));
	else if (eventtype == wxEVT_CLOSE_WINDOW) return (_T("wxEVT_CLOSE_WINDOW"));
	else if (eventtype == wxEVT_END_SESSION) return (_T("wxEVT_END_SESSION"));
	else if (eventtype == wxEVT_QUERY_END_SESSION) return (_T("wxEVT_QUERY_END_SESSION"));
	else if (eventtype == wxEVT_ACTIVATE_APP) return (_T("wxEVT_ACTIVATE_APP"));

	else if (eventtype == wxEVT_ACTIVATE) return (_T("wxEVT_ACTIVATE"));
	else if (eventtype == wxEVT_CREATE) return (_T("wxEVT_CREATE"));
	else if (eventtype == wxEVT_DESTROY) return (_T("wxEVT_DESTROY"));
	else if (eventtype == wxEVT_SHOW) return (_T("wxEVT_SHOW"));
	else if (eventtype == wxEVT_ICONIZE) return (_T("wxEVT_ICONIZE"));
	else if (eventtype == wxEVT_MAXIMIZE) return (_T("wxEVT_MAXIMIZE"));
	else if (eventtype == wxEVT_MOUSE_CAPTURE_CHANGED) return (_T("wxEVT_MOUSE_CAPTURE_CHANGED"));
	else if (eventtype == wxEVT_MOUSE_CAPTURE_LOST) return (_T("wxEVT_MOUSE_CAPTURE_LOST"));
	else if (eventtype == wxEVT_PAINT) return (_T("wxEVT_PAINT"));
	else if (eventtype == wxEVT_ERASE_BACKGROUND) return (_T("wxEVT_ERASE_BACKGROUND"));
	else if (eventtype == wxEVT_NC_PAINT) return (_T("wxEVT_NC_PAINT"));
	else if (eventtype == wxEVT_PAINT_ICON) return (_T("wxEVT_PAINT_ICON"));
	else if (eventtype == wxEVT_MENU_OPEN) return (_T("wxEVT_MENU_OPEN"));
	else if (eventtype == wxEVT_MENU_CLOSE) return (_T("wxEVT_MENU_CLOSE"));
	else if (eventtype == wxEVT_MENU_HIGHLIGHT) return (_T("wxEVT_MENU_HIGHLIGHT"));
	else if (eventtype == wxEVT_CONTEXT_MENU) return (_T("wxEVT_CONTEXT_MENU"));
	else if (eventtype == wxEVT_SYS_COLOUR_CHANGED) return (_T("wxEVT_SYS_COLOUR_CHANGED"));
	else if (eventtype == wxEVT_DISPLAY_CHANGED) return (_T("wxEVT_DISPLAY_CHANGED"));
	else if (eventtype == wxEVT_SETTING_CHANGED) return (_T("wxEVT_SETTING_CHANGED"));
	else if (eventtype == wxEVT_QUERY_NEW_PALETTE) return (_T("wxEVT_QUERY_NEW_PALETTE"));
	else if (eventtype == wxEVT_PALETTE_CHANGED) return (_T("wxEVT_PALETTE_CHANGED"));
	else if (eventtype == wxEVT_JOY_BUTTON_DOWN) return (_T("wxEVT_JOY_BUTTON_DOWN"));
	else if (eventtype == wxEVT_JOY_BUTTON_UP) return (_T("wxEVT_JOY_BUTTON_UP"));
	else if (eventtype == wxEVT_JOY_MOVE) return (_T("wxEVT_JOY_MOVE"));
	else if (eventtype == wxEVT_JOY_ZMOVE) return (_T("wxEVT_JOY_ZMOVE"));
	else if (eventtype == wxEVT_DROP_FILES) return (_T("wxEVT_DROP_FILES"));
	else if (eventtype == wxEVT_DRAW_ITEM) return (_T("wxEVT_DRAW_ITEM"));
	else if (eventtype == wxEVT_MEASURE_ITEM) return (_T("wxEVT_MEASURE_ITEM"));
	else if (eventtype == wxEVT_COMPARE_ITEM) return (_T("wxEVT_COMPARE_ITEM"));
	else if (eventtype == wxEVT_INIT_DIALOG) return (_T("wxEVT_INIT_DIALOG"));
	else if (eventtype == wxEVT_IDLE) return (_T("wxEVT_IDLE"));
	else if (eventtype == wxEVT_UPDATE_UI) return (_T("wxEVT_UPDATE_UI"));
	else if (eventtype == wxEVT_SIZING) return (_T("wxEVT_SIZING"));
	else if (eventtype == wxEVT_MOVING) return (_T("wxEVT_MOVING"));
	else if (eventtype == wxEVT_HIBERNATE) return (_T("wxEVT_HIBERNATE"));
	else if (eventtype == wxEVT_COMMAND_TEXT_COPY) return (_T("wxEVT_COMMAND_TEXT_COPY"));
	else if (eventtype == wxEVT_COMMAND_TEXT_CUT) return (_T("wxEVT_COMMAND_TEXT_CUT"));
	else if (eventtype == wxEVT_COMMAND_TEXT_PASTE) return (_T("wxEVT_COMMAND_TEXT_PASTE"));
	else if (eventtype == wxEVT_COMMAND_LEFT_CLICK) return (_T("wxEVT_COMMAND_LEFT_CLICK"));
	else if (eventtype == wxEVT_COMMAND_LEFT_DCLICK) return (_T("wxEVT_COMMAND_LEFT_DCLICK"));
	else if (eventtype == wxEVT_COMMAND_RIGHT_CLICK) return (_T("wxEVT_COMMAND_RIGHT_CLICK"));
	else if (eventtype == wxEVT_COMMAND_RIGHT_DCLICK) return (_T("wxEVT_COMMAND_RIGHT_DCLICK"));
	else if (eventtype == wxEVT_COMMAND_SET_FOCUS) return (_T("wxEVT_COMMAND_SET_FOCUS"));
	else if (eventtype == wxEVT_COMMAND_KILL_FOCUS) return (_T("wxEVT_COMMAND_KILL_FOCUS"));
	else if (eventtype == wxEVT_COMMAND_ENTER) return (_T("wxEVT_COMMAND_ENTER"));
	else if (eventtype == wxEVT_HELP) return (_T("wxEVT_HELP"));
	else if (eventtype == wxEVT_DETAILED_HELP) return (_T("wxEVT_DETAILED_HELP"));

	else if (eventtype == wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED) return (_T("wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED"));
	else if (eventtype == wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING) return (_T("wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING"));
	else if (eventtype == wxEVT_COMMAND_AUINOTEBOOK_BUTTON) return (_T("wxEVT_COMMAND_AUINOTEBOOK_BUTTON"));
	else if (eventtype == wxEVT_COMMAND_AUINOTEBOOK_BEGIN_DRAG) return (_T("wxEVT_COMMAND_AUINOTEBOOK_BEGIN_DRAG"));
	else if (eventtype == wxEVT_COMMAND_AUINOTEBOOK_END_DRAG) return (_T("wxEVT_COMMAND_AUINOTEBOOK_END_DRAG"));
	else if (eventtype == wxEVT_COMMAND_AUINOTEBOOK_DRAG_MOTION) return (_T("wxEVT_COMMAND_AUINOTEBOOK_DRAG_MOTION"));

    return (_T("wxEVT_UNKNOWN (Event Type is not registered)"));
}





