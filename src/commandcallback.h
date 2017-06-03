/*
 * commandcallback.h - Declaration of the CommandCallback class
 *
 * Author: Steffen Ott
 * Date: 10/14/2007
 *
 */


#ifndef _PENV_COMMANDCALLBACK_H_
#define _PENV_COMMANDCALLBACK_H_

#include "commandobject.h"

namespace penv {


/**
 * \brief The CommandCallback class saves the class pointer and method
 * pointer of an callback. Because various classes can be connected to
 * an command this class is a template. The best way to create this class
 * is with a call similar to:<br>
 * <code>
 * CommandObject* object = new penv::CommandCallback<MyClass>(this,&MyClass::MyMethod)<br>
 * </code>
 * Where MyClass is the class which hold the MyMethod method who should
 * be called, when executing an command.
 **/
template<class C>
class CommandCallback : public CommandObject
{
    private:
        CommandCallback() {}
        CommandCallback(const CommandCallback& object) {}
        CommandCallback& operator=(const CommandCallback& object) {}
    public:
        typedef void (C::*penvCommandEventFunction)(CommandEvent&);
        CommandCallback(C* object, penvCommandEventFunction method);
        ~CommandCallback();

        void Execute(CommandEvent& event);


    private:
        C* m_object;
        penvCommandEventFunction m_method;
};



//----------------------------------------------------------------
template<class C>
CommandCallback<C>::CommandCallback(C* object, penvCommandEventFunction method)
/**
 * \brief Constructor, which inititializes the callback method and class.
 * \param object Pointer to the class.
 * \param method Pointer to the method.
 **/
{
    m_object = object;
    m_method = method;
    m_ptr1 = (void*)m_object;
    m_ptr2 = (void*)&m_method;
}


//----------------------------------------------------------------
template<class C>
CommandCallback<C>::~CommandCallback()
/**
 * \brief Destruktor, deinitialize the class.
 **/
{
    // Nothing to do.
}



//----------------------------------------------------------------
template<class C>
void CommandCallback<C>::Execute(CommandEvent& event)
/**
 * \brief Executes the internally stored callback.
 * \param event The event which will be given to the callback.
 **/
{
    (m_object->*m_method)(event);
}


} // namespace penv

#endif // _PENV_COMMANDCALLBACK_H_
