/*
 * messagewindow.cpp - Implementation of the MessageWindow class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */


#include "messagewindow.h"
#include "messagewindowlogchain.h"
#include "../penvhelper.h"

namespace penv {

//----------------------------------------------------------------
MessageWindow::MessageWindow()
/**
 * \brief Constructor (private).
 **/
{
}



//----------------------------------------------------------------
MessageWindow::MessageWindow(wxWindow* parent, int id, bool uselogchain)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
/**
 * \brief Constructor, initialize the MessageWindow object.
 * \param parent The parent window of this window.
 * \param id The id for this window.
 * \param uselogchain True if a log chain should be used.
 **/
{
    InitializeUI();
    DoLayout();
    ConnectEvents();

    // Columns definitions
    m_listctrl->InsertColumn(0, _T(""), wxLIST_FORMAT_CENTRE, 30);
    m_listctrl->InsertColumn(1, _("Message"), wxLIST_FORMAT_LEFT, 700);
    m_listctrl->InsertColumn(2, _("Time"), wxLIST_FORMAT_LEFT, 200);
	// Color definitions
    m_messagecolor = *wxBLACK;
    m_warningcolor = wxColour(195, 88, 0);
    m_errorcolor = *wxRED;

    // In dieses Logfenster umleiten
    if (uselogchain) {
        m_logchain = new MessageWindowLogChain(this);
    } else {
        m_logchain = NULL;
    }

    // Parse Log File
    if (!wxFileExists(_T(penvCOMMON_LOGFILEPATH))) {
        ParseLogFile(_T(penvCOMMON_LOGFILEPATH));
    }
}



//----------------------------------------------------------------
MessageWindow::~MessageWindow()
/**
 * \brief Destructor.
 **/
{
    // Logger deregistrieren (gelöscht wird der Logger vom wxFramework)
    if (m_logchain != NULL)
        m_logchain->SetForwarding(false);
}



//----------------------------------------------------------------
void MessageWindow::InitializeUI()
/**
 * \brief Initialize all UI elements and set their properties.
 **/
{
	m_listctrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxLC_REPORT);
}



//----------------------------------------------------------------
void MessageWindow::DoLayout()
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
void MessageWindow::ConnectEvents()
/**
 * \brief Connect the callback methods with the components.
 **/
{
	m_listctrl->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
        wxListEventHandler(MessageWindow::OnListItemRightClick), NULL, this);
}



//----------------------------------------------------------------
void MessageWindow::ParseLogFile(const wxString& filename)
/**
 * \brief Parses the log file and puts the logs into this
 * window.
 **/
{
    if (!wxFileExists(filename)) {
        return;
    }
    // Öffnen des gesamtem logfiles
    //wxString logfile = PenvHelper::ReadTextFile(filename);
    // Parsen und jede Zeile in das Fenster eintragen
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
void MessageWindow::OnListItemRightClick(wxListEvent& event)
/**
 * \brief Occurs if the user right clicks on a list item.
 * This method should open the context menu.
 **/
{
    NOT_IMPLEMENTED_YET();
}



//----------------------------------------------------------------
void MessageWindow::DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp)
/**
 * \brief Does the log for the window. If a message is recived, then
 * this method adds a new item with the message to the list ctrl.
 * \param level The log level.
 * \param msg The message.
 * \param timestamp Time when message is generated.
 **/
{
    long index = 0;
    wxString datetime = wxDateTime(timestamp).Format();
    wxColour color = m_messagecolor;
    wxString message;
    switch (level)
    {
        case wxLOG_FatalError:
            message = wxString::Format(_T("Fatal Error: %s"), msg);
            color = m_errorcolor;
            break;
        case wxLOG_Error:
            message = wxString::Format(_T("Error: %s"), msg);
            color = m_errorcolor;
            break;
        case wxLOG_Warning:
            message = wxString::Format(_T("Warning: %s"), msg);
            color = m_warningcolor;
            break;
        case wxLOG_Message:
            message = wxString::Format(_T("Message: %s"), msg);
            color = m_messagecolor;
            break;
        case wxLOG_Status:
            message = wxString::Format(_T("Status: %s"), msg);
            color = m_messagecolor;
            break;
        case wxLOG_Info:
            message = wxString::Format(_T("Info: %s"), msg);
            color = m_messagecolor;
            break;
        case wxLOG_Debug:
            message = wxString::Format(_T("Debug: %s"), msg);
            color = m_messagecolor;
            break;
        case wxLOG_Trace:
            message = wxString::Format(_T("Trace: %s"), msg);
            color = m_messagecolor;
            break;
        case wxLOG_Progress:
            message = wxString::Format(_T("Progress: %s"), msg);
            color = m_messagecolor;
            break;
        default:
            message = wxString::Format(_T("User: %s"), msg);
            color = m_warningcolor;
            break;
    }
    index = m_listctrl->InsertItem(index, 0);
    m_listctrl->SetItem(index, 1, msg, -1);
    m_listctrl->SetItem(index, 2, datetime, -1);
    m_listctrl->SetItemTextColour(index, color);
}

//----------------------------------------------------------------
void MessageWindow::Flush()
/**
 * \brief Flushes the log window. This method
 * does nothing because only log files need to be
 * flushed.
 **/
{
    // Nichts zu tun !!!
}


} // namespace penv
