/*
 * menuitemtype.h - Declaration of the MenuItemType enumerator
 *
 * Author: Steffen Ott
 * Date: 10/18/2007
 *
 */

#ifndef _PENV_MENUITEMTYPE_H_
#define _PENV_MENUITEMTYPE_H_


namespace penv {


/**
 * \brief The MenuItemType enumerator spezifies the type of
 * an menuitem.
 **/
enum MenuItemType
{
    /**
     * \brief Spezifies that the menuitem is a submenu.
     **/
    penvMI_Submenu,
    /**
     * \brief Spezifies that the menuitem is a simple command (menuitem).
     **/
    penvMI_Command,
    /**
     * \brief Spezifies that the menuitem is a checkbox menuitem.
     **/
    penvMI_Checkbox,
    /**
     * \brief Spezifies that the menuitem is a radio menuitem.
     * \todo This feature is not implemented yet.
     **/
    penvMI_Radio,
    /**
     * \brief Spezifies that the menuitem is a sparator.
     * \todo This feature is not implemented yet.
     **/
    penvMI_Separator
};



} // namespace penv


#endif //  _PENV_MENUITEMTYPE_H_
