/*
 * commandshortcutwindow.h - Declaration of the CommandShortcutWindow class
 *
 * Author: Steffen Ott
 * Date: 04/23/2008
 *
 */


#ifndef _PENV_UI_COMMANDSHORTCUTWINDOW_H_
#define _PENV_UI_COMMANDSHORTCUTWINDOW_H_

#include "../wxincludes.h"


namespace penv {

/**
 * \brief The CommandShortcutWindow class is a panel where the user
 * could change the shortcuts. This class is used in the CommandDialog
 * class, which is the easiest way to give your user the chance to
 * change the shortcuts.
 * \todo Not implemented yet.
 **/
class CommandShortcutWindow : public wxPanel
{
    private:
        CommandShortcutWindow();
    public:
		CommandShortcutWindow(wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition,
            wxSize size = wxDefaultSize, int style = wxTAB_TRAVERSAL);
        ~CommandShortcutWindow();

        void InitializeUI();
        void DoLayout();
        void ConnectEvents();
        void UpdateComponents();
        void SaveAndUpdate();


    private:
		wxStaticText* m_labelnotimplemented;
};

} //namespace penv

#endif // _PENV_UI_COMMANDSHORTCUTWINDOW_H_
