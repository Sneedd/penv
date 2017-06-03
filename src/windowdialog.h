/*
 * windowdialog.h - Declaration of the WindowDialog class
 *
 * Author: Steffen Ott
 * Date: 07/22/2008
 *
 */


#ifndef _PENV_WINDOWDIALOG_H_
#define _PENV_WINDOWDIALOG_H_

#include "window.h"

namespace penv {

/**
 * \brief The WindowDialog class supports the creation of dialog
 * within the window managament.
 * TODO
 **/
class WindowDialog : public Window
{
    public:
        WindowDialog();
        virtual ~WindowDialog();
    protected:
    private:
};

} // namespace penv

#endif // _PENV_WINDOWDIALOG_H_
