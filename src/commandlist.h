/*
 * commandlist.h - Declaration of the CommandList class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */

#ifndef _PENV_COMMANDLIST_H_
#define _PENV_COMMANDLIST_H_

#include "wxincludes.h"
#include "command.h"
#include "menubar.h"
#include "toolbarlist.h"
#include "shortcutlist.h"


namespace penv {

/**
 * \brief The CommandList class is a container for all commands.
 * It holds commands, which can be executed via an menuitem, toolitem
 * or shortcut, and events who can be executed programmatically.
 * Every Command class have an unique ID which will be stored in an
 * HashMap. So it's only needed to have this ID to execute a command.<br>
 * <b>Add new commands:</b> New commands can be added simple, by just
 * creating a new instance of a command and use Add() from this class.
 * Just remember, that command IDs must be unique.<br>
 * <b>Get all availible commands:</b> To get all command IDs you can
 * use the GetCommands() method. It will return every command ID who
 * is registered in this class.<br>
 * <b>Execute a command:</b> You can execute an command simply by
 * calling the Execute() method and deliver the command ID as a parameter.<br>
 * \todo Clear event registrations in menubar, toolbar and shortcut if any.
 **/
class CommandList
{
    private:
        friend class Environment;
        CommandList();
        CommandList(ApplicationFrame* frame, wxAuiManager* manager, wxMenuBar* menubar);
        ~CommandList();
        void InitializeEvents();
    public:

        ToolBarList* GetToolBarList();
        MenuBar* GetMenuBar();

        wxArrayString* GetCommands();
        Command* GetCommand(const wxString& id);
        bool Add(Command* cmd);
        bool Remove(const wxString& id, bool free = true);
        void Clear();

        bool Execute(const wxString& id);
        bool Execute(const wxString& id, const wxVariant& variant);


    private: // Wird nur von Evironment aufgerufen!!
        bool LoadMenuBarConfiguration(wxXmlNode* node);
        bool LoadToolBarsConfiguration(wxXmlNode* node);
        bool LoadShortcutsConfiguration(wxXmlNode* node);
        wxXmlNode* WriteMenuBarConfiguration();
        wxXmlNode* WriteToolBarsConfiguration();
        wxXmlNode* WriteShortcutsConfiguration();

    private:
        WX_DECLARE_STRING_HASH_MAP(Command*, CommandHashMap);
        CommandHashMap* m_hashmap;
        ToolBarList* m_toolbarlist;
        MenuBar* m_menubar;
        ShortcutList* m_shortcutlist;
};

} // namespace penv

#endif // _PENV_COMMANDLIST_H_
