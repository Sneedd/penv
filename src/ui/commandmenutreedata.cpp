/*
 * commandmenutreedata.cpp - implementation of the CommandMenuTreeData class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04/25/2008
 *
 */



#include "commandmenutreedata.h"
#include "../expect.h"
#include "../penvhelper.h"
#include "../environment.h"

namespace penv {


//----------------------------------------------------------------
CommandMenuTreeData::CommandMenuTreeData()
/**
 * \brief Constructor, initializes the CommandMenuTreeData object.
 **/
{
    m_item = NULL;
    m_type = penvMI_Separator;
}


//----------------------------------------------------------------
CommandMenuTreeData::CommandMenuTreeData(MenuItemType type, MenuItem* item)
/**
 * \brief Constructor, initializes the CommandMenuTreeData object.
 * \param type The type of the menu item.
 * \param item MenuItem of the tree item.
 **/
{
    m_item = item;
    m_type = type;
    if (item != NULL) {
        m_id = item->GetId();
    }
}

//----------------------------------------------------------------
CommandMenuTreeData::CommandMenuTreeData(MenuItemType type, const wxString& id)
/**
 * \brief Constructor, initializes the CommandMenuTreeData object.
 * \param type The type of the menu item.
 * \param id Command id for the menuitem.
 **/
{
    m_id = id;
    m_item = NULL;
    m_type = type;
}


//----------------------------------------------------------------
CommandMenuTreeData::~CommandMenuTreeData()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
void CommandMenuTreeData::SetMenuItem(MenuItem* item)
/**
 * \brief Sets the menu item for this data item.
 * \param item MenuItem of the tree item.
 **/
{
    m_item = item;
}


//----------------------------------------------------------------
MenuItem* CommandMenuTreeData::GetMenuItem()
/**
 * \brief Returns the menu item for this tree item data.
 * \return Menu item of tree item.
 **/
{
    return (m_item);
}


//----------------------------------------------------------------
void CommandMenuTreeData::SetCommandId(const wxString& id)
/**
 * \brief Sets the command id for the tree item.
 * \param id Command id for tree item.
 **/
{
    m_id = id;
}


//----------------------------------------------------------------
const wxString& CommandMenuTreeData::GetCommandId() const
/**
 * \brief Returns the command id for the tree item.
 * \return Command id for tree item.
 **/
{
    return (m_id);
}


//----------------------------------------------------------------
void CommandMenuTreeData::SetType(MenuItemType type)
/**
 * \brief Sets the MenuItemType for the tree item.
 * \param type MenuItemType of the item.
 **/
{
    m_type = type;
}


//----------------------------------------------------------------
MenuItemType CommandMenuTreeData::GetType() const
/**
 * \brief Returns the MenuItemType for the tree item.
 * \return Command MenuItemType.
 **/
{
    return (m_type);
}



} // namespace penv

