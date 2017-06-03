/*
 * command.h - Declaration of the command class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */


#ifndef _PENV_COMMAND_H_
#define _PENV_COMMAND_H_

#include "wxincludes.h"
#include "commandevent.h"
#include "container/objarray.h"
#include "commandobject.h"
#include "commandcallback.h"


namespace penv {


/**
 * \brief The Command class represents an internal command in
 * the framework. An command must have an unique id, which is
 * needed to execute the command. An command is also an event,
 * the differences between them are that an normal command can
 * be connected to an menu item, toolbar item or shortcut.
 * An event on the other hand can only be executed programmatically.
 * You could connect one or more callbacks to a command, which
 * will be executed in order of their registration.<br>
 * Use Connect() to add callbacks to this command or Disconnect()
 * to disconnect them. Call Execute() to execute all the callbacks
 * registered whithin this command.
 * \todo Implement the Visible() method for the command class.
 **/
class Command : public wxEvtHandler
{
    public:
        friend class CommandList;
        Command();
        Command(const wxString& id, const wxString& name, const wxString& help, bool isevent = false);
        ~Command();

        wxString& GetId();
        void SetId(const wxString& id);
        wxString& GetName();
        void SetName(const wxString& name);
        wxString& GetHelp();
        void SetHelp(const wxString& help);
        const wxBitmap& GetIcon() const;
        void SetIcon(const wxBitmap& bitmap);

        bool IsEnabled() const;
        void Enable(bool enable = true);
        bool IsVisible() const;
        void Visible(bool visible = true, bool update = false);
        bool IsEvent() const;
        void SetEvent(bool isevent = true);

        size_t GetCallbacksCount() const;
        size_t Connect(CommandObject* callback);
        void Disconnect(size_t connectid);
        void Disconnect(CommandObject* object);
        void Execute();
        void Execute(const wxVariant& variant);
        void OnExecute(wxCommandEvent& event);

    private:
        wxString m_id;
        wxString m_name;
        wxString m_help;
        wxBitmap m_icon;
        bool m_enabled;
        bool m_visible;
        bool m_event;
        Array<CommandObject>* m_funcarray;
};


} // namespace penv

#endif // _PM_COMMAND_H_
