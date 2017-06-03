/*
 * debugwindow.h - Declaration of the DebugWindow class
 *
 * Author: Steffen Ott
 * Date: 09/20/2007
 *
 */


#ifndef _PENV_UI_DEBUGWINDOW_H_
#define _PENV_UI_DEBUGWINDOW_H_

#include "../wxincludes.h"
#include "debugpagecommands.h"
#include "debugpagewindows.h"

namespace penv {

/**
 * \brief The DebugWindow class is a panel with a notebook to
 * display the DebugPageCommands and DebugPageWindows panels.
 * This panel must not implemented, it already is. Just call
 * the ApplicationFrame::ToogleDebugWindow() Method to toogle
 * the visibility of the window.
 * \todo Add page for packages.
 **/
class DebugWindow : public wxPanel
{
    private:
        DebugWindow();
    public:
        DebugWindow(wxWindow* parent, int id = wxID_ANY);
        ~DebugWindow();

        void RefreshWindows();
        void RefreshCommands();

    private:
		wxAuiNotebook* m_notebook;
        DebugPageWindows* m_debugpagewindows;
        DebugPageCommands* m_debugpagecommands;
};

} // namespace penv


#endif // _PENV_UI_DEBUGWINDOW_H_
