/*
 * toolbaritem.h - Declaration of the ToolBarItem class
 *
 * Author: Steffen Ott
 * Date: 11/05/2007
 *
 */


#ifndef _PENV_TOOLBARITEM_H_
#define _PENV_TOOLBARITEM_H_

#include "command.h"
#include "wxincludes.h"

namespace penv {

/**
 * \brief The ToolBarItem class is one toolbar item in
 * a toolbar. There are two kinds of toolbar items: normal
 * toolbar items which will be connected to an command and
 * sperators. To create a separator programmatically just
 * call the constructor with parameter command=NULL.<br>
 * Here the definition of a 'toolitem' xml element:<br>
 * <code>
 *  &lt;toolitem type="command" enabled="true" visible="true"&gt;COMMAND_ID&lt;/toolitem&gt;<br>
 *  &lt;toolitem type="separator" /&gt;<br>
 * </code>
 **/
class ToolBarItem
{
    private:
        friend class ToolBar;
    public:
        ToolBarItem(Command* command=NULL);
        ~ToolBarItem();

        bool IsSeparator() const;
        const wxString& GetName() const;
        const wxString& GetHelp() const;
        const wxBitmap& GetIcon() const;
        const wxString& GetId() const;

    private:
        void SetCommand(Command* command);
    public:
        Command* GetCommand();
    private:
        bool TryConnect();
    public:
        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

    private:
        void SetTool(wxToolBarToolBase* tool);
    public:
        wxToolBarToolBase* GetTool();

    private:
        wxToolBarToolBase* m_tool;
        wxString m_id;
        Command* m_command;
        bool m_enabled;
        bool m_visible;
        wxString m_empty;
};

} // namespace penv

#endif // _PENV_TOOLBARITEM_H_
