/*
 * messagewindow.h - Declaration of the MessageWindow class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */

#ifndef _PENV_UI_MESSAGEWINDOW_H_
#define _PENV_UI_MESSAGEWINDOW_H_

#include "../wxincludes.h"

namespace penv {

/**
 * \brief The MessageWindow class is a ListCtrl which items are messages generated
 * by the framework. Messages can be generated with the wxLogMessage(), wxLogWarning()
 * or wxLogError() functions.
 * \todo Implement a context menu to skip showing warnings, errors or messages.
 * \todo Clear and copy to clipboard will be helpful for the context menu.
 **/
class MessageWindow : public wxPanel
{
    private:
        friend class MessageWindowLogChain;
        MessageWindow();
    public:
        MessageWindow(wxWindow* parent, int id, bool uselogchain = true);
        ~MessageWindow();

        void InitializeUI();
        void DoLayout();
        void ConnectEvents();
        void ParseLogFile(const wxString& filename);

    private:
        void OnListItemRightClick(wxListEvent& event);
        void DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp);
        void Flush();


    private:
        MessageWindowLogChain* m_logchain;
        wxListCtrl* m_listctrl;
        wxColor m_messagecolor;
        wxColor m_warningcolor;
        wxColor m_errorcolor;

};

} // namespace penv

#endif // _PENV_UI_MESSAGEWINDOW_H_
