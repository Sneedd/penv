/*
 * commandmenutreedata.h - Declaration of the CommandMenuTreeData class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04/25/2008
 *
 */


#ifndef _PENV_UI_COMMANDMENUTREEDATA_H_
#define _PENV_UI_COMMANDMENUTREEDATA_H_

#include "../menuitem.h"
#include "../wxincludes.h"


namespace penv {

/**
 * \brief The CommandMenuTreeData contains information about a
 * tree item in the CommandMenuWindow's tree control. The class
 * simply stores a poiter to a menu item.
 **/
class CommandMenuTreeData : public wxTreeItemData
{
    public:
        CommandMenuTreeData();
        CommandMenuTreeData(MenuItemType type, MenuItem* item);
        CommandMenuTreeData(MenuItemType type, const wxString& id);
        virtual ~CommandMenuTreeData();

        void SetMenuItem(MenuItem* item);
        MenuItem* GetMenuItem();

        void SetCommandId(const wxString& id);
        const wxString& GetCommandId() const;

        void SetType(MenuItemType type);
        MenuItemType GetType() const;

    private:
        MenuItemType m_type;
        MenuItem* m_item;
        wxString m_id;
};

} // namespace penv


#endif // _PENV_UI_COMMANDMENUTREEDATA_H_


