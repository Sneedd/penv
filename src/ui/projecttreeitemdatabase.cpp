/*
 * projecttreeitemdatabase.cpp - Implementation of the ProjectTreeItemDataBase class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04/23/2008
 *
 */


#include "projecttreeitemdatabase.h"
#include "projecttreeitemdata.h"
#include "projecttreeitemdataobject.h"

namespace penv {


//----------------------------------------------------------------
ProjectTreeItemDataBase::ProjectTreeItemDataBase()
/**
 * \brief Constructor, initializes a empty ProjectTreeItemData object.
 **/
{
    m_project  = NULL;
    m_workspace  = NULL;
    m_item  = NULL;
    m_type = penvTI_ProjectItem;
}



//----------------------------------------------------------------
ProjectTreeItemDataBase::ProjectTreeItemDataBase(Project* project)
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a project.
 * \param project Project attached to this object.
 **/
{
    m_project  = project;
    m_workspace  = NULL;
    m_item  = NULL;
    m_type = penvTI_Project;
}



//----------------------------------------------------------------
ProjectTreeItemDataBase::ProjectTreeItemDataBase(Workspace* workspace)
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a workspace.
 * \param project Workspace attached to this object.
 **/
{
    m_project  = NULL;
    m_workspace  = workspace;
    m_item  = NULL;
    m_type = penvTI_Workspace;
}



//----------------------------------------------------------------
ProjectTreeItemDataBase::ProjectTreeItemDataBase(ProjectItem* item)
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a project item.
 * \param project Project item attached to this object.
 **/
{
    m_project  = NULL;
    m_workspace  = NULL;
    m_item  = item;
    m_type = penvTI_ProjectItem;
}



//----------------------------------------------------------------
ProjectTreeItemDataBase::~ProjectTreeItemDataBase()
/**
 * \brief Destructor.
 **/
{
    // Nothing to do.
}


//----------------------------------------------------------------
Project* ProjectTreeItemDataBase::GetProject()
/**
 * \brief Returns the project if this data object is a project,
 * otherwise this method will return NULL.
 * \return The project attached to this data object or NULL.
 **/
{
    return (m_project);
}

//----------------------------------------------------------------
void ProjectTreeItemDataBase::SetProject(Project* project)
/**
 * \brief Sets this data object as a project and reference to it.
 * \param project A project for this data object.
 **/
{
    m_project = project;
    m_type = penvTI_Project;
}



//----------------------------------------------------------------
Workspace* ProjectTreeItemDataBase::GetWorkspace()
/**
 * \brief Returns the workspace if this data object is a workspace,
 * otherwise this method will return NULL.
 * \return The workspace attached to this data object or NULL.
 **/
{
    return (m_workspace);
}



//----------------------------------------------------------------
void ProjectTreeItemDataBase::SetWorkspace(Workspace* workspace)
/**
 * \brief Sets this data object as a workspace and reference to it.
 * \param workspace A workspace for this data object.
 **/
{
    m_workspace = workspace;
    m_type = penvTI_Workspace;
}



//----------------------------------------------------------------
ProjectItem* ProjectTreeItemDataBase::GetProjectItem()
/**
 * \brief Returns the project item if this data object is a project item,
 * otherwise this method will return NULL.
 * \return The project item attached to this data object or NULL.
 **/
{
    return (m_item);
}



//----------------------------------------------------------------
void ProjectTreeItemDataBase::SetProjectItem(ProjectItem* item)
/**
 * \brief Sets this data object as a project item and reference to it.
 * \param item A project item for this data object.
 **/
{
    m_item = item;
    m_type = penvTI_ProjectItem;
}



//----------------------------------------------------------------
ProjectTreeItemType ProjectTreeItemDataBase::GetType() const
/**
 * \brief Returns the type attached to this data object.
 * \return Type of data attached to this data object.
 **/
{
    return (m_type);
}


//----------------------------------------------------------------
PenvItem* ProjectTreeItemDataBase::GetPenvItem()
/**
 * \brief Returns the item as PenvItem object.
 * \return Item as PenvItem object.
 **/
{
    if (m_type == penvTI_Project)
        return (m_project);
    else if (m_type == penvTI_Workspace)
        return (m_workspace);
    else if (m_type == penvTI_ProjectItem)
        return (m_item);
}



//----------------------------------------------------------------
ProjectTreeItemDataBase* ProjectTreeItemDataBase::Clone() const
/**
 * \brief Clones this object into the returning object.
 * Only the pointer for the spezial type will be cloned,
 * all others are initialized with NULL.
 * \return Cloned object of this class.
 **/
{
    if (m_type == penvTI_Project) {
        return (new ProjectTreeItemDataBase(m_project));
    }
    else if (m_type == penvTI_ProjectItem) {
        return (new ProjectTreeItemDataBase(m_item));
    }
    else if (m_type == penvTI_Workspace) {
        return (new ProjectTreeItemDataBase(m_workspace));
    }
    return (NULL);
}


//----------------------------------------------------------------
void ProjectTreeItemDataBase::LogClassInfo()
/**
 * \brief Logs information in the class.
 **/
{
    wxString typestring = _T("item");
    if (m_type == penvTI_Project) typestring = _T("project");
    else if (m_type == penvTI_Workspace) typestring = _T("workspace");
    wxLogMessage(_T("[penv::ProjectTreeItemData::LogClassInfo] "
        "this = 0x%p; type = %i / %s; project* = %p; workspace* = %p; item* = %p"),
        this, (int)m_type, typestring.c_str(), m_project, m_workspace, m_item);
    if (m_type == penvTI_Project) {
        if (m_project == NULL) {
            wxLogMessage(_T("[penv::ProjectTreeItemData::LogClassInfo] "
                "Project is NULL."));
        } else {
            wxLogMessage(_T("[penv::ProjectTreeItemData::LogClassInfo] "
                "Project Name: %s"), m_project->GetName().c_str());
        }
    }
    else if (m_type == penvTI_ProjectItem) {
        if (m_item == NULL) {
            wxLogMessage(_T("[penv::ProjectTreeItemData::LogClassInfo] "
                "Project Item is NULL."));
        } else {
            wxString itemtype = _T("file");
            if (m_item->GetType() == penvPI_Directory) itemtype = _T("directory");
            else if (m_item->GetType() == penvPI_LinkedItems) itemtype = _T("linked items");
            else if (m_item->GetType() == penvPI_SubProject) itemtype = _T("subproject");
            wxLogMessage(_T("[penv::ProjectTreeItemData::LogClassInfo] "
                "Project Item Name: %s; Type: %s"), m_item->GetName().c_str(), itemtype.c_str());
        }
    }
    else if (m_type == penvTI_Workspace) {
        if (m_workspace == NULL) {
            wxLogMessage(_T("[penv::ProjectTreeItemData::LogClassInfo] "
                "Workspace is NULL."));
        } else {
            wxLogMessage(_T("[penv::ProjectTreeItemData::LogClassInfo] "
                "Workspace Name: %s"), m_workspace->GetName().c_str());
        }
    }
}



} // namepsace penv
