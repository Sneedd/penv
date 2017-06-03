/*
 * menuitem.cpp - implementation of the MenuItem class
 *
 * Author: Steffen Ott
 * Date: 10/16/2007
 *
 */


#include "menuitem.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"

namespace penv {


//----------------------------------------------------------------
MenuItem::MenuItem()
/**
 * \brief Constructor, initializes the menuitem. This constructor
 * is private and only used framework internally.
 **/
{
    m_command = NULL;
    m_submenu = NULL;
    m_connected = false;
    m_emptystring = _T("");
    m_type = penvMI_Separator;
    m_enabled = true;
    m_visible = true;
    m_menuitem = NULL;
}


//----------------------------------------------------------------
MenuItem::MenuItem(MenuItemType type, Command* command)
/**
 * \brief Constructor, initializes the menuitem.
 * \param type Type of the menuitem.
 * \param command Command id for the menuitem, only needed
 * for Command, Radio and Checkbox menuitems.
 **/
{
    switch (type)
    {
        case penvMI_Command:
            m_submenu = NULL;
            m_command = command;
            m_id = command->GetId();
        break;
        case penvMI_Submenu:
            m_command = NULL;
            m_submenu = new Array<MenuItem>();
        break;
        case penvMI_Separator:
            m_command = NULL;
            m_submenu = NULL;
        break;
        case penvMI_Checkbox:
            NOT_IMPLEMENTED_YET();
        break;
        case penvMI_Radio:
            NOT_IMPLEMENTED_YET();
        break;
    }
    m_type = type;
    m_connected = false;
    m_emptystring = _T("");
    m_enabled = true;
    m_visible = true;
    m_menuitem = NULL;
}


//----------------------------------------------------------------
MenuItem::~MenuItem()
/**
 * \brief Destructor.
 **/
{
    if (m_submenu != NULL) delete m_submenu;
}

//----------------------------------------------------------------
const wxString& MenuItem::GetName() const
/**
 * \brief Returns the name of the menuitem. If the type of the menuitem
 * is command, radio or checkbox the commands name will be returned, but only
 * if the menuitem is connected. If the menuitem is not connected, then this
 * method will return the id of the menuitem. The other types separator for
 * example has no name and therefor this method will return an empty string.
 * A submenu is never connected to a command and has its own name stored in
 * this class, which will be returned.
 * \return Name of menu item.
 **/
{
    if (m_type == penvMI_Command || m_type == penvMI_Checkbox || m_type == penvMI_Radio) {
        if (unlikely(m_command == NULL)) {
            return (m_id);
        } else {
            return (m_command->GetName());
        }
    }
    return (m_name);
}


//----------------------------------------------------------------
void MenuItem::SetName(const wxString& name)
/**
 * \brief Sets the name of the menuitem. (See also GetName().)
 * \return Name of menu item.
 **/
{
    m_name = name;
}


//----------------------------------------------------------------
MenuItemType MenuItem::GetType() const
/**
 * \brief Returns the type of the menuitem.
 * \return Type of the menuitem.
 **/
{
    return (m_type);
}



//----------------------------------------------------------------
const wxString& MenuItem::GetHelp() const
/**
 * \brief Returns the help for this menuitem. This method is
 * likely the GetName() method. It will only return a help,
 * when the type of the menu item is a command, checkbox or radio.
 * Separators and submenus always return an empty string. Also
 * not connected menuitems will return a empty string.
 * \return Help of the menuitem.
 **/
{
    if (m_type == penvMI_Command || m_type == penvMI_Checkbox || m_type == penvMI_Radio) {
        if (m_command != NULL) {
            return (m_command->GetHelp());
        }
    }
    return (m_emptystring);
}


//----------------------------------------------------------------
const wxBitmap& MenuItem::GetIcon() const
/**
 * \brief Returns a icon for the menuitem. This method is
 * likely the GetName() method. It will only return a icon,
 * when the type of the menu item is a command, checkbox or radio.
 * Separators and submenus always return an null bitmap. Also
 * not connected menuitems will return a empty string.
 * \return Icon of menu item; or wxNullBitmap.
 **/
{
    if (likely(m_type == penvMI_Command && m_command != NULL)) {
        return (m_command->GetIcon());
    }
    return (wxNullBitmap);
}


//----------------------------------------------------------------
const wxString& MenuItem::GetId() const
/**
 * \brief Returns the CommanId for this MenuItem.
 * \return CommandId.
 **/
{
    return m_id;
}


//----------------------------------------------------------------
MenuItem* MenuItem::GetItem(size_t index)
/**
 * \brief Returns a menu item with the given index.
 * If the index is out of range this method will return
 * null.
 * This method can only be used on menu items of type submenu.
 * \param index Index of the menu item.
 * \return Menu item if found; NULL on error.
 **/
{
    if (unlikely(m_type != penvMI_Submenu)) {
        wxLogWarning(_T("[penv::MenuItem::GetItem] MenuItem is not of type submenu."));
        return (NULL);
    }
    if (unlikely(index < 0 || index >= m_submenu->Count())) {
        wxLogError(_T("[penv::ToolBar::GetItem] Given index \"%i\" is out of range."), index);
        return (NULL);
    }
    return ((*m_submenu)[index]);
}



//----------------------------------------------------------------
bool MenuItem::Add(MenuItem* menuitem)
/**
 * \brief Adds an menu item to the end of the container.
 * To update the changed menubar simply call Update().
 * This method can only be used on menu items of type submenu.
 * \param menuitem Menu item which should be added.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_type != penvMI_Submenu)) {
        wxLogWarning(_T("[penv::MenuItem::Add] MenuItem is not of type submenu."));
        return (false);
    }
    if (unlikely(menuitem == NULL)) {
        wxLogError(_T("[penv::MenuItem::Add] Could not add menu item, because parameter argument is NULL."));
        return (false);
    }
    m_submenu->Add(menuitem);
    return (true);
}



//----------------------------------------------------------------
bool MenuItem::Insert(size_t index, MenuItem* menuitem)
/**
 * \brief Inserst a menu item at the spezified index.
 * To update the changed menubar simply call Update().
 * This method can only be used on menu items of type submenu.
 * \param index Index at which to insert menu item.
 * \param menuitem Menu item to insert.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_type != penvMI_Submenu)) {
        wxLogWarning(_T("[penv::MenuItem::Add] MenuItem is not of type submenu."));
        return (false);
    }
    if (index < 0 || index > m_submenu->Count())
    {
        wxLogError(_T("[penv::MenuItem::Insert] Index is out of range for menu item '%s'."), m_name.c_str());
        return (false);
    }
    return (m_submenu->Insert(index, menuitem));
}



//----------------------------------------------------------------
bool MenuItem::Remove(MenuItem* menuitem)
/**
 * \brief Removes a menu item from the submenu item.
 * Searches the menuitem recursively.
 * To update the changed menubar simply call Update().
 * This method can only be used on menu items of type submenu.
 * \param menuitem The menu item which should be removed.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_type != penvMI_Submenu)) {
        wxLogWarning(_T("[penv::MenuItem::Add] MenuItem is not of type submenu."));
        return (false);
    }
    bool result = false;
    if (m_type == penvMI_Submenu) {
        for (size_t i=0; i<m_submenu->Count(); ++i)
        {
            MenuItem* item = (*m_submenu)[i];
            if (menuitem == item) {
                m_submenu->Remove(menuitem);
                return (true);
            } else {
                if (m_type == penvMI_Submenu) {
                    result = item->Remove(menuitem);
                }
            }
        }
    }
    return (result);
}

//----------------------------------------------------------------
bool MenuItem::Remove(size_t index)
/**
 * \brief Removes a menu item from the submenu at the spezified
 * index. To update the changed menubar simply call Update().
 * This method can only be used on menu items of type submenu.
 * \param index The index of the menu item to remove.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_type != penvMI_Submenu)) {
        wxLogWarning(_T("[penv::MenuItem::Add] MenuItem is not of type submenu."));
        return (false);
    }
    if (index < 0 || index >= m_submenu->Count())
    {
        wxLogError(_T("[penv::MenuItem::Remove] Index is out of range for menu item '%s'."), m_name.c_str());
        return (false);
    }
    return (m_submenu->Remove(index));
}



//----------------------------------------------------------------
size_t MenuItem::Count() const
/**
 * \brief Returns the number of the menu items in the submenu item.
 * This method can only be used on menu items of type submenu.
 * \return Number of toobar items.
 **/
{
    if (unlikely(m_type != penvMI_Submenu)) {
        wxLogWarning(_T("[penv::MenuItem::Add] MenuItem is not of type submenu."));
        return (0);
    }
    return (m_submenu->Count());
}


//----------------------------------------------------------------
wxMenuItem* MenuItem::GetMenuItem()
/**
 * \brief Returns the menuitem in the displaying menubar.
 * This method is used internally, use the returning menuitem
 * only if you know what you are doing.
 * \return Internally used menuitem.
 **/
{
    return (m_menuitem);
}


//----------------------------------------------------------------
bool MenuItem::TryConnect(bool recursive)
/**
 * \brief Tries to connect a menuitem to a command. Or if recursive
 * is spezified and the menu item is a submenu then the underlying
 * menuitems will be connected.
 * \param recursive Connects also submenu items, recursively.
 * \return True on success; false otherwise.
 **/
{
    bool nerror = true;
    Environment* env = Environment::Get();
    if (m_type == penvMI_Command)
    {
        // Wenn das MenuItem ein Befehl ist, dann einfach verbinden
        CommandList* cmdlist = env->GetCommandList();
        Command* cmd = cmdlist->GetCommand(m_id);
        if (unlikely(cmd == NULL)) {
            wxLogError(_T("[MenuItem::TryConnect] Could not connect menuitem to command with id \"%s\", because the command does not exists."), m_id.c_str());
            m_connected = false;
            return (false);
        }
        // TEST: Was ist wenn es mehrere MenuItems gibt die auf das gleiche Command zeigen?
        // TODO SO: Was ist wenn MenüItems gelöscht wurden??
        env->GetFrame()->Connect(m_menuitem->GetId(), wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Command::OnExecute), NULL, cmd);
        m_connected = true;
    }
    else if (m_type == penvMI_Checkbox)
    {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvMI_Radio)
    {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvMI_Submenu && recursive)
    {
        // Wenn das MenuItem ein Submenü ist, dann
        // rekursiv TryConnect() aufrufen
        for (size_t i=0; i<m_submenu->Count(); ++i)
        {
            if (!(*m_submenu)[i]->TryConnect()) nerror = false;
        }
    }
    return (nerror);
}





//----------------------------------------------------------------
bool MenuItem::ReadXml(wxXmlNode* node)
/**
 * \brief Read out a 'menuitem' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'menuitem' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Fehler abfangen
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::MenuItem::ReadXml] Cannot read a NULL Xml Node."));
        return (false);
    }
    if (unlikely(node->GetName() != _T("menuitem"))) {
        wxLogError(_T("[penv::MenuItem::ReadXml] Xml node named \"%s\", but expecting \"menuitem\"."), node->GetName().c_str());
        return (false);
    }
    if (!unlikely(node->HasProp(_T("type")))) {
        wxLogError(_T("[penv::MenuItem::ReadXml] Xml node \"menuitem\" must have a type attribute."));
        return (false);
    }
    // Typ bestimmen und Typ spezifische Informationen auslesen
    wxString type = node->GetPropVal(_T("type"), wxEmptyString);
    if (type == _T("command")) {
        // Typ und ID auslesen
        m_type = penvMI_Command;
        m_id = node->GetNodeContent().Trim(true).Trim(false);
    }
    else if (type == _T("separator")) {
        // Es reicht der Typ
        m_type = penvMI_Separator;
    }
    else if (type == _T("checkbox")) {
        NOT_IMPLEMENTED_YET();
    }
    else if (type == _T("radio")) {
        NOT_IMPLEMENTED_YET();
    }
    else if (type == _T("submenu")) {
        // Name auslesen und die Unterelemente auslesen
        m_type = penvMI_Submenu;
        m_name = node->GetPropVal(_T("name"),_T("none")).Trim(true).Trim(false);
        m_submenu = new Array<MenuItem>();
        wxXmlNode* childnode = node->GetChildren();
        while (childnode != NULL)
        {
            // Muss vom Typ Element Node sein
            if (childnode->GetType() == wxXML_ELEMENT_NODE)
            {
                MenuItem* item = new MenuItem();
                item->ReadXml(childnode);
                m_submenu->Add(item);
            }
            childnode = childnode->GetNext();
        }
    }
    else {
        wxLogError(_T("[penv::MenuItem::ReadXml] Unknown type \"%s\" for \"menuitem\" xml node."),type.c_str());
        return (false);
    }
    // Standard Informationen auslesen
    m_enabled = PenvHelper::ParseBoolean(node->GetPropVal(_T("enabled"),_T("false")));
    m_visible = PenvHelper::ParseBoolean(node->GetPropVal(_T("visible"),_T("false")));
    return (true);
}



//----------------------------------------------------------------
wxXmlNode* MenuItem::WriteXml()
/**
 * \brief Writes a 'menuitem' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'menuitem' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = NULL;
    if (m_type == penvMI_Command)
    {
        node = PenvHelper::CreateXmlNode(_T("menuitem"), m_id);
        node->AddProperty(_T("type"), _T("command"));
        node->AddProperty(_T("enabled"), PenvHelper::CreateBoolean(m_enabled));
        node->AddProperty(_T("visible"), PenvHelper::CreateBoolean(m_visible));
    }
    else if (m_type == penvMI_Submenu)
    {
        node = PenvHelper::CreateXmlNode(_T("menuitem"));
        node->AddProperty(_T("type"), _T("submenu"));
        node->AddProperty(_T("name"), m_name);
        node->AddProperty(_T("enabled"), PenvHelper::CreateBoolean(m_enabled));
        node->AddProperty(_T("visible"), PenvHelper::CreateBoolean(m_visible));
        for (size_t i=0; i<m_submenu->Count(); ++i)
        {
            wxXmlNode* itemnode = (*m_submenu)[i]->WriteXml();
            PenvHelper::AddXmlChildNode(node, itemnode);
        }
    }
    else if (m_type == penvMI_Checkbox)
    {
        node = PenvHelper::CreateXmlNode(_T("menuitem"), m_id);
        node->AddProperty(_T("type"), _T("checkbox"));
        node->AddProperty(_T("enabled"), PenvHelper::CreateBoolean(m_enabled));
        node->AddProperty(_T("visible"), PenvHelper::CreateBoolean(m_visible));
        node->AddProperty(_T("checked"), PenvHelper::CreateBoolean(m_menuitem->IsChecked()));
    }
    else if (m_type == penvMI_Radio)
    {
        node = PenvHelper::CreateXmlNode(_T("menuitem"), m_id);
        node->AddProperty(_T("type"), _T("radio"));
        node->AddProperty(_T("enabled"), PenvHelper::CreateBoolean(m_enabled));
        node->AddProperty(_T("visible"), PenvHelper::CreateBoolean(m_visible));
        node->AddProperty(_T("checked"), PenvHelper::CreateBoolean(m_menuitem->IsChecked()));
    }
    else if (m_type == penvMI_Separator)
    {
        node = PenvHelper::CreateXmlNode(_T("menuitem"));
        node->AddProperty(_T("type"), _T("separator"));
        node->AddProperty(_T("enabled"), PenvHelper::CreateBoolean(m_enabled));
        node->AddProperty(_T("visible"), PenvHelper::CreateBoolean(m_visible));
    }
    return (node);
}




//----------------------------------------------------------------
void MenuItem::AddItems(wxMenu* menu, bool connect)
/**
 * \brief Adds the items into the given menu. If this
 * menuitem is a command, radio, checkbox or separator
 * item then this item will be added. If this item is
 * is a submenu then a submenu item will be created and
 * added to the given menu. This method is only used
 * framework internally.
 * \param menu Menu where to add the items.
 * \param connect If true the items will be tried to
 * connect to a given command.
 **/
{
    Environment* env = Environment::Get();
    if (m_type == penvMI_Command)
    {
        // Command Menu abholen falls es existiert.
        if (unlikely(m_command == NULL)) {
            m_command = env->GetCommandList()->GetCommand(m_id);
            if (unlikely(m_command == NULL)) {
                wxLogError(_T("[penv::MenuItem::AddItem] MenuItem command with ID \"%s\" does not exists in command list."), m_id.c_str());
                return;
            }
        }
        // MenuItem erstellen und dem Menü hinzufügen
        wxMenuItem* item = new wxMenuItem(menu,
            wxID_ANY,
            m_command->GetName(),
            m_command->GetHelp(),
            wxITEM_NORMAL,
            NULL);
        m_menuitem = item;
        if (m_command->GetIcon().IsOk()) {
            item->SetBitmap(m_command->GetIcon());
        }
        menu->Append(item);
        item->Enable(m_command->IsEnabled());
        // Verbinden mit MenuBar falls noch nicht verbunden
        if (unlikely(!m_connected)) {
            // Verbinden mit Command
            if (unlikely(!TryConnect(false))) {
                wxLogError(_T("[penv::MenuItem::AddItem] Could not connect menuitem \"%s\", with menubar event handler."), m_command->GetName().c_str());
            }
        }
    }
    else if (m_type == penvMI_Radio)
    {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvMI_Checkbox)
    {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvMI_Separator)
    {
        // Einfach einen Separator hinzufügen
        menu->AppendSeparator();
    }
    else if (m_type == penvMI_Submenu)
    {
        // Ein SubMenu hinzufügen
        wxMenu* submenu = new wxMenu(_T(""));
        for (size_t i=0; i<m_submenu->Count(); ++i)
        {
            // Rekursiv aufrufen der MenuItems im Submenu
            m_submenu->ItemPtr(i)->AddItems(submenu, connect);
        }
        menu->AppendSubMenu(submenu, m_name);
    }
}


//----------------------------------------------------------------
void MenuItem::FindMenuItems(Array<MenuItem>* array, const wxString& commandid)
/**
 * \brief Searches for menuitems with the given command id and
 * stores them into the array. This method is used only internally.
 * \param array Array to store found items.
 * \param commandid Command id to search for.
 **/
{
    if (m_type == penvMI_Command)
    {
        if (commandid == m_id) array->Add(this);

    }
    else if (m_type == penvMI_Submenu)
    {
        for (size_t i=0; i<m_submenu->Count(); ++i)
        {
            MenuItem* item = (*m_submenu)[i];
            item->FindMenuItems(array, commandid);
        }
    }
    else if (m_type == penvMI_Checkbox)
    {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvMI_Radio)
    {
        NOT_IMPLEMENTED_YET();
    }
}



} // namespace penv
