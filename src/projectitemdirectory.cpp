/*
 * projectitemdirectory.cpp - Implementation of the ProjectItem class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#include "projectitemdirectory.h"
#include "expect.h"
#include "penvhelper.h"
#include "projectitemlist.h"
#include "project.h"

namespace penv {

//----------------------------------------------------------------
ProjectItemDirectory::ProjectItemDirectory()
    : ProjectItem(NULL)
/**
 * \brief Constructor (private).
 **/
{
    wxLogError(_T("iiiiiiiiiiaaaaaaaaaaaaaaaaargh"));
    wxASSERT(false);
}


//----------------------------------------------------------------
ProjectItemDirectory::ProjectItemDirectory(ProjectItemList* parent)
    : ProjectItem(parent)
/**
 * \brief Constructor, initializes the ProjectItemDirectory object.
 * \param parent The parent ProjectItemList who contains this item.
 **/
{
    m_innerlist = new ProjectItemList(this);
    m_type = penvPI_Directory;
    m_penvitemtype = penvIT_ProjectItemDirectory;
}


//----------------------------------------------------------------
ProjectItemDirectory::~ProjectItemDirectory()
/**
 * \brief Destructor.
 **/
{
    if (m_innerlist != NULL) delete m_innerlist;
}


//----------------------------------------------------------------
wxString ProjectItemDirectory::GetPathString() const
/**
 * \brief Returns the path of this item as an string.
 * \return Full path to item as wxString.
 **/
{
    return (m_directory.GetFullPath());
}


//----------------------------------------------------------------
wxFileName ProjectItemDirectory::GetPath() const
/**
 * \brief Returns the path of this item.
 * \return Full path to item as wxFileName.
 **/
{
    return (m_directory);
}


//----------------------------------------------------------------
ProjectItemList* ProjectItemDirectory::GetItemList()
/**
 * \brief Returns the project items in this directory item.
 * \return Project items in this directory.
 **/
{
    return (m_innerlist);
}



//----------------------------------------------------------------
bool ProjectItemDirectory::Save()
/**
 * \brief Saves this project item. Because this is a directory,
 * a save action will only check if the directory exists. If the
 * directory does not exist, then it will be created.
 * \return True on succcess; false otherwise.
 **/
{
    if (m_virtual) return (true);
    wxString path = m_directory.GetPath();
    if (unlikely(!wxDir::Exists(path))) {
        if (!Path::Create(path)) {
            wxLogError(_T("[penv::ProjectItemDirectory::Save] Could not create directory \"%s\"."), path.c_str());
            return (false);
        }
    }
    return (true);
}



//----------------------------------------------------------------
bool ProjectItemDirectory::Load()
/**
 * \brief Loads this project item. Because this is a directory,
 * there will be nothing to load and therefore this method
 * does nothing and ever returns true.
 * \return True.
 **/
{
    return (true);
}

//----------------------------------------------------------------
ProjectItem* ProjectItemDirectory::Clone()
/**
 * \brief Clones this project item and all its underlying
 * project items. The parent will not be cloned, instead
 * this will always be null.
 * \return Return the cloned project item.
 **/
{
    ProjectItemDirectory* item = new ProjectItemDirectory(NULL);
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
bool ProjectItemDirectory::ReadNode(wxXmlNode* node)
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
        wxLogError(_T("[penv::ProjectItemDirectory::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("projectitem"))
    {
        wxLogError(_T("[penv::ProjectItemDirectory::ReadNode] Node should be an element node with name 'projectitem'."));
        return (false);
    }
    wxString type = node->GetPropVal(_T("type"), wxEmptyString);
    if (type != _T("directory"))
    {
        wxLogError(_T("[penv::ProjectItemDirectory::ReadNode] ProjectItem must be ProjectItemDirectory."));
        return (false);
    }
    m_name = node->GetPropVal(_T("name"),wxEmptyString);
    if(m_name.IsEmpty())
    {
       wxLogWarning(_T("[penv::ProjectItemDirectory::ReadNode] ProjectItemDirectory is nameless"));
    }
    m_virtual = PenvHelper::ParseBoolean(node->GetPropVal(_T("virtual"), _T("false")));

    // Durch alle knoten unterhalb von 'projectitems' loopen
    wxXmlNode* innernode = node->GetChildren();
    while (innernode != NULL)
    {
        if (innernode->GetType() == wxXML_ELEMENT_NODE)
        {
            if (innernode->GetName() == _T("path"))
            {
                wxString path = innernode->GetNodeContent();
                if (m_parent->GetProjectParent() != NULL) {
                    path = Path::Combine(Path::RemoveLastEntry(m_parent->GetProjectParent()->GetFileNameString()), path);
                } else {
                    path = Path::Combine(m_parent->GetProjectItemParent()->GetPathString(), path);
                }
                if(!wxDir::Exists(path) && !m_virtual)
                {
                    wxLogWarning(_T("[penv::ProjectItemDirectory::ReadNode] Directory '%s' does not exist."), path.c_str());
                    innernode = innernode->GetNext();
                    continue;
                }
                m_directory.SetPath(path);
            }
            else if (innernode->GetName() == _T("projectitem"))
            {
                wxString type = innernode->GetPropVal(_T("type"),wxEmptyString);
                if(type.IsEmpty())
                {
                    wxLogError(_T("[penv::ProjectItemDirectory::ReadNode] Type attribut is missing."));
                    return(false);
                }
                ProjectItem* item = m_innerlist->CreateProjectItem(type);
                if (item == NULL)
                {
                    wxLogError(_T("[penv::ProjectItemDirectory::ReadNode] ProjectItemType '%s' undefined."), type.c_str());
                    return(false);
                }
                if (!item->ReadNode(innernode))
                {
                    wxLogWarning(_T("[penv::ProjectItemDirectory::ReadNode] ProjectItem failed to read."), type.c_str());
                    innernode = innernode->GetNext();
                    continue;
                }
                m_innerlist->Add(item);
            }
            else if (innernode->GetName() == _T("properties"))
            {
                if (!m_properties->ReadNode(innernode))
                {
                    wxLogError(_T("[penv::ProjectItemDirectory::ReadNode] Cannot read properties in ProjectItemDirectory."));
                }
            }
        }
        innernode = innernode->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* ProjectItemDirectory::WriteNode()
/**
 * \brief Writes a 'projectitem' xml element node.
 * See class description for more information.
 * \return The 'projectitem' xml element node.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("projectitem"));
    node->AddProperty(_T("name"), m_name);
    node->AddProperty(_T("type"), _T("directory"));
    node->AddProperty(_T("virtual"), PenvHelper::CreateBoolean(m_virtual));
    wxString parentpath;
    if (m_parent->GetProjectParent() == NULL) {
        ProjectItem* item = m_parent->GetProjectItemParent();
        parentpath = item->GetPathString();
    } else {
        Project* project = m_parent->GetProjectParent();
        parentpath = project->GetFileNameString();
    }
    wxXmlNode* path = PenvHelper::CreateXmlNode(_T("path"),
        Path::MakeRelative(parentpath, m_directory.GetPath()));
    PenvHelper::AddXmlChildNode(node, path);
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

