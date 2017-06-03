/*
 * messagewindowlogchain.h - Declaration of the MessageWindowLogChain class
 *
 * Author: Steffen Ott
 * Date: 03/21/2007
 *
 */


#ifndef _PENV_UI_MESSAGEWINDOWLOGCHAIN_H_
#define _PENV_UI_MESSAGEWINDOWLOGCHAIN_H_

#include "../wxincludes.h"
#include "messagewindow.h"


namespace penv {

/**
 * \brief The MessageWindowLogChain class does the logs for
 * the MessageWindow. It's inherited from wxLog and is implemented
 * over a wxLogChain.
 * \todo Save all messages in memory or load them from the logfile,
 * to show all previous messages in the window, too.
 **/
class MessageWindowLogChain : public wxLog
{
    private:
        MessageWindowLogChain();
    public:
        MessageWindowLogChain(MessageWindow* window);
        virtual ~MessageWindowLogChain();

        void SetMessageWindow(MessageWindow* window);
        void SetForwarding(bool forwarding);

        void DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp);
        void Flush();

    private:
        wxLogChain* m_logchain;
        bool m_forwarding;
        MessageWindow* m_window;
};

} // namespace penv

#endif // _PENV_UI_MESSAGEWINDOWLOGCHAIN_H_
