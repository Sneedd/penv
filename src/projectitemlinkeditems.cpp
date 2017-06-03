/*
 * projectitemlinkeditems.cpp - Implementation of the ProjectItemLinkedItems class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */

#include "projectitemlinkeditems.h"
#include "expect.h"
#include "penvhelper.h"
#include "projectitemlist.h"


namespace penv {



//----------------------------------------------------------------
ProjectItemLinkedItems::ProjectItemLinkedItems()
    : ProjectItem(NULL)
/**
 * \brief Constructor (private).
 **/
{
    wxLogError(_T("oooooooooh"));
    wxASSERT(false);
}


//----------------------------------------------------------------
ProjectItemLinkedItems::ProjectItemLinkedItems(ProjectItemList* parent)
    : ProjectItem(parent)
/**
 * \brief Constructor, initializes the ProjectItemDirectory object.
 * \param parent The parent ProjectItemList who contains this item.
 **/
{
    m_innerlist = new ProjectItemList(this);
    m_type = penvPI_LinkedItems;
    m_penvitemtype = penvIT_ProjectItemLinkedItems;
}

//----------------------------------------------------------------
ProjectItemLinkedItems::~ProjectItemLinkedItems()
/**
 * \brief Destructor.
 **/
{
    if (m_innerlist != NULL) delete m_innerlist;
}



//----------------------------------------------------------------
ProjectItemList* ProjectItemLinkedItems::GetItemList()
/**
 * \brief Returns the project items in this item.
 * \return Project items in this item.
 **/
{
    return (m_innerlist);
}


//----------------------------------------------------------------
ProjectItem* ProjectItemLinkedItems::GetMainItem()
/**
 * \brief Returns the main item of this liked items.
 * The main item is included in the liked items, but
 * is also the only one loaded.
 * \return Returns the main item.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (NULL);
}



//----------------------------------------------------------------
bool ProjectItemLinkedItems::Save()
/**
 * \brief Saves this project item. Because this item can store
 * more than one item the main item will be saved, but this could
 * also save the other items. The functionality is implemented
 * in the windows save method so there could be various behaivors.
 * \return True on succcess; false otherwise.
 **/
{
    ProjectItem* item = GetMainItem();
    if (item == NULL) {
        wxLogError(_T("[penv::ProjectItemLinkedItems::Save] There is no main item in the LinkedItems ProjectItem."));
        return (false);
    }
    return (item->Save());
}



//----------------------------------------------------------------
bool ProjectItemLinkedItems::Load()
/**
 * \brief Loads this project item. Because this item can store
 * more than one item the main item will be loaded, but this could
 * also load the other items. The functionality is implemented
 * in the windows load method so there could be various behaivors.
 * \return True on succcess; false otherwise.
 **/
{
    ProjectItem* item = GetMainItem();
    if (item == NULL) {
        wxLogError(_T("[penv::ProjectItemLinkedItems::Load] There is no main item in the LinkedItems ProjectItem."));
        return (false);
    }
    return (item->Load());
}



//----------------------------------------------------------------
ProjectItem* ProjectItemLinkedItems::Clone()
/**
 * \brief Clones this project item and all its underlying
 * project items. The parent will not be cloned, instead
 * this will always be null.
 * \return Return the cloned project item.
 **/
{
    ProjectItemLinkedItems* item = new ProjectItemLinkedItems(NULL);
    item->m_name = m_name;
    item->m_type = m_type;
    item->m_windowid = wxEmptyString;
    item->m_windowtype = m_windowtype;
    item->m_virtual = m_virtual;
    item->m_properties = m_properties->Clone();
    item->m_parent = NULL;
    item->m_innerlist = m_innerlist->Clone();
    item->m_innerlist->SetProjectItemParent(item);
    return (item);
}


//----------------------------------------------------------------
bool ProjectItemLinkedItems::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'projectitem' xml element node.
 * See class description for more information.
 * \param node The 'projectitem' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Knoten muss vom Typ Element sein
    if (node == NULL)
    {
        wxLogError(_T("[penv::ProjectItemLinkedItems::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("projectitem"))
    {
        wxLogError(_T("[penv::ProjectItemLinkedItems::ReadNode] Node should be an element node with name 'projectitem'."));
        return (false);
    }
    wxString type = node->GetPropVal(_T("type"), wxEmptyString);
    if (type != _T("linkeditem"))
    {
        wxLogError(_T("[penv::ProjectItemLinkedItems::ReadNode] ProjectItem must be ProjectItemLinkedItems."));
        return (false);
    }
    m_name = node->GetPropVal(_T("name"),wxEmptyString);
    if(m_name.IsEmpty())
    {
       wxLogWarning(_T("[penv::ProjectItemLinkedItems::ReadNode] ProjectItem is nameless"));
    }
    m_virtual = PenvHelper::ParseBoolean(node->GetPropVal(_T("virtual"), _T("false")));
    // Durch alle knoten unterhalb von 'projectitems' loopen
    wxXmlNode* innernode = node->GetChildren();
    while (innernode != NULL)
    {
        if (innernode->GetType() == wxXML_ELEMENT_NODE)
        {
            if (innernode->GetName() == _T("mainitem"))
            {
                m_name = innernode->GetNodeContent();
            }
            else if (innernode->GetName() == _T("projectitem"))
            {
                wxString type = innernode->GetPropVal(_T("type"),wxEmptyString);
                if(type.IsEmpty())
                {
                    wxLogError(_T("[penv::ProjectItemLinkedItems::ReadNode] Type attribut is missing."));
                    return(false);
                }
                ProjectItem* item = m_innerlist->CreateProjectItem(type);
                if (item == NULL)
                {
                    wxLogError(_T("[penv::ProjectItemLinkedItems::ReadNode] ProjectItemType '%s' undefined."), type.c_str());
                    return(false);
                }
                if (!item->ReadNode(innernode))
                {
                    wxLogWarning(_T("[penv::ProjectItemLinkedItems::ReadNode] ProjectItem failed to read."), type.c_str());
                    innernode = innernode->GetNext();
                    continue;
                }
                m_innerlist->Add(item);
            }
            else if (innernode->GetName() == _T("properties"))
            {
                if (!m_properties->ReadNode(innernode))
                {
                    wxLogError(_T("[penv::ProjectItemLinkedItems::ReadNode] Cannot read properties in ProjectItemFile."));
                }
            }
        }
        innernode = innernode->GetNext();
    }
    return (true);
}



//----------------------------------------------------------------
wxXmlNode* ProjectItemLinkedItems::WriteNode()
/**
 * \brief Writes a 'projectitem' xml element node.
 * See class description for more information.
 * \return The 'projectitem' xml element node.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("projectitem"));
    node->AddProperty(_T("name"), m_name);
    node->AddProperty(_T("type"), _T("linkeditem"));
    node->AddProperty(_T("virtual"), PenvHelper::CreateBoolean(m_virtual));

    wxXmlNode* mainitemnode = PenvHelper::CreateXmlNode(_T("mainitem"), m_name);
    PenvHelper::AddXmlChildNode(node, mainitemnode);
    for(size_t i = 0; m_innerlist->Count(); i++)
    {
        ProjectItem* item = m_innerlist->GetItem(i);
        wxXmlNode* itemnode = item->WriteNode();
        PenvHelper::AddXmlChildNode(node, itemnode);
    }
    wxXmlNode* propnode = m_properties->WriteNode();
    PenvHelper::AddXmlChildNode(node, propnode);

    return (node);
}





} // namespace penv
