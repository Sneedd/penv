/*
 * statusbar.h - Declaration of the StatusBar class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */


#ifndef _PENV_STATUSBAR_H_
#define _PENV_STATUSBAR_H_

#include "wxincludes.h"

namespace penv {

/**
 * \brief The StatusBar class represents the statusbar in
 * the application.
 **/
class StatusBar : public wxStatusBar
{
    private:
        StatusBar();
    public:
        StatusBar(wxWindow* parent, int id = wxID_ANY);
        ~StatusBar();
    private:
};


} // namespace penv

#endif // _PENV_STATUSBAR_H_
