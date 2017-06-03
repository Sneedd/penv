/*
 * commanddialog.h - Declaration of the CommandDialog class
 *
 * Author: Steffen Ott
 * Date: 03/18/2007
 *
 */

#ifndef _PENV_UI_COMMANDDIALOG_H_
#define _PENV_UI_COMMANDDIALOG_H_


#include "../penv.h"
#include "../wxincludes.h"


namespace penv {

/**
 * \brief The CommandDialog class is a Dialog who should help the user
 * to edit his custom toolbars, menubar and shortcuts. Also the application
 * developer could use this dialog to create the default mapping of this
 * interaction elements.<br>
 * You do not need to test for wxID_OK or wxID_CANCEL. The dialog generates them
 * but does all the stuff like saving the changed content, etc.<br>
 * Example:<br>
 * <code>
 * CommandDialog dialog(Environment::Get()->GetFrame());
 * dialog.ShowModal();
 * </code>
 * \todo The accept button does not work for now, should be enabled or disabled on any change.
 * \todo The shortcuts panel is not implemented yet.
 **/
class CommandDialog : public wxDialog
{
    private:
        CommandDialog();
    public:
        CommandDialog(wxWindow* parent, int id = wxID_ANY,
            const wxString& title = _("Customize commands ..."),
            wxPoint pos = wxDefaultPosition, wxSize size = wxSize(640,480),
            int style = wxCLOSE_BOX|wxCAPTION|wxRESIZE_BORDER);
        virtual ~CommandDialog();

        void SaveContent();

    protected:
        void InitializeUI();
        void DoLayout();

		void OnClose(wxCloseEvent& event);
		void OnButtonOkClick(wxCommandEvent& event);
		void OnButtonCancelClick(wxCommandEvent& event);
		void OnButtonAcceptClick(wxCommandEvent& event);


    private:
		wxAuiNotebook* m_notebook;
		wxPanel* m_panelmenubar;
		wxPanel* m_paneltoolbars;
		wxPanel* m_panelshortcuts;
		wxPanel* m_buttonspanel;
		wxPanel* m_paneldummy;
		wxButton* m_buttonok;
		wxButton* m_buttoncancel;
		wxButton* m_buttonaccept;
};

} // namepsace penv

#endif // _PENV_UI_COMMANDWINDOW_H_
