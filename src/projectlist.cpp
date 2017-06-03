/*
 * projectlist.cpp - Implementation of the ProjectList class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/01/2007
 *
 */


#include "projectlist.h"
#include "expect.h"
#include "penvhelper.h"
#include "workspace.h"
#include "environment.h"

namespace penv {

//----------------------------------------------------------------
ProjectList::ProjectList()
/**
 * \brief Constructor (private).
 **/
{
    wxLogError(_T("[penv::ProjectList::ProjectList] Not allowed to be called."));
    wxASSERT(false);
}



//----------------------------------------------------------------
ProjectList::ProjectList(Workspace* parent)
/**
 * \brief Constructor, initializes the Workspace object.
 * \param parent The parent workspace of this list.
 **/
{
    m_parent = parent;
    m_array = new Array<Project>();
}



//----------------------------------------------------------------
ProjectList::~ProjectList()
/**
 * \brief Destructor.
 **/
{
    Clear();
    delete m_array;
}


//----------------------------------------------------------------
Project* ProjectList::GetItem(size_t index)
/**
 * \brief Returns a project from this container. This method
 * returns NULL if index is out of range.
 * \param index Index of a project.
 * \return The project at spezified index or NULL.
 **/
{
    return (m_array->ItemPtr(index));
}


//----------------------------------------------------------------
int ProjectList::GetItemIndex(Project* project)
/**
 * \brief Returns the index of the given project.
 * Can return -1 if the project is not found.
 * \param project The Project to search the index for.
 * \return Index of the Project on success; -1 if not found.
 **/
{
    int index = -1;

    for(size_t i = 0; i < m_array->Count(); i++)
    {
        if (project == m_array->ItemPtr(i)) {
            index = i;
            break;
        }
    }
    return index;
}

//----------------------------------------------------------------
bool ProjectList::Add(Project* prj)
/**
 * \brief Adds a project to the end of this container.
 * \param prj The project which should be added.
 * \return True on success; false otherwise.
 **/
{
    if(prj == NULL)
    {
        wxLogError(_T("[penv::ProjectList::Add] Argument 'prj' is NULL."));
        return(false);
    }
    m_array->Add(prj);
    prj->SetParent(this);
    m_parent->Modified(true);
    return(true);
}


//----------------------------------------------------------------
bool ProjectList::Remove(size_t index)
/**
 * \brief Removes a project from this container. This method
 * fails if the index is out of range.
 * \param index Index of the project which should be removed.
 * \return True on success; false otherwise.
 **/
{
    if(index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectList::Remove] Index is out of range."));
        return(false);
    }
    return(m_array->Remove(index));
}


//----------------------------------------------------------------
void ProjectList::Clear()
/**
 * \brief Removes all projects from this container.
 **/
{
    m_array->Clear(true);
}


//----------------------------------------------------------------
size_t ProjectList::Count()
/**
 * \brief Returns the number of projects in this container.
 * \return Number of projects in this container.
 **/
{

    return (m_array->Count());
}



//----------------------------------------------------------------
Workspace* ProjectList::GetParent()
/**
 * \brief Returns the parent workspace of this object.
 * \return Parent workspace.
 **/
{
   return (m_parent);
}

//----------------------------------------------------------------
void ProjectList::SetParent(Workspace* workspace)
/**
 * \brief Sets the parent workspace of this object.
 * \param workspace The parent workspace.
 **/
{
   m_parent = workspace;
}

//----------------------------------------------------------------
bool ProjectList::MoveProject(size_t index, Workspace* dest)
/**
 * \brief Moves a project from this container to another workspace.
 * This method fails if the project is moved to the same workspace
 * or the index is out of range. A 'PENV_EVT_PROJECT_MOVED' event
 * will be generated after the project is successful moved.
 * \param index Index of the project which should be moved.
 * \param dest Destination workspace.
 * \return True on success; false otherwise.
 **/
{
    if (index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectList::MoveProject] Index is out of range."));
        return (false);
    }
    if(dest == m_parent){
        wxLogWarning(_T("[penv::ProjectList::MoveProject] Destination and source workspace are the same."));
        return (true);
    }
    Project* project = (*m_array)[index];
    project->SetParent(dest->GetProjectList());
    dest->GetProjectList()->m_array->Add(project);
    dest->Modified();
    m_parent->Modified();
    m_array->Remove(index, false);
    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_PROJECT_MOVED"));
    return(true);
}



//----------------------------------------------------------------
bool ProjectList::CopyProject(size_t index, Workspace* dest)
/**
 * \brief Copies a project from this container to another workspace.
 * This method fails if the index is out of range.
 * A 'PENV_EVT_PROJECT_COPIED' event will be generated after the
 * project is successful copied.
 * \param index Index of the project which should be moved.
 * \param dest Destination workspace.
 * \return True on success; false otherwise.
 **/
{
    if(index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ProjectList::CopyProject] Index out of range."));
        return (false);
    }

    Project* project = (*m_array)[index]->Clone();
    project->SetParent(dest->GetProjectList());
    dest->GetProjectList()->m_array->Add(project);
    dest->Modified();

    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_PROJECT_COPIED"));
    return(true);
}



//----------------------------------------------------------------
bool ProjectList::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'projects' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'projects' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Knoten muss vom Typ Element sein
    if (node == NULL)
    {
        wxLogError(_T("[penv::ProjectList::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("projects"))
    {
        wxLogError(_T("[penv::ProjectList::ReadNode] Node should be an element node with name 'projects'."));
        return (false);
    }
    // Durch alle knoten unterhalb von 'projects' loopen
    wxXmlNode* projectnode = node->GetChildren();
    while (projectnode != NULL)
    {
        if (projectnode->GetType() == wxXML_ELEMENT_NODE &&
            projectnode->GetName() == _T("project"))
        {
            Project* project = new Project(this);
            if (!project->ReadNode(projectnode))
            {
                projectnode = projectnode->GetNext();
                continue;
            }
            m_array->Add(project);
        }
        projectnode = projectnode->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* ProjectList::WriteNode()
/**
 * \brief Writes a 'projects' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'projects' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("projects"));
    for (size_t i=0; i<m_array->Count(); i++)
    {
        Project* project = m_array->ItemPtr(i);
        if (project == NULL)
        {
            wxLogError(_T("[penv::ProjectList::WriteNode] Project is NULL, skipping project."));
            continue;
        }
        wxXmlNode* projectnode = project->WriteNode();
        if (projectnode == NULL)
        {
            wxLogError(_T("[penv::ProjectList::WriteNode] Can not write project '%s' node, skipping project."), project->GetName().c_str());
            continue;
        }
        PenvHelper::AddXmlChildNode(node, projectnode);
    }
    return (node);
}

//----------------------------------------------------------------
ProjectList* ProjectList::Clone()
/**
 * \brief Clones this class and returns it.
 * Parent workspace will not be cloned, instead this
 * variable will be NULL.
 * \return Cloned instance of this class.
 **/
{
    ProjectList* list = new ProjectList(NULL);
    for(size_t i = 0; i < m_array->Count(); i++)
    {
        Project* project = (*m_array)[i]->Clone();
        list->m_array->Add(project);
    }
    return (list);
}

} // namespace penv


