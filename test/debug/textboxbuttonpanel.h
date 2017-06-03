/*
 * textboxbuttonpanel.h - Declaration of the TextBoxButtonPanel class
 *
 * Author: Steffen Ott
 * Date: 04/08/2008
 *
 */


#ifndef _TEST_TEXTBOXBUTTONPANEL_H_
#define _TEST_TEXTBOXBUTTONPANEL_H_

#include "../../src/penv.h"

/**
 * \brief TextBoxButtonPanel is a Panel with a MultiLine TextBox
 * and a Button below it.
 **/
class TextBoxButtonPanel : public wxPanel
{
    public:
        TextBoxButtonPanel(wxWindow* parent, int id = wxID_ANY);
        virtual ~TextBoxButtonPanel();

        void InitializeUI(const wxString& buttontext);
        void DoLayout();

        virtual void OnButtonClick(wxCommandEvent& event);

        void WriteLine(const wxString& string);
        void ClearTextbox();

    private:
        wxTextCtrl* m_textctrl;
        wxButton*   m_button;
};

#endif // _TEST_TEXTBOXBUTTONPANEL_H_
