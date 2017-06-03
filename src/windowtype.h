/*
 * windowtype.h - Declaration of the WindowType enumerator
 *
 * Author: Steffen Ott
 * Date: 11/10/2007
 *
 */

#ifndef _PENV_WINDOWTYPE_H_
#define _PENV_WINDOWTYPE_H_

namespace penv {


/**
 * \brief The WindowType enumerator spezifies the type of
 * an Window.
 **/
enum WindowType
{
    /**
     * \brief Spezifies that the window is not definied.
     **/
    penvWT_None,
    /**
     * \brief Spezifies that the window is a WindowPane.
     **/
    penvWT_Pane,
    /**
     * \brief Spezifies that the window is a WindowDocument.
     **/
    penvWT_Document
};


} // namespace penv

#endif // _PENV_WINDOWTYPE_H_
