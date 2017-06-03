/*
 * messagewindowlogchain.cpp - Implementation of the MessageWindowLogChain class
 *
 * Author: Steffen Ott
 * Date: 03/21/2007
 *
 */


#include "messagewindowlogchain.h"


namespace penv {


//----------------------------------------------------------------
MessageWindowLogChain::MessageWindowLogChain()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
MessageWindowLogChain::MessageWindowLogChain(MessageWindow* window)
/**
 * \brief Constructor, initializes the MessageWindowLogChain object.
 * \param window The message window for this logchain.
 **/
{
    m_logchain = new wxLogChain(this);
    m_window = window;
    if (m_window == NULL) m_forwarding = false;
    else m_forwarding = true;
}



//----------------------------------------------------------------
MessageWindowLogChain::~MessageWindowLogChain()
/**
 * \brief Destructor.
 **/
{
    m_forwarding = false;
    // NOTE: wxLogChain wird von wxWidgets gelöscht!!!
    //delete m_logchain;
}



//----------------------------------------------------------------
void MessageWindowLogChain::SetMessageWindow(MessageWindow* window)
/**
 * \brief Sets the message window for this class.
 * \param window The message window.
 **/
{
    m_window = window;
    if (m_window == NULL) m_forwarding = false;
    else m_forwarding = true;
}


//----------------------------------------------------------------
void MessageWindowLogChain::SetForwarding(bool forwarding)
/**
 * \brief Sets if the messages should be forwarded to
 * the message window.
 * \param forwarding True to forward messages to message window; false otherwise.
 **/
{
    m_forwarding = forwarding;
}


//----------------------------------------------------------------
void MessageWindowLogChain::DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp)
/**
 * \brief Does the actual logging, simply sends the message to
 * the message window, if fowarding is on.
 * \param level The log level (error, warning, message, ...)
 * \param msg The actual message.
 * \param timestamp The time when message is generated.
 **/
{
    if (m_forwarding)
    {
        m_window->DoLog(level, msg, timestamp);
    }
}


//----------------------------------------------------------------
void MessageWindowLogChain::Flush()
/**
 * \brief Flushes all messages.
 **/
{
    if (m_forwarding)
    {
        m_window->Flush();
    }
}


} // namespace penv




