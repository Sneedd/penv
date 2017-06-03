/*
 * menuitem.h - Declaration of the MenuItem class.
 *
 * Author: Steffen Ott
 * Date: 10/16/2007
 *
 */


#ifndef _PENV_MENUITEM_H_
#define _PENV_MENUITEM_H_

#include "wxincludes.h"
#include "command.h"
#include "menuitemtype.h"

namespace penv {

/**
 * \brief The MenuItem class represents one menu item in the framework.
 * A menu item can be a standard menu item connected to a command, a
 * separator, submenu, checkable menuitem or radio menuitem. <br>The
 * following methods are for submenu items only, if they called on
 * another menu item type, you will get an error: <code>
 * GetItem(), Add(), Insert(), Remove() and Count()</code><br>
 * Here the definition of a 'toolitem' xml element:<br>
 * <code>
 *  &lt;menuitem type="command" enabled="true" visible="true"&gt;COMMAND_ID&lt;/menuitem&gt;<br>
 *  &lt;menuitem type="separator" enabled="true" visible="true" /&gt;<br>
 *  &lt;menuitem type="checkbox" enabled="true" visible="true"&gt;COMMAND_ID&lt;/menuitem&gt;<br>
 *  &lt;menuitem type="radio" enabled="true" visible="true"&gt;COMMAND_ID&lt;/menuitem&gt;<br>
 *  &lt;menuitem type="submenu" enabled="true" visible="true"&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- menu items elements should be here --&gt;<br>
 *  &lt;/menuitem&gt;<br>
 * </code>
 **/
class MenuItem
{
    private:
        friend class MenuBar;
        MenuItem();
    public:
        MenuItem(MenuItemType type, Command* command=NULL);
        ~MenuItem();

        const wxString& GetName() const;
        void SetName(const wxString& name);
        MenuItemType GetType() const;
        const wxString& GetHelp() const;
        const wxBitmap& GetIcon() const;
        const wxString& GetId() const;

        // Submenu spezifisch
        MenuItem* GetItem(size_t index);
        bool Add(MenuItem* menuitem);
        bool Insert(size_t index, MenuItem* menuitem);
        bool Remove(MenuItem* menuitem);
        bool Remove(size_t index);
        size_t Count() const;

        wxMenuItem* GetMenuItem();
        bool TryConnect(bool recursive = false);

        bool ReadXml(wxXmlNode* node);
        wxXmlNode* WriteXml();

    private:
        void AddItems(wxMenu* menu, bool connect);
        void FindMenuItems(Array<MenuItem>* array, const wxString& commandid);

    private:
        Array<MenuItem>* m_submenu;
        wxString m_name;
        wxString m_id;
        MenuItemType m_type;
        Command* m_command;
        bool m_enabled;
        bool m_visible;
        bool m_connected;
        wxString m_emptystring;
        wxMenuItem* m_menuitem;
};

} // namespace penv


#endif // _PENV_MENUITEM_H_
