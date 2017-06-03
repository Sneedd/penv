/*
 * commandlist.cpp - Implementation of the CommandList class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */

#include "commandlist.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"

namespace penv {

//----------------------------------------------------------------
CommandList::CommandList()
/**
 * \brief Constructor (private). Not for use.
 **/
{
    wxLogError(_("[penv::CommandList::CommandList] Standard Constructor should not be called."));
}


//----------------------------------------------------------------
CommandList::CommandList(ApplicationFrame* frame, wxAuiManager* manager, wxMenuBar* menubar)
/**
 * \brief Constructor, initializes the CommandList object. This
 * constructor is private and only be called from the Environment
 * class.
 * \param frame The top level window.
 * \param manager The aui manager of the top level window.
 * \param menubar The internally used menubar of the top level window.
 **/
{
    m_hashmap = new CommandHashMap();
    m_toolbarlist = new ToolBarList(frame, manager);
    m_menubar = new MenuBar(menubar);
    m_shortcutlist = new ShortcutList();
    InitializeEvents();
}


//----------------------------------------------------------------
CommandList::~CommandList()
/**
 * \brief Destructor.
 **/
{
    if (m_toolbarlist != NULL) delete m_toolbarlist;
    if (m_menubar != NULL) delete m_menubar;
    if (m_shortcutlist != NULL) delete m_shortcutlist;
    if (m_hashmap != NULL) delete m_hashmap;
}



//----------------------------------------------------------------
void CommandList::InitializeEvents()
/**
 * \brief Initialize all common events which will be executed by the penv
 * Framework. All events of that type, begin with 'PENV_EVT_'.
 **/
{
    Add(new Command(_T("PENV_EVT_WINDOW_REGISTERED"), _T("penv window type registered"),
        _T("Occurs when a window type is registered to the framework."), true));
    Add(new Command(_T("PENV_EVT_WINDOW_UNREGISTERED"), _T("penv window type unregistered"),
        _T("Occurs when a window type is unregistered from the framework."), true));
    Add(new Command(_T("PENV_EVT_WINDOW_ADDED"), _T("penv window added"),
        _T("Occurs when a window is added to the framework."), true));
    Add(new Command(_T("PENV_EVT_WINDOW_CLOSED"), _T("penv window closed"),
        _T("Occurs when a window is closed in the framework."), true));

    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_ADDCLICKED"), _T("penv projecttreecontrol add clicked"),
        _T("Occurs when a toolbar item 'add item' is clicked in the projecttreecontrol."), true));
    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_ITEM_ACTIVATED"), _T("penv projecttreecontrol item activated"),
        _T("Occurs when tree item is double clicked in the projecttreecontrol."), true));

    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_COPIED"), _T("penv projecttreecontrol item copied"),
        _T("Occurs when tree item is copied in the projecttreecontrol."), true));
    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_MOVED"), _T("penv projecttreecontrol item moved"),
        _T("Occurs when tree item is moved in the projecttreecontrol."), true));
    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_RENAMED"), _T("penv projecttreecontrol item renamed"),
        _T("Occurs when tree item is renamed in the projecttreecontrol."), true));
    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_DELETED"), _T("penv projecttreecontrol item deleted"),
        _T("Occurs when tree item is deleted in the projecttreecontrol."), true));

//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_COPIED_PROJECT_TO_WORKSPACE"), _T("penv project copied to workspace"),
//        _T("Occurs when a project is copied to a workspace."), true));
//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_COPIED_SUBPROJECT_TO_WORKSPACE"), _T("penv subproject copied to workspace"),
//        _T("Occurs when a subproject is copied to a workspace."), true));
//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_COPIED_PROJECTITEM_TO_PROJECT"), _T("penv projectitem copied to project"),
//        _T("Occurs when a project item is copied to a project."), true));
//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_COPIED_PROJECTITEM_TO_PROJECTITEM"), _T("penv projectitem copied to projectitem"),
//        _T("Occurs when a projectitem is copied to a projectitem."), true));
//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_MOVED_PROJECT_TO_WORKSPACE"), _T("penv project moved to workspace"),
//        _T("Occurs when a project is moved to a workspace."), true));
//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_MOVED_SUBPROJECT_TO_WORKSPACE"), _T("penv subproject moved to workspace"),
//        _T("Occurs when a subproject is moved to a workspace."), true));
//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_MOVED_PROJECTITEM_TO_PROJECT"), _T("penv projectitem moved to project"),
//        _T("Occurs when a project item is moved to a project."), true));
//    Add(new Command(_T("PENV_EVT_PROJECTTREECTRL_MOVED_PROJECTITEM_TO_PROJECTITEM"), _T("penv projectitem moved to projectitem"),
//        _T("Occurs when a projectitem is moved to a projectitem."), true));


    Add(new Command(_T("PENV_EVT_PROJECT_COPIED"), _T("penv project copied"),
        _T("Occurs when a project is successfully copied to another workspace."), true));
    Add(new Command(_T("PENV_EVT_PROJECT_MOVED"), _T("penv project moved"),
        _T("Occurs when a project is successfully moved to another workspace."), true));
    Add(new Command(_T("PENV_EVT_PROJECTITEM_COPIED"), _T("penv project item copied"),
        _T("Occurs when a project is item successfully copied to another project or project item."), true));
    Add(new Command(_T("PENV_EVT_PROJECTITEM_MOVED"), _T("penv project item moved"),
        _T("Occurs when a project item is successfully moved to another project or project item."), true));


    Add(new Command(_T("PENV_EVT_ACTIVATED_WORKSPACE"), _T("penv workspace activated"),
        _T("Occurs when a workspace is activated."), true));
    Add(new Command(_T("PENV_EVT_ACTIVATED_PROJECT"), _T("penv project activated"),
        _T("Occurs when a project is activated."), true));
    Add(new Command(_T("PENV_EVT_ACTIVATED_PROJECTITEMFILE"), _T("penv project item file activated"),
        _T("Occurs when a project item file is activated."), true));
    Add(new Command(_T("PENV_EVT_ACTIVATED_PROJECTITEMDIRECTORY"), _T("penv project item directory activated"),
        _T("Occurs when a project item directory is activated."), true));
    Add(new Command(_T("PENV_EVT_ACTIVATED_PROJECTITEMLINKEDITEMS"), _T("penv project item linked items activated"),
        _T("Occurs when a project item linked items is activated."), true));
    Add(new Command(_T("PENV_EVT_ACTIVATED_PROJECTITEMSUBPROJECT"), _T("penv project item sub project activated"),
        _T("Occurs when a project item sub project is activated."), true));










}


//----------------------------------------------------------------
ToolBarList* CommandList::GetToolBarList()
/**
 * \brief Returns the ToolBarList, which handles all toolbars and their
 * items.
 * \return Returns the ToolBarList.
 **/
{
    return (m_toolbarlist);
}


//----------------------------------------------------------------
MenuBar* CommandList::GetMenuBar()
/**
 * \brief Returns the MenuBar, which handles all menu items.
 * \return Returns the MenuBar.
 **/
{
    return (m_menubar);
}


//----------------------------------------------------------------
wxArrayString* CommandList::GetCommands()
/**
 * \brief Returns all command ids registered in this class.
 * The returned string array must be destroyed by the caller.
 * \return String array with command ids registered to this class.
 **/
{
    wxArrayString* array = new wxArrayString();
    array->Alloc(m_hashmap->size());
    CommandHashMap::iterator it;
    for( it = m_hashmap->begin(); it != m_hashmap->end(); ++it )
    {
        wxString key = it->first;
        array->Add(key);
    }
    return (array);
}


//----------------------------------------------------------------
Command* CommandList::GetCommand(const wxString& id)
/**
 * \brief Returns a commnad by its id.
 * \param id Command ID.
 * \return command with the given id; or NULL if command not exists.
 **/
{
    if (unlikely(m_hashmap->find(id) == m_hashmap->end())) {
        return (NULL);
    }
    return ((*m_hashmap)[id]);
}


//----------------------------------------------------------------
bool CommandList::Add(Command* cmd)
/**
 * \brief Adds a event or command to the CommandList. The command
 * which will be added must have a unique id defined by the framework
 * user.
 * \param cmd Command which should be added.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(cmd == NULL)) {
        wxLogWarning(_T("[penv::CommandList::Add] Could not add event or command, because parameter argument is NULL."));
        return (false);
    }
    // Wenn keine ID angegeben wurde also der String leer ist,
    // muss eine Fehlermeldung ausgegeben werden. Eindeutige IDs
    // für Commands können nur vom Programmierer gesetzt werden.
    if (unlikely(cmd->m_id.IsEmpty()))
    {
        wxLogError(_T("[penv::CommandList::Add] Could not add event or command with id field left empty."));
        return (false);
    }
    // Überprüfen ob die ID schon existiert.
    CommandHashMap::iterator itr = m_hashmap->find(cmd->m_id);
    if (unlikely(itr != m_hashmap->end()))
    {
        wxLogError(_T("[penv::CommandList::Add] Could not add event or command, because the id \"%\" already exists."),cmd->GetId().c_str());
        return (false);
    }
    // Hinzufügen zu Hashmap
    (*m_hashmap)[cmd->m_id] = cmd;
    return (true);
}


//----------------------------------------------------------------
bool CommandList::Remove(const wxString& id, bool free)
/**
 * \brief Removes a event or command from this container.
 * All registrations will be removed and the command cannot
 * be executed anymore.
 * \param id Command id of the command which should be removed.
 * \param free True if the command should be deleted.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_hashmap->find(id) == m_hashmap->end())) {
        wxLogError(_T("[penv::CommandList::Remove] There is no event or command with the id \"%s\" to remove."), id.c_str());
        return (false);
    }
    // TODO: Clear event registration if any in menubar, toolbars and shortcuts
    if (unlikely(m_hashmap->erase(id) == 0)) {
        wxLogError(_T("[penv::CommandList::Remove] Could not remove event or command with id \"%s\" from hash table."), id.c_str());
        return (false);
    }
    return (true);
}


//----------------------------------------------------------------
void CommandList::Clear()
/**
 * \brief Removes all registered commands or events in this
 * container.
 **/
{
    // TODO: Clear event registration if any in menubar, toolbars and shortcuts
    m_hashmap->clear();
}


//----------------------------------------------------------------
bool CommandList::Execute(const wxString& id)
/**
 * \brief Executes a event or command by its id.
 * \param id The command id of the command which should be executed.
 * \return True on succcess; false otherwise.
 **/
{
    if (unlikely(m_hashmap->find(id) == m_hashmap->end())) {
        wxLogWarning(_T("[penv::CommandList::Execute] The command id '%s' does not exist in this container."), id.c_str());
        return (false);
    }
    (*m_hashmap)[id]->Execute();
    return (true);
}



//----------------------------------------------------------------
bool CommandList::Execute(const wxString& id, const wxVariant& variant)
/**
 * \brief Executes a event or command by its id.
 * \param id The command id of the command which should be executed.
 * \param variant Additional information for the callback registered
 * to this command or event.
 * \return True on succcess; false otherwise.
 **/
{
    if (unlikely(m_hashmap->find(id) == m_hashmap->end())) {
        wxLogWarning(_T("[penv::CommandList::Execute] The command id '%s' does not exist in this container."), id.c_str());
        return (false);
    }
    (*m_hashmap)[id]->Execute(variant);
    return (true);
}




//----------------------------------------------------------------
bool CommandList::LoadMenuBarConfiguration(wxXmlNode* node)
/**
 * \brief Loads the menubar configuration form delivered
 * XML element node. Updates the menubar afterwards.
 * \param node XML element node "menubar"
 * \return True on success; false otherwise
 **/
{
    if (m_menubar->ReadNode(node)) {
        if (m_menubar->Update()) {
            return (true);
        }
    }
    return (false);
}


//----------------------------------------------------------------
bool CommandList::LoadToolBarsConfiguration(wxXmlNode* node)
/**
 * \brief Loads the toolbar configuration form delivered
 * XML element node and updates the toolbar after that.
 * \param node XML element node "toolbars"
 * \return True on success; false otherwise
 **/
{
    if (m_toolbarlist->ReadNode(node)) {
        if (m_toolbarlist->Update()) {
            return (true);
        }
    }
    return (false);
}


//----------------------------------------------------------------
bool CommandList::LoadShortcutsConfiguration(wxXmlNode* node)
/**
 * \brief This method loads th shortcut configuration
 * from the delivered XML element node and
 * \param node XML element node "shortcuts"
 * \return True on success; false otherwise
 **/
{
    if (m_shortcutlist->ReadNode(node)) {
        if (m_shortcutlist->Update()) {
            return (true);
        }
    }
    return (false);
}


//----------------------------------------------------------------
wxXmlNode* CommandList::WriteMenuBarConfiguration()
/**
 * \brief This Method writes the configuration for
 * the menubar into an XML element node.
 * \return XML element node
 **/
{
    wxXmlNode* node = m_menubar->WriteNode();
    return (node);
}


//----------------------------------------------------------------
wxXmlNode* CommandList::WriteToolBarsConfiguration()
/**
 * \brief This Method writes the configuration for
 * the toolbars into an XML element node.
 * \return XML element node
 **/
{
    wxXmlNode* node = m_toolbarlist->WriteNode();
    return (node);
}


//----------------------------------------------------------------
wxXmlNode* CommandList::WriteShortcutsConfiguration()
/**
 * \brief This Method writes the configuration for
 * the shortcut list into an XML element node.
 * \return XML element node
 **/
{
    wxXmlNode* node = m_shortcutlist->WriteNode();
    return (node);
}





} // namespace penv


