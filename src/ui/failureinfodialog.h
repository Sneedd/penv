/*
 * failureinfodialog.h - Declaration of the FailureInfoDialog class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */


#ifndef _PENV_UI_FAILUREINFODIALOG_H_
#define _PENV_UI_FAILUREINFODIALOG_H_

#include "../wxincludes.h"

namespace penv {

/**
 * \brief The FailureInfoDialog class is a dialog who shows informations
 * about failures in the system. This dialog is for example shown, when
 * the application cannot start.
 **/
class FailureInfoDialog : public wxDialog
{
    private:
        FailureInfoDialog();
    public:
        FailureInfoDialog(wxWindow* parent, int id = wxID_ANY,
            const wxString& message = _T("Please look into the log for more info and/or report this error."),
            const wxString& shortmessage = _T("Unexpected error occured"),
            const wxString& caption = _T("Error"));
        virtual ~FailureInfoDialog();

    protected:
        void InitializeUI();
        void DoLayout();
        void ConnectEvents();

		virtual void OnClose(wxCloseEvent& event);
		virtual void OnSize(wxSizeEvent& event);
		virtual void OnButtonCloseClick(wxCommandEvent& event);

    protected:
        wxString m_messageshort;
        wxString m_message;
		wxStaticText* m_labeltitel;
		wxStaticText* m_labelmessage;
		wxPanel* m_panel;
		wxButton* m_buttonclose;
};


} // namespace penv

#endif // _PENV_UI_FAILUREINFODIALOG_H_
