/*
 * messageboxadvanced.h - Declaration of the MessageBoxAdvanced class
 *
 * Author: Steffen Ott
 * Date: 05/01/2008
 *
 */

#ifndef _PENV_UI_MESSAGEBOXADVANCED_H_
#define _PENV_UI_MESSAGEBOXADVANCED_H_

#include "../wxincludes.h"

namespace penv {

#ifndef wxYESALL
    #define wxYESALL 99
#endif
#ifndef wxNOALL
    #define wxNOALL 99
#endif

/**
 * \brief The MessageBoxAdvanced class represents a advanced message box
 * with additional buttons, like 'Yes, to all.'.
 **/
class MessageBoxAdvanced
{
    public:
        MessageBoxAdvanced();
        ~MessageBoxAdvanced();

//        static int Show(const wxString& message, const wxString& caption = _T("Message"),
//            int style = wxOK, wxWindow* parent = NULL, int x = -1, int y = -1);

    protected:
    private:
};


} // namespace penv


#endif // _PENV_UI_MESSAGEBOXADVANCED_H_
