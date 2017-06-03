/*
 * menubar.cpp - Implementation of the MenuBar class
 *
 * Author: Steffen Ott
 * Date: 09/20/2007
 *
 */

#include "menubar.h"
#include "penvhelper.h"
#include "environment.h"


namespace penv {


//----------------------------------------------------------------
MenuBar::MenuBar()
/**
 * \brief Constructor (private).
 **/
{
    wxASSERT_MSG(false, _T("Not allowed to be called."));
}


//----------------------------------------------------------------
MenuBar::MenuBar(wxMenuBar* menubar)
/**
 * \brief Constructor, initializes the menubar object.
 * \param menubar The internally used menubar.
 **/
{
    m_array = new Array<MenuItem>();
    m_menubar = menubar;
}


//----------------------------------------------------------------
MenuBar::~MenuBar()
/**
 * \brief Destructor.
 **/
{
    if (m_array != NULL) delete m_array;
}


//----------------------------------------------------------------
wxMenuBar* MenuBar::GetMenuBar()
/**
 * \brief Returns the wxWigets internal menubar.
 * \return Internal menubar.
 **/
{
    return (m_menubar);
}

//----------------------------------------------------------------
MenuItem* MenuBar::GetItem(size_t index)
/**
 * \brief Returns a menubar item with the given index.
 * If the menubar is out of range this method will return
 * null.
 * \param index Index of the menubar item.
 * \return Menubar item if found; NULL on error.
 **/
{
    if (unlikely(index < 0 || index >= m_array->Count())) {
        wxLogError(_T("[penv::MenuBar::GetItem] Given index \"%i\" is out of range."), index);
        return (NULL);
    }
    return ((*m_array)[index]);
}


//----------------------------------------------------------------
bool MenuBar::Add(MenuItem* item)
/**
 * \brief Adds an menubar item to the end of the container.
 * The inserted menubar item musst be a submenuitem.
 * To update the changed menubar simply call Update().
 * \param item Toolbar item which should be added.
 * \return True on success; false otherwise.
 **/
{
    if (item == NULL) {
        wxLogError(_T("[penv::MenuBar::Add] Argument 'item' is null."));
        return (false);
    }
    if (item->GetType() != penvMI_Submenu) {
        wxLogError(_T("[penv::MenuBar::Add] Menu item in the first level must be a submenu."));
        return (false);
    }
    if (unlikely(item == NULL)) {
        wxLogError(_T("[penv::MenuBar::Add] Could not add menubar item, because parameter argument is NULL."));
        return (false);
    }
    m_array->Add(item);
    return (true);
}



//----------------------------------------------------------------
bool MenuBar::Insert(size_t index, MenuItem* item)
/**
 * \brief Inserts a menubar item at the spezified index.
 * The inserted menubar item musst be a submenuitem.
 * To update the changed menubar simply call Update().
 * \param index Index at which to insert menubar item.
 * \param item Menubar item to insert.
 * \return True on success; false otherwise.
 **/
{
    if (item == NULL) {
        wxLogError(_T("[penv::MenuBar::Insert] Argument 'item' is null."));
        return (false);
    }
    if (item->GetType() != penvMI_Submenu) {
        wxLogError(_T("[penv::MenuBar::Insert] Menu item in the first level must be a submenu."));
        return (false);
    }
    if (index < 0 || index > m_array->Count()) {
        wxLogError(_T("[penv::MenuBar::Insert] Index is out of range for menubar."));
        return (false);
    }
    return (m_array->Insert(index, item));
}


//----------------------------------------------------------------
bool MenuBar::Remove(MenuItem* item)
/**
 * \brief Removes a menubar item from the menubar.
 * Searches the menuitem recursively.
 * To update the changed menubar simply call Update().
 * \param item The menubar item which should be removed.
 * \return True on success; false otherwise.
 **/
{
    bool result = false;
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        MenuItem* menuitem = (*m_array)[i];
        if (menuitem == item) {
            m_array->Remove(item);
            return (true);
        } else {
            result = menuitem->Remove(item);
        }
    }
    if (!result)
    {
        wxLogError(_T("[penv::MenuBar::Remove] Cannot remove the menubar item, because it does not exist in menu item."));
    }
    return (result);
}


//----------------------------------------------------------------
bool MenuBar::Remove(size_t index)
/**
 * \brief Removes a menubar item from the menubars first level at
 * the spezified index. To update the changed menubar simply call Update().
 * \param index The index of the menubar item to remove.
 * \return True on success; false otherwise.
 **/
{
    if (index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::MenuBar::Remove] Index is out of range for menubar."));
        return (false);
    }
    return (m_array->Remove(index));
}



//----------------------------------------------------------------
void MenuBar::Clear()
/**
 * \brief Removes all menubar items from the menubar.
 * To update the changed menubar simply call Update().
 **/
{
    m_array->Clear();
}



//----------------------------------------------------------------
size_t MenuBar::Count()
/**
 * \brief Returns the number of the menubar items in the first
 * level of the menubar.
 * \return Number of menubar items.
 **/
{
    return (m_array->Count());
}



//----------------------------------------------------------------
Array<MenuItem>* MenuBar::FindMenuItems(const wxString& commandid)
/**
 * \brief Searches the menubar for items who are connected with the
 * given commandid. The returned array must be destroyed by the caller
 * of this method.
 * \param commandid Command Id to search for.
 * \return Array with menubar items.
 **/
{
    Array<MenuItem>* array = new Array<MenuItem>(false);
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        // First level only contains submenus so there
        //  will be no menuitem to add!
        MenuItem* item = (*m_array)[i];
        item->FindMenuItems(array, commandid);
    }
    return (array);
}


//----------------------------------------------------------------
bool MenuBar::Update()
/**
 * \brief Updates the internally used menubar with the configuration
 * in this menubar.
 * \return True on success; false otherwise.
 **/
{
    if (m_menubar == NULL) {
        wxLogError(_T("[penv::MenuBar::Update] Cannot update menubar, because the menubar is not initialized."));
        return (false);
    }
    // Alte MenuBar löschen
    for (size_t i=0, count=m_menubar->GetMenuCount(); i<count; ++i)
    {
        // TODO SO : Events löschen
        wxMenu* menu = m_menubar->Remove(0);
        if (menu != NULL) delete menu;
    }
    // Neue Elemente hinzufügen
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        // Überprüfen ob das MenuItem wirklich vom Typ submenu ist
        if (unlikely(m_array->ItemPtr(i)->GetType() != penvMI_Submenu)) {
            wxLogError(_T("[penv::MenuBar::Update] Menu item \"%s\" must be a submenu, because it is in the first level."), m_array->ItemPtr(i)->GetName().c_str());
            return (false);
        }
        // Neues Menu Element hinzufügen und ...
        wxString name = m_array->ItemPtr(i)->GetName();
        wxMenu* menu = new wxMenu(_T(""));
        m_menubar->Append(menu, name);
        // ... rekursiv MenuItems aufrufen da dieses MenuItem immer
        // ein Submenu sein muss, alle Unterelemente hinzufügen
        MenuItem* submenu = m_array->ItemPtr(i);
        for (size_t j=0; j<submenu->Count(); ++j)
        {
            submenu->GetItem(j)->AddItems(menu, true);
        }
    }

    // Die MenuBar Änderungen Updaten
    m_menubar->Refresh();
    return (true);
}



//----------------------------------------------------------------
bool MenuBar::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'menubar' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'menubar' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Fehler abfangen
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::MenuBar::ReadNode] Xml node is NULL."));
        return (false);
    }
    if (unlikely(node->GetType() != wxXML_ELEMENT_NODE)) {
        wxLogError(_T("[penv::MenuBar::ReadNode] Xml node is not an element node."));
        return (false);
    }
    if (unlikely(node->GetName() != _T("menubar"))) {
        wxLogError(_T("[penv::MenuBar::ReadNode] Xml node is not an \"menubar\" node."));
        return (false);
    }

    // Beginnen mit dem Auslesen
    wxXmlNode* child = node->GetChildren();
    while (child != NULL)
    {
        // Nicht Element Nodes überspringen und nicht menuitem Elemente überspringen
        if (likely(child->GetType() == wxXML_ELEMENT_NODE && child->GetName() == _T("menuitem"))) {
            // Nur submenu MenuItems im ersten Level der MenuBar erlauben!
            if (unlikely(child->GetPropVal(_T("type"), _T("none")) != _T("submenu"))) {
                wxLogError(_T("[penv::MenuBar::ReadNode] Only submenu entries allowed in the first menubar level."));
                child = child->GetNext();
                continue;
            }
            MenuItem* item = new MenuItem();
            item->ReadXml(child);
            m_array->Add(item);
        }
        // Nächstes Child Node
        child = child->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* MenuBar::WriteNode()
/**
 * \brief Writes a 'menubar' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'menubar' xml element node; or NULL on error.
 **/
{
    wxXmlNode* menubarnode = PenvHelper::CreateXmlNode(_T("menubar"));
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        wxXmlNode* itemnode = (*m_array)[i]->WriteXml();
        if (itemnode == NULL) {
            wxLogError(_T("[penv::MenuBar::WriteNode] Error writing 'menuitem' node, skipping this node."));
            continue;
        }
        PenvHelper::AddXmlChildNode(menubarnode, itemnode);
    }
    return (menubarnode);
}




} // namespace penv


