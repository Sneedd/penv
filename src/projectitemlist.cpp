/*
 * projectitemlist.cpp - Implementation of the ProjectItemList class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 11/04/2007
 *
 *
 */


#include "projectitemlist.h"
#include "expect.h"
#include "penvhelper.h"
#include "project.h"
#include "projectitem.h"
#include "projectitemfile.h"
#include "projectitemdirectory.h"
#include "projectitemlinkeditems.h"
#include "projectitemsubproject.h"
#include "environment.h"


namespace penv {


//----------------------------------------------------------------
ProjectItemList::ProjectItemList()
/**
 * \brief Constructor (private).
 **/
{
    wxLogMessage(_T("fadsljkjlkfdfsajlkdsafljkasdfjafsdjkl"));
}

//----------------------------------------------------------------
ProjectItemList::ProjectItemList(Project* parent)
/**
 * \brief Constructor, initializes the ProjectItemList object.
 * \param parent The parent project of this container.
 **/
{
    m_projectparent = parent;
    m_projectitemparent = NULL;
    m_array = new Array<ProjectItem>();
}



//----------------------------------------------------------------
ProjectItemList::ProjectItemList(ProjectItem* parent)
/**
 * \brief Constructor, initializes the ProjectItemList object.
 * \param parent The parent projectitem of this container.
 **/
{
    m_projectitemparent = parent;
    m_projectparent = NULL;
    m_array = new Array<ProjectItem>();
}



//----------------------------------------------------------------
ProjectItemList::~ProjectItemList()
/**
 * \brief Destructor.
 **/
{
    if(m_array != NULL) delete m_array;
}


//----------------------------------------------------------------
ProjectItem* ProjectItemList::GetItem(size_t index)
/**
 * \brief Returns the project item at the specified index.
 * \param index Index of the project item.
 * \return Project item on success; false otherwise.
 **/
{
    if (index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectItemList::Item] Index is out of range."));
        return (NULL);
    }
    return ((*m_array)[index]);
}

//----------------------------------------------------------------
int ProjectItemList::GetItemIndex(ProjectItem* item)
/**
 * \brief Returns the index of the given project item.
 * Can return -1 if the project item is not found.
 * \param item The ProjectItem to search the index for.
 * \return Index of the ProjectItem on success; -1 if not found.
 **/
{
    int index = -1;

    for(size_t i = 0; i < m_array->Count(); i++)
    {
        if (item == m_array->ItemPtr(i)) {
            index = i;
            break;
        }
    }
    return index;
}

//----------------------------------------------------------------
bool ProjectItemList::Add(ProjectItem* prjitem)
/**
 * \brief Adds a project item at the end of this container.
 * \param prjitem The project item which should be added.
 * \return True on success; false otherwise.
 **/
{
    if(prjitem == NULL)
    {
        wxLogError(_T("[penv::ProjectItemList::Add] Argument 'prjitem' must be not NULL."));
        return(false);
    }
    m_array->Add(prjitem);
    Modified(true);
    return(true);
}


//----------------------------------------------------------------
bool ProjectItemList::Remove(size_t index)
/**
 * \brief Removes a project item from this container.
 * \param index Index of the project item which should be removed.
 * \return True on success; false otherwise.
 **/
{
    if(index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectItemList::Remove] index out of range."));
        return(false);
    }
    m_array->Remove(index);
    Modified(true);
    return (true);
}


//----------------------------------------------------------------
void ProjectItemList::Clear()
/**
 * \brief Removes all project items from this container. This
 * method does not set the parent project to modified.
 **/
{
    m_array->Clear();
}


//----------------------------------------------------------------
size_t ProjectItemList::Count()
/**
 * \brief Returns the number of project items in this container.
 * \return Number of project items in this container.
 **/
{
    return(m_array->Count());
}


//----------------------------------------------------------------
Project* ProjectItemList::GetProjectParent()
/**
 * \brief Returns the parent project of this container. Can return
 * NULL if this container is in a ProjectItemDirectory or
 * ProjectItemLinkedItems.
 * \return Parent project of this container or NULL.
 **/
{
    return(m_projectparent);
}

//----------------------------------------------------------------
void ProjectItemList::SetProjectParent(Project* project)
/**
 * \brief Sets the parent project of this container.
 * \param project Parent project of this container.
 **/
{
    m_projectparent = project;
    m_projectitemparent = NULL;
}

//----------------------------------------------------------------
ProjectItem* ProjectItemList::GetProjectItemParent()
/**
 * \brief Returns the parent project item of this container. Can return
 * NULL if this container is in a Project.
 * \return Parent project item of this container or NULL.
 **/
{
    return(m_projectitemparent);
}

//----------------------------------------------------------------
void ProjectItemList::SetProjectItemParent(ProjectItem* projectitem)
/**
 * \brief Sets the parent project item of this container.
 * \param project Parent project item of this container.
 **/
{
    m_projectparent = NULL;
    m_projectitemparent = projectitem;
}


//----------------------------------------------------------------
bool ProjectItemList::MoveProjectItem(size_t index, Project* dest)
/**
 * \brief Moves a project item from this container to another project.
 * This method fails if the project item is moved to the same project
 * or the index is out of range. A 'PENV_EVT_PROJECTITEM_MOVED' event
 * will be generated after the project item is successful moved.
 * \param index Index of the project item which should be moved.
 * \param dest Destination project.
 * \return True on success; false otherwise.
 **/
{

    if (index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectItemList::MoveProjectItem] Index is out of range."));
        return (false);
    }
    if(dest == m_projectparent){
        wxLogWarning(_T("[penv::ProjectItemList::MoveProjectItem] Destination and source workspace are the same."));
        return (true);
    }
    ProjectItem* item = (*m_array)[index];
    item->SetParent(dest->GetProjectItemList());
    dest->GetProjectItemList()->m_array->Add(item);
    dest->Modified();
    if (m_projectparent != NULL) {
        m_projectparent->Modified();
    } else {
        if(m_projectitemparent->GetType() == penvPI_SubProject)
        {
            ((ProjectItemSubProject*)m_projectitemparent)->Modified();
        }
    }
    m_array->Remove(index, false);
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_PROJECTITEM_MOVED"));
    return(true);
}


//----------------------------------------------------------------
bool ProjectItemList::CopyProjectItem(size_t index, Project* dest)
/**
 * \brief Copies a project item from this container to another project.
 * This method fails if the index is out of range.
 * A 'PENV_EVT_PROJECTITEM_COPIED' event will be generated after the
 * project item is successful copied.
 * \param index Index of the project item which should be moved.
 * \param dest Destination project.
 * \return True on success; false otherwise.
 **/
{
    if(index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectItemList::CopyProjectItem] Index out of range."));
        return (false);
    }
    ProjectItem* item = (*m_array)[index]->Clone();
    item->SetParent(dest->GetProjectItemList());
    dest->GetProjectItemList()->m_array->Add(item);
    dest->Modified();

    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_PROJECTITEM_COPIED"));
    return(true);
}


//----------------------------------------------------------------
bool ProjectItemList::MoveProjectItem(size_t index, ProjectItem* dest)
/**
 * \brief Moves a project item from this container to another project item.
 * This method fails if the project item is moved to the same project item
 * or the index is out of range. A 'PENV_EVT_PROJECTITEM_MOVED' event
 * will be generated after the project item is successful moved.
 * \param index Index of the project item which should be moved.
 * \param dest Destination project item.
 * \return True on success; false otherwise.
 **/
{
    if (index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectItemList::MoveProjectItem] Index is out of range."));
        return (false);
    }
    if(dest == m_projectitemparent){
        wxLogWarning(_T("[penv::ProjectItemList::MoveProjectItem] Destination and source workspace are the same."));
        return (true);
    }
    if(dest->GetType() == penvPI_File) {
        wxLogError(_T("[penv::ProjectItemList::MoveProjectItem] Cannot move project item to project item file."));
        return (false);
    }
    ProjectItem* item = (*m_array)[index];
    item->SetParent(dest->GetItemList());
    dest->GetItemList()->m_array->Add(item);
    if(dest->GetType() == penvPI_SubProject)
    {
        ((ProjectItemSubProject*)dest)->Modified();
    }
    if (m_projectparent != NULL) {
        m_projectparent->Modified();
    } else {
        if(m_projectitemparent->GetType() == penvPI_SubProject)
        {
            ((ProjectItemSubProject*)m_projectitemparent)->Modified();
        }
    }
    m_array->Remove(index, false);
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_PROJECTITEM_MOVED"));
    return(true);
}


//----------------------------------------------------------------
bool ProjectItemList::CopyProjectItem(size_t index, ProjectItem* dest)
/**
 * \brief Copies a project item from this container to another project item.
 * This method fails if the index is out of range.
 * A 'PENV_EVT_PROJECTITEM_COPIED' event will be generated after the
 * project item is successful copied.
 * \param index Index of the project item which should be moved.
 * \param dest Destination project item.
 * \return True on success; false otherwise.
 **/
{
    if(index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectItemList::CopyProjectItem] Index out of range."));
        return (false);
    }
    ProjectItem* item = (*m_array)[index]->Clone();
    item->SetParent(dest->GetItemList());
    dest->GetItemList()->m_array->Add(item);
    if(dest->GetType() == penvPI_SubProject)
    {
        ((ProjectItemSubProject*)dest)->Modified();
    }

    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_PROJECTITEM_COPIED"));
    return(true);
}


//----------------------------------------------------------------
ProjectItemList* ProjectItemList::Clone()
/**
 * \brief Clones this class and returns it.
 * Parent project and projectitem will not be cloned, instead this
 * variable will be NULL.
 * \return Cloned instance of this class.
 **/
{
    ProjectItemList* projectitemlist = new ProjectItemList((ProjectItem*)NULL);
    projectitemlist->m_projectparent = NULL;
    projectitemlist->m_projectitemparent = NULL;
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        ProjectItem* item = (*m_array)[i]->Clone();
        projectitemlist->m_array->Add(item);
    }
    return (projectitemlist);
}


//----------------------------------------------------------------
bool ProjectItemList::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'projectitems' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'projectitems' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Knoten muss vom Typ Element sein
    if (node == NULL)
    {
        wxLogError(_T("[penv::ProjectItemList::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("projectitems"))
    {
        wxLogError(_T("[penv::ProjectItemList::ReadNode] Node should be an element node with name 'projectitems'."));
        return (false);
    }
    // Durch alle knoten unterhalb von 'projectitems' loopen
    wxXmlNode* itemnode = node->GetChildren();
    while (itemnode != NULL)
    {
        if (itemnode->GetType() == wxXML_ELEMENT_NODE &&
            itemnode->GetName() == _T("projectitem"))
        {
            wxString type = itemnode->GetPropVal(_T("type"),wxEmptyString);
            if(type.IsEmpty())
            {
                wxLogError(_T("[penv::ProjectItemList::ReadNode] Type attribut is missing."));
                return(false);
            }
            ProjectItem* item = CreateProjectItem(type);
            if (item == NULL)
            {
                wxLogError(_T("[penv::ProjectItemList::ReadNode] ProjectItemType '%s' undefined."), type.c_str());
                return(false);
            }
            if (!item->ReadNode(itemnode))
            {
                wxLogWarning(_T("[penv::ProjectItemList::ReadNode] ProjectItem failed to read."), type.c_str());
                itemnode = itemnode->GetNext();
                continue;
            }
            m_array->Add(item);
        }
        itemnode = itemnode->GetNext();
    }
    return (true);
}



//----------------------------------------------------------------
wxXmlNode* ProjectItemList::WriteNode()
/**
 * \brief Writes a 'projectitems' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'projectitems' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("projectitems"));
    for(size_t i = 0; i < m_array->Count(); i++)
    {
        wxXmlNode* nodeitem = (*m_array)[i]->WriteNode();
        if(nodeitem == NULL)
        {
            wxLogError(_T("[penv::ProjectItemList::WriteNode] Could not write project item '%s' xml node."), (*m_array)[i]->GetName().c_str());
            continue;
        }
        PenvHelper::AddXmlChildNode(node, nodeitem);
    }
    return (node);
}


//----------------------------------------------------------------
ProjectItem* ProjectItemList::CreateProjectItem(const wxString& type)
/**
 * \brief Creates a project item after a identifier. The possible identifiers
 * could be 'file', 'directory', 'linkeditem' or 'subproject'.
 * \param type Indentifier for the project item creation.
 * \return Project item on success; NULL otherwise.
 **/
{
    ProjectItem* item = NULL;
    if(type == _T("file"))  {
        item = new ProjectItemFile(this);
    }
    else if(type == _T("directory")) {
        item = new ProjectItemDirectory(this);
    }
    else if(type == _T("linkeditem")) {
        item = new ProjectItemLinkedItems(this);
    }
    else if(type == _T("subproject")) {
        item = new ProjectItemSubProject(this);
    }
    else {
        wxLogError(_T("[penv::ProjectItemList::CreateProjectItem] ProjectItemType '%s' undefined."), type.c_str());
    }
    return (item);
}


//----------------------------------------------------------------
ProjectItem* ProjectItemList::CreateProjectItem(ProjectItemType type)
/**
 * \brief Creates a project item after the type identifier. See
 * ProjectItemType for possible types.
 * \param type Indentifier for the project item creation.
 * \return Project item on success; NULL otherwise.
 **/
{
    ProjectItem* item = NULL;
    if(type == penvPI_File)  {
        item = new ProjectItemFile(this);
    }
    else if(type == penvPI_Directory) {
        item = new ProjectItemDirectory(this);
    }
    else if(type == penvPI_LinkedItems) {
        item = new ProjectItemLinkedItems(this);
    }
    else if(type == penvPI_SubProject) {
        item = new ProjectItemSubProject(this);
    }
    else {
        wxLogError(_T("[penv::ProjectItemList::CreateProjectItem] ProjectItemType is undefined."));
    }
    return (item);
}


//----------------------------------------------------------------
void ProjectItemList::Modified(bool modified)
/**
 * \brief Sets the parent project or subproject modified.
 * \param modified True if modified.
 **/
{
    if (m_projectparent != NULL) {
        m_projectparent->Modified(true);
    } else {
        // SubProject is modified
        if (m_projectitemparent->GetType() == penvPI_SubProject) {
            ((ProjectItemSubProject*)m_projectitemparent)->Modified(true);
        }
        // If parent ProjectItem is not a ProjectItemFile then
        //    deepest project must be set modified
        ProjectItemList* list = this;
        while (list->GetProjectItemParent() != NULL)
        {
            list = list->GetProjectItemParent()->GetParent();
        }
        Project* project = list->GetProjectParent();
        if (project == NULL) {
            wxLogError(_T("[penv::ProjectItemList::Modified] There is a mismatch between the project and project items."));
            return;
        }
        project->Modified(true);
    }
}



} // namespace penv
