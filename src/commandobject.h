/*
 * commandobject.h - Deklaration der CommandObject Klasse
 *
 * Author: Steffen Ott
 * Datum: 14.10.2007
 *
 */


#ifndef _PENV_COMMANDOBJECT_H_
#define _PENV_COMMANDOBJECT_H_

#include "commandevent.h"

namespace penv {

/**
 * \brief The CommandObject class is an abstract class for
 * the CommandCallback class. So that every connected callback
 * can be registered to the command. This method should only
 * be used framework internally.
 **/
class CommandObject
{
    public:
        friend class Command;

        /**
         * \brief Constructor, initializes this class.
         **/
        CommandObject() : m_connected(true) {}

        /**
         * \brief Destructor.
         **/
        virtual ~CommandObject() {}

        /**
         * \brief Calling execute on this method, will execute
         * the connected callback. This method is abstact.
         * \param event Delivered CommandEvent for this callback.
         **/
        virtual void Execute(CommandEvent& event) = 0;

        /**
         * \brief Simple compare method, compares if the given
         * CommandObject has the same callback as this class.
         * \param object The CommandObject to compare with.
         * \return True, if the same; false otherwise.
         **/
        inline bool Compare(const CommandObject* object)
        { return (m_ptr1 == object->m_ptr1 && m_ptr2 == object->m_ptr2); }

    protected:
        /**
         * \brief Sets if the CommandObject is connected
         * to a callback.
         * \param connected True, when CommandObject is connected.
         **/
        inline void Connected(bool connected = true)
        { m_connected = connected; }

        /**
         * \brief Returns if the CommandObject is connected
         * to a callback.
         * \return True, when CommandObject is connected; false otherwise.
         **/
        inline bool IsConnected() const
        { return (m_connected); }
    public:
        bool m_connected;
        void* m_ptr1;
        void* m_ptr2;
};


} // namespace penv


#endif // _PENV_COMMANDOBJECT_H_
