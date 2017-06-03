/*
 * toolbaritem.cpp - Implementation of the ToolBarItem class
 *
 * Author: Steffen Ott
 * Date: 11/05/2007
 *
 */


#include "toolbaritem.h"
#include "penvhelper.h"
#include "environment.h"

namespace penv {




//----------------------------------------------------------------
ToolBarItem::ToolBarItem(Command* command)
/**
 * \brief Constructor, initializes the ToolBarItem object.
 * Because we have two types of toolbar items, a normal icon
 * which is connected with a command and a separator, you can
 * call this constructor with a command to create a icon or
 * set command to NULL to create a separator.
 * \param command The command connected to the toolbar item or
 * NULL if toolbar item should be a separator.
 **/
{
    m_command = command;
    m_tool = NULL;
    if (m_command == NULL) {
        m_id = wxEmptyString;
        m_enabled = true;
        m_visible = true;
    } else {
        m_id = m_command->GetId();
        m_enabled = m_command->IsEnabled();
        m_visible = m_command->IsVisible();
    }
}

//----------------------------------------------------------------
ToolBarItem::~ToolBarItem()
/**
 * \brief Destructor.
 **/
{
    // TEST: Event verbindung gelöscht? Nach ToolBar::Update()
}

//----------------------------------------------------------------
bool ToolBarItem::IsSeparator() const
/**
 * \brief Returns true if this toolbar item is a spearator.
 * \return True if toolbar item is a separator; false otherwise.
 **/
{
    if (m_command == NULL && !m_id.IsEmpty()) {
        wxLogError(_T("[penv::ToolBarItem::IsSeparator] ToolBar item not connected to the command '%s'."), m_id.c_str());
        return (false);
    }
    return (m_command == NULL);
}

//----------------------------------------------------------------
const wxString& ToolBarItem::GetName() const
/**
 * \brief Returns the name of the toolbar item. Returns the command
 * name if toolbar item is a command or an empty string if toolbar
 * item is a separator.
 * \return Name of the toolbar item or empty string.
 **/
{
    if (m_command == NULL) return (m_empty);
    return (m_command->GetName());
}

//----------------------------------------------------------------
const wxString& ToolBarItem::GetHelp() const
/**
 * \brief Returns the help of the toolbar item. Returns the command
 * help if toolbar item is a command or an empty string if toolbar
 * item is a separator.
 * \return Help of the toolbar item or empty string.
 **/
{
    if (m_command == NULL) return (m_empty);
    return (m_command->GetHelp());
}

//----------------------------------------------------------------
const wxBitmap& ToolBarItem::GetIcon() const
/**
 * \brief Returns the icon of the toolbar item. Returns the command
 * icon if toolbar item is a command or an null bitmap if toolbar
 * item is a separator.
 * \return Icon of the toolbar item or null bitmap.
 **/
{
    if (m_command == NULL) return (wxNullBitmap);
    return (m_command->GetIcon());
}


//----------------------------------------------------------------
const wxString& ToolBarItem::GetId() const
/**
 * \brief Returns the command id of the toolbar item. If the
 * toolbar item is a separator, then an empty string will be returned.
 * \return Command id of the toolbar item or empty string.
 **/
{
    return (m_id);
}



//----------------------------------------------------------------
void ToolBarItem::SetCommand(Command* command)
/**
 * \brief Sets the command of the toolbar item. This method is
 * private and therefor only used internally.
 * \param command The command of the toolbar item.
 **/
{
    m_command = command;
}


//----------------------------------------------------------------
Command* ToolBarItem::GetCommand()
/**
 * \brief Returns the command of the toolbar item. This method
 * returns NULL, if the toolbar item is not connected or a
 * separator.
 * \return Command connected to toolbar item.
 **/
{
    return (m_command);
}



//----------------------------------------------------------------
bool ToolBarItem::TryConnect()
/**
 * \brief Connects a command event with this toolbar item.
 * This method is private and therefor only used internally.
 * \return True on success; false otherwise.
 **/
{
    Environment* env = Environment::Get();
    env->GetFrame()->Connect(m_tool->GetId(), wxEVT_COMMAND_TOOL_CLICKED,
        wxCommandEventHandler(Command::OnExecute), NULL, m_command);
    return (true);
}

//----------------------------------------------------------------
bool ToolBarItem::ReadNode(wxXmlNode* node)
/**
 * \brief Reads out a 'toolitem' xml element.
 * See class description for more information.
 * \param node The 'toolitem' xml element.
 * \return True on success; false otherwise.
 **/
{
    // Fehler überprüfen
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::ToolBarItem::ReadNode] Cannot load xml node, because parameter argument is NULL."));
        return (false);
    }
    if (unlikely(node->GetType() != wxXML_ELEMENT_NODE)) {
        wxLogError(_T("[penv::ToolBarItem::ReadNode] Cannot load, xml node must be an element node."));
        return (false);
    }
    if (unlikely(node->GetName() != _T("toolitem"))) {
        wxLogError(_T("[penv::ToolBarItem::ReadNode] Cannot load, xml element node must have the name \"toolitem\". This element node has the name \"%s\"."), node->GetName().c_str());
        return (false);
    }

    // Einlesen des ToolItems
    if (unlikely(!node->HasProp(_T("type")))) {
        wxLogError(_T("[penv::ToolBarItem::ReadNode] Cannot load \"toolitem\", because it has no \"type\" attribute."));
        return (false);
    }
    if (node->GetPropVal(_T("command"),_T("separator"))) {
        m_id = node->GetNodeContent().Trim(true).Trim(false);
    }
    m_enabled = PenvHelper::ParseBoolean(node->GetPropVal(_T("enabled"), _T("false")));
    m_visible = PenvHelper::ParseBoolean(node->GetPropVal(_T("visible"), _T("false")));
    return (true);
}

//----------------------------------------------------------------
wxXmlNode* ToolBarItem::WriteNode()
/**
 * \brief Writes a 'toolitem' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'toolitem' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = NULL;
    if (IsSeparator())
    {
        node = PenvHelper::CreateXmlNode(_T("toolitem"));
        node->AddProperty(_T("type"), _T("separator"));
    }
    else
    {
        node = PenvHelper::CreateXmlNode(_T("toolitem"), m_command->GetId());
        node->AddProperty(_T("type"), _T("command"));
        node->AddProperty(_T("enabled"), PenvHelper::CreateBoolean(m_enabled));
        node->AddProperty(_T("visible"), PenvHelper::CreateBoolean(m_visible));
    }
    return (node);
}


//----------------------------------------------------------------
void ToolBarItem::SetTool(wxToolBarToolBase* tool)
/**
 * \brief Sets the internally toolbar item of this toolbar item.
 * This method is private and therefor only used internally.
 * \param tool Internally used toolbar item.
 **/
{
    m_tool = tool;
}



//----------------------------------------------------------------
wxToolBarToolBase* ToolBarItem::GetTool()
/**
 * \brief Returns the internally used toolbar item of
 * this toolbar item.
 * \return Internally used toolbar item.
 **/
{
    return (m_tool);
}




} // namespace penv



