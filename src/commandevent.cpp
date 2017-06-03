/*
 * commandevent.cpp - Implementation of the CommandEvent class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */


#include "commandevent.h"
#include "command.h"
#include "penvhelper.h"

namespace penv {

DEFINE_EVENT_TYPE(penvEVT_COMMAND_EXECUTED);

//----------------------------------------------------------------
CommandEvent::CommandEvent()
/**
 * \brief Constructor, initialize this object.
 **/
{
    m_command = NULL;
}


//----------------------------------------------------------------
CommandEvent::CommandEvent(Command* command)
/**
 * \brief Constructor, initialize this object.
 * \param command Command which will be executed.
 **/
{
    m_command = command;
    //SetEventObject(command);
}


//----------------------------------------------------------------
CommandEvent::CommandEvent(Command* command, const wxVariant& variant)
/**
 * \brief Constructor, initialize this object.
 * \param command Command which will be executed.
 * \param variant Additionally information for command.
 **/
{
    m_command = command;
    m_variant = variant;
    //SetEventObject(command);
}


//----------------------------------------------------------------
CommandEvent::~CommandEvent()
/**
 * \brief Destructor.
 **/
{
    // Nothing to do.
}


//----------------------------------------------------------------
wxEvent* CommandEvent::Clone() const
/**
 * \brief Clones the this object and returns it as a wxEvent object.
 * \return Cloned object.
 **/
{
    CommandEvent* event = new CommandEvent(m_command, m_variant);
    return ((wxEvent*)event);
}



//----------------------------------------------------------------
const Command* CommandEvent::GetCommand() const
/**
 * \brief Returns the command, which is executed.
 * \return Executed command.
 **/
{
    return (m_command);
}


//----------------------------------------------------------------
wxVariant& CommandEvent::GetVariant()
/**
 * \brief Returns additional information connected to
 * this event as a variant.
 * \return Additional information.
 **/
{
    return (m_variant);
}



} // namespace penv



