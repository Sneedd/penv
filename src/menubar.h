/*
 * menubar.h - Declaration of the MenuBar class
 *
 * Author: Steffen Ott
 * Date: 09/20/2007
 *
 */

#ifndef _PENV_MENUBAR_H_
#define _PENV_MENUBAR_H_

#include "wxincludes.h"
#include "command.h"
#include "menuitem.h"


namespace penv {

/**
 * \brief The MenuBar class represents the menubar in the framework.
 * The menu items in the first level of the menubar must always be
 * submenu items. The framework is able to create, edit or remove
 * a menuitems within the framework. You can programmatically create
 * a menu item, use the xml file to create own menuitems or use an internally
 * dialog for editing the menubar and its items.<br>
 * Here the definition of a 'menubar' xml element:<br>
 * <code>
 *  &lt;menubar&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- menuitem elements should be here --&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- only submenu items are allowed at this level --&gt;<br>
 *  &lt;/menubar&gt;<br>
 * </code>
 **/
class MenuBar
{
    private:
        friend class CommandList;
        MenuBar();
        MenuBar(wxMenuBar* menubar);
        ~MenuBar();
    public:
        wxMenuBar* GetMenuBar();

        MenuItem* GetItem(size_t index);
        bool Add(MenuItem* item);
        bool Insert(size_t index, MenuItem* item);
        bool Remove(MenuItem* item);
        bool Remove(size_t index);
        void Clear();
        size_t Count();

        Array<MenuItem>* FindMenuItems(const wxString& commandid);
        bool Update();

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

    private:
        wxMenuBar* m_menubar;
        Array<MenuItem>* m_array;

};

} // namespace penv


#endif // _PENV_MENUBAR_H_
