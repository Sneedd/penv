/*
 * commandevent.h - Declaration of the CommandEvent class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */


#ifndef _PENV_COMMANDEVENT_H_
#define _PENV_COMMANDEVENT_H_

#include "wxincludes.h"

namespace penv {

// Forward declaration
class Command;

/**
 * \brief The CommandEvent class will be generated by the command
 * class when an command is executed. This class just holds information
 * about the command and eventually an wxVariant object for more
 * information on the event.
 **/
class CommandEvent
{
    public:
        CommandEvent();
        CommandEvent(Command* command);
        CommandEvent(Command* command, const wxVariant& variant);
        virtual ~CommandEvent();

        virtual wxEvent* Clone() const;
        const Command* GetCommand() const;
        wxVariant& GetVariant();

    private:
        wxVariant m_variant;
        Command* m_command;
};


} // namespace penv

#endif // _PENV_COMMANDEVENT_H_