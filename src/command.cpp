/*
 * command.cpp - Implemantation of the Command class
 *
 * Author: Steffen Ott
 * Date: 09/21/2007
 *
 */

#include "command.h"
#include "commandlist.h"
#include "environment.h"
#include "penvhelper.h"

namespace penv {

//----------------------------------------------------------------
Command::Command()
/**
 * \brief Constructor, initializes a empty command.
 * The prefered constructor should be the other one.
 **/
{
    m_id = wxEmptyString;
    m_name = wxEmptyString;
    m_help = wxEmptyString;
    m_enabled = true;
    m_visible = true;
    m_funcarray = NULL;
    m_event = false;
}


//----------------------------------------------------------------
Command::Command(const wxString& id, const wxString& name, const wxString& help, bool isevent)
/**
 * \brief Constructor, which initialize a filled Command object.
 * \param id An unique ID for the command.
 * \param name Name of the command.
 * \param help A longer help for the command.
 * \param isevent True if this command is an internally used event.
 **/
{
    m_id = id;
    m_name = name;
    m_help = help;
    m_enabled = true;
    m_visible = true;
    m_funcarray = NULL;
    m_event = isevent;
}



//----------------------------------------------------------------
Command::~Command()
/**
 * \brief Destructor.
 **/
{
    // Events aushängen
    if (m_funcarray != NULL) {
        m_funcarray->Clear(false);
        delete m_funcarray;
    }
}




//----------------------------------------------------------------
wxString& Command::GetId()
/**
 * \brief Returns the ID of the command. The ID of an command must
 * be unique. Because with that ID an command, can be executed.
 * \return ID of the Command.
 **/
{
    return (m_id);
}



//----------------------------------------------------------------
void Command::SetId(const wxString& id)
/**
 * \brief Sets the ID of the command. Because the ID is unique,
 * you must remove the command from the commandlist before
 * changing its ID. After you changed the ID you could add
 * the command to the CommandList again. To remove an command
 * from the CommandList without deleteing it, you should call
 * the Remove()-Method from CommandList with the ID and false.
 * \param id The command ID.
 **/
{
    m_id = id;
}




//----------------------------------------------------------------
wxString& Command::GetName()
/**
 * \brief Returns the name of the command. The name is the label of a
 * command, which will be shown in the menubar or the tooltip of a
 * toolbar item.
 * \return Name of the command.
 **/
{
    return (m_name);
}



//----------------------------------------------------------------
void Command::SetName(const wxString& name)
/**
 * \brief Sets the name of a command. The name is the label of a
 * command, which will be shown in the menubar or the tooltip of a
 * toolbar item.
 * \param name The name of the command.
 **/
{
    m_name = name;
}




//----------------------------------------------------------------
wxString& Command::GetHelp()
/**
 * \brief Returns the help (also called long help under wxWidgets) of
 * the command. This help will be shown in the statusbar of the
 * application, when the mouse pointer is over a menu item oder toolbar
 * item.
 * \return Help of the command.
 **/
{
    return (m_help);
}



//----------------------------------------------------------------
void Command::SetHelp(const wxString& help)
/**
 * \brief Sets the help (also called long help under wxWidgets) of
 * the command. This help will be shown in the statusbar of the
 * application, when the mouse pointer is over a menu item oder toolbar
 * item.
 * \param help Help of the command.
 **/
{
    m_help = help;
}


//----------------------------------------------------------------
const wxBitmap& Command::GetIcon() const
/**
 * \brief Returns the icon of the command. The icon will be used for
 * the toolbar and in the menu item.
 * \return The icon of the command.
 **/
{
    return (m_icon);
}

//----------------------------------------------------------------
void Command::SetIcon(const wxBitmap& bitmap)
/**
 * \brief Sets the icon of the command. The icon will be used for
 * the toolbar and in the menu item.
 * \param bitmap The icon of the command.
 **/
{
    m_icon = bitmap;
}




//----------------------------------------------------------------
bool Command::IsEnabled() const
/**
 * \brief Checks if a command is enabled. If a command is disabled,
 * then this command cannot be executed. Menu items or toolbar items,
 * which are connected to this command, will be grayed out.
 * \return True if command is enabled; false if disabled.
 **/
{
    return (m_enabled);
}



//----------------------------------------------------------------
void Command::Enable(bool enable)
/**
 * \brief Sets if a command is enabled or disabled. Disabled
 * commands cannot be executed and are grayed out if connected
 * to a toolbar item or menu item.
 * \param enable  True if command is enabled; false if disabled.
 **/
{
    if (m_enabled == enable) return;
    m_enabled = enable;
    // Events are not connected to toolbar item or menu item
    if (m_event) return;
    // Find all toolbar or menuitems connected with this command
    MenuBar* menubar = Environment::Get()->GetCommandList()->GetMenuBar();
    ToolBarList* tbarlist = Environment::Get()->GetCommandList()->GetToolBarList();
    // Search in MenuBar
    Array<MenuItem>* mitems = menubar->FindMenuItems(m_id);
    for (size_t i=0; i<mitems->Count(); ++i)
    {
        wxMenuItem* menuitem = (*mitems)[i]->GetMenuItem();
        if (menuitem != NULL) menuitem->Enable(m_enabled);
    }
    // Search in ToolBars
    Array<ToolBarItem>* tbitems = tbarlist->FindToolBarItems(m_id);
    for (size_t i=0; i<tbitems->Count(); ++i)
    {
        wxToolBarToolBase* toolitem = (*tbitems)[i]->GetTool();
        if (toolitem != NULL) toolitem->Enable(m_enabled);
    }
    tbarlist->RealizeAllToolBars();
}




//----------------------------------------------------------------
bool Command::IsVisible() const
/**
 * \brief Returns if the command is visible.
 * \return True if command is shown; false if hidden.
 **/
{
    return (m_visible);
}



//----------------------------------------------------------------
void Command::Visible(bool visible, bool update)
/**
 * \brief Sets the visibility of a command. The change of
 * the visibility of an event, does not affect the event
 * in any way. But changing the visibility of a command
 * which is connected to a menuitem or toolbar item hides
 * or shows this item. You must update the toolbar or
 * menubar, after changing this property. For performance
 * reasons you should change all commands which you want
 * to change and then call this method with parameter
 * update = 'true', to commit the changes.
 * \param visible True to show command; false to hide it.
 * \param update True to update menubar and toolbars; false
 * does not commit any changes.
 **/
{
    // TODO SO
    NOT_IMPLEMENTED_YET();
}



//----------------------------------------------------------------
bool Command::IsEvent() const
/**
 * \brief Returns if the command is a event. A event can only
 * be executed programmatically. A event cannot be connected to
 * a menu item, toolbar item or shortcut.
 * \return True if command is a event; false if not.
 **/
{
    return (m_event);
}


//----------------------------------------------------------------
void Command::SetEvent(bool isevent)
/**
 * \brief Sets if the command is a event or not. A event can only
 * be executed programmatically. A event cannot be connected to
 * a menu item, toolbar item or shortcut.
 * \param isevent True if command is a event; false if not.
 **/
{
    m_event = isevent;
}



//----------------------------------------------------------------
size_t Command::GetCallbacksCount() const
/**
 * \brief Returns the number of registered callbacks.
 * \return Registered callbacks count.
 **/
{
    if (m_funcarray == NULL) return (0);
    return (m_funcarray->Count());
}


//----------------------------------------------------------------
size_t Command::Connect(CommandObject* object)
/**
 * \brief Connects a callback to a command. You will be able to
 * connect more than one callback to a command. Every callback will
 * be called in order of their connection. This means if you connect
 * callback A and then callback B on a execute the callback A will
 * be first exectuted and then callback B.<br>
 * Example:<br>
 * <code>
 * CommandObject* object = new penv::CommandCallback<MyClass>(this,&MyClass::MyMethod)<br>
 * command->Connect(object);<br>
 * </code>
 * \param object The connected callback.
 * \return Returns the connected id of the command. Can be used to
 * disconnect the callback.
 **/
{
    // Funktion Array neu initialisieren
    if (unlikely(m_funcarray == NULL)) {
        m_funcarray = new Array<CommandObject>();
    }
    // Testen ob Funktion schon im Array existiert
    for (size_t i=0; i<m_funcarray->Count(); ++i) {
        // Testen ob schon registriert
        if (unlikely(object->Compare(m_funcarray->ItemPtr(i)))) {
            // Wenn schon registriert aber nicht verbunden, dann
            // wieder verbinden und ohne Warning beenden.
            if (unlikely(!object->IsConnected())) {
                object->Connected();
                return (i);
            }
            wxLogWarning(_T("[penv::Command::Connect] Function already connected to the command."));
            return (i);
        }
    }
    // Hinzufügen zum Array
    m_funcarray->Add(object);
    return (m_funcarray->Count()-1);
}



//----------------------------------------------------------------
void Command::Disconnect(size_t connectid)
/**
 * \brief Disconnects a callback by its position in the callback array.
 * \param connectid The id of the callback in the array.
 **/
{
    if (unlikely(m_funcarray == NULL)) {
        wxLogWarning(_T("[penv::Command::Disconnect] Cannot disconnect function where no function available or function NULL."));
        return;
    }
    if (unlikely(connectid < 0 || connectid >= m_funcarray->Count())) {
        wxLogWarning(_T("[penv::Command::Disconnect] Function ID not in the range of callback array."));
        return;
    }
    // Cleanup kann nur beim Beenden der Anwendung stattfinden
    (*m_funcarray)[connectid]->Connected(false);
}



//----------------------------------------------------------------
void Command::Disconnect(CommandObject* object)
/**
 * \brief Disconnects a callback in the callback array. Remeber to destroy
 * the created CommandObject, see also example.
 * Example:<br>
 * <code>
 * CommandObject* object = new penv::CommandCallback<MyClass>(this,&MyClass::MyMethod)<br>
 * command->Disconnect(object);<br>
 * delete object;<br>
 * </code>
 * \param object The callback, which should be disconnected.
 **/
{
    if (unlikely(m_funcarray == NULL)) {
        wxLogWarning(_T("[penv::Command::Disconnect] Cannot disconnect function where no function available or function NULL."));
        return;
    }
    // Find callback
    for (size_t i=0; i<m_funcarray->Count(); ++i)
    {
        if (object->Compare((*m_funcarray)[i]))
        {
            (*m_funcarray)[i]->Connected(false);
            return;
        }
    }
    wxLogError(_T("[penv::Command::Disconnect] Cannot find callback from command '%s' to disconnect it."), m_id.c_str());
}



//----------------------------------------------------------------
void Command::Execute()
/**
 * \brief Executes this command. All registered callbacks will be called.
 **/
{
    if (!m_enabled) {
        wxLogWarning(_T("[penv::Command::Execute] Cannot execute disabled command '%s'."), m_id.c_str());
        return;
    }
    if (m_funcarray == NULL) return;
    CommandEvent event(this);
    // Alle verbundenen Funktionen aufrufen
    for (size_t i=0; i<m_funcarray->Count(); ++i)
    {
        // Funktion holen und aufrufen
        CommandObject* function = (*m_funcarray)[i];
        if (likely(function->IsConnected())) {
            function->Execute(event);
        }
    }
}




//----------------------------------------------------------------
void Command::Execute(const wxVariant& variant)
/**
 * \brief Executes this command with a additional parameter. All
 * registered callbacks will be executed.
 * \param variant Additional parameter for the command.
 **/
{
    if (!m_enabled) {
        wxLogWarning(_T("[penv::Command::Execute] Cannot execute disabled command '%s'."), m_id.c_str());
        return;
    }
    if (m_funcarray == NULL) return;
    CommandEvent event(this, variant);
    // Alle verbundenen Funktionen aufrufen
    for (size_t i=0; i<m_funcarray->Count(); ++i)
    {
        // Funktion holen und aufrufen
        CommandObject* function = (*m_funcarray)[i];
        if (likely(function->IsConnected())) {
            function->Execute(event);
        }
    }
}



//----------------------------------------------------------------
void Command::OnExecute(wxCommandEvent& event)
/**
 * \brief Executes this command. This method is only used
 * internally and it is always better to call Execute(). This
 * Method will normaly only be called from a menu item or tool item.
 * \param event wxCommandEvent from wxWidgets.
 **/
{
    Execute();
}




} // namespace penv
