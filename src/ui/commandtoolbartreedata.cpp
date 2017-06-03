/*
 * commandtoolbartreedata.cpp - Implementation of the CommandToolBarTreeData class
 *
 * Author: Steffen Ott
 * Date: 04/28/2008
 *
 */

#include "commandtoolbartreedata.h"

namespace penv {



//----------------------------------------------------------------
CommandToolBarTreeData::CommandToolBarTreeData()
/**
 * \brief Constructor, initializes the CommandToolBarTreeData object.
 **/
{
}


//----------------------------------------------------------------
CommandToolBarTreeData::CommandToolBarTreeData(const wxString& id)
/**
 * \brief Constructor, initializes the CommandToolBarTreeData object.
 * \param id The command id or toolbar id.
 **/
{
    m_id = id;
}


//----------------------------------------------------------------
CommandToolBarTreeData::~CommandToolBarTreeData()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
void CommandToolBarTreeData::SetCommandId(const wxString& id)
/**
 * \brief Sets the command id for this data item.
 * \param id The command id.
 **/
{
    m_id = id;
}


//----------------------------------------------------------------
void CommandToolBarTreeData::SetToolBarId(const wxString& id)
/**
 * \brief Sets the toolbar id for this data item.
 * \param id The toolbar id.
 **/
{
    m_id = id;
}


//----------------------------------------------------------------
const wxString& CommandToolBarTreeData::GetCommandId() const
/**
 * \brief Returns the command id for this data item.
 * \return Command id.
 **/
{
    return (m_id);
}


//----------------------------------------------------------------
const wxString& CommandToolBarTreeData::GetToolBarId() const
/**
 * \brief Returns the toolbar id for this data item.
 * \return Toolbar id.
 **/
{
    return (m_id);
}


} // namespace penv





