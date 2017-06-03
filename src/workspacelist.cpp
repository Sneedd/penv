/*
 * workspacelist.cpp - Implementation of the WorkspaceList class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/01/2007
 *
 */


#include "workspacelist.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"
#include "commandlist.h"
#include "ui/projecttreeitemdata.h"




namespace penv {

//----------------------------------------------------------------
WorkspaceList::WorkspaceList()
/**
 * \brief Constructor, initializes the WorkspaceList object.
 **/
{
    m_array = new Array<Workspace>();
    // Connect the WorkspaceList command to the workspace
    CommandList* list = Environment::Get()->GetCommandList();
    Command* cmd = list->GetCommand(_T("PENV_EVT_PROJECTTREECTRL_ITEM_ACTIVATED"));
    if (cmd != NULL) {
        cmd->Connect(new CommandCallback<WorkspaceList>(this,&WorkspaceList::OnItemActivated));
    }
}

//----------------------------------------------------------------
WorkspaceList::~WorkspaceList()
/**
 * \brief Destructor.
 **/
{
    if (m_array != NULL) delete m_array;
}


//----------------------------------------------------------------
Workspace* WorkspaceList::GetItem(size_t index)
/**
 * \brief Returns a workspace from this container.
 * \param index Index of workspace.
 * \return The workspace or NULL.
 **/
{
    if (unlikely(index < 0 || index >= m_array->Count())) {
        wxLogError(_T("[penv::WorkspaceList::Item] Index is out ouf range."));
        return (NULL);
    }
    return (m_array->ItemPtr(index));
}


//----------------------------------------------------------------
int WorkspaceList::GetItemIndex(Workspace* workspace)
/**
 * \brief Returns the index of the given workspace.
 * Can return -1 if the workspace is not found.
 * \param item The workspace to search the index for.
 * \return Index of the workspace on success; -1 if not found.
 **/
{
    int index = -1;

    for(size_t i = 0; i < m_array->Count(); i++)
    {
        if (workspace == m_array->ItemPtr(i)) {
            index = i;
            break;
        }
    }
    return index;
}


//----------------------------------------------------------------
bool WorkspaceList::Add(Workspace* workspace)
/**
 * \brief Adds a new workspace to this container.
 * \param workspace The workspace which should be added.
 * \return True on success; false otherwise.
 **/
{
    if (workspace == NULL) {
        wxLogError(_T("[penv::WorkspaceList::Add] Argument 'workspace' is NULL."));
        return (false);
    }
    m_array->Add(workspace);
    return (true);
}


//----------------------------------------------------------------
bool WorkspaceList::Remove(size_t index)
/**
 * \brief Removes a workspace from this container.
 * \param index Index of the workspace which should be removed.
 * \return True on success; false otherwise.
 **/
{
    if (index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::WorkspaceList::Remove] Index is out of range."));
        return (false);
    }
    m_array->Remove(index);
    return (true);
}


//----------------------------------------------------------------
void WorkspaceList::Clear()
/**
 * \brief Removes all workspaces from this container.
 **/
{
    // TODO MS: Registrierung der ProjektItems beachten (nach Windowmanagement implementierbar!)
    NOT_IMPLEMENTED_YET();
    m_array->Clear();
}




//----------------------------------------------------------------
size_t WorkspaceList::Count() const
/**
 * \brief Returns the number of workspaces in this container.
 * \return Number of workspaces in this container.
 **/
{
    return (m_array->Count());
}


//----------------------------------------------------------------
bool WorkspaceList::CopyWorkspace(size_t index)
/**
 * \brief Copys a workspace in this container and adds it
 * after it was copied. Copies all items in the workspaces, too.
 * \param index The index of the workspace which should be copied.
 * \return True on success; false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (false);
}



//----------------------------------------------------------------
bool WorkspaceList::IsWorkspaceOrProjectModified()
/**
 * \brief Returns true if a workspace or project is modified.
 * \return True if workspace or project is modified.
 **/
{
    bool modified = false;
    for (size_t i=0; i<m_array->Count(); i++)
    {
        Workspace* workspace = (*m_array)[i];
        if (workspace == NULL) {
            wxLogWarning(_T("[penv::WorkspaceList::IsWorkspaceOrProjectModified] Workspace is NULL."));
            continue;
        }
        if (workspace->IsModified()) {
            modified = true;
            break;
        }
        for (size_t j=0; j<workspace->GetProjectList()->Count(); j++)
        {
            Project* project = workspace->GetProjectList()->GetItem(j);
            if (project == NULL) {
                wxLogWarning(_T("[penv::WorkspaceList::IsWorkspaceOrProjectModified] Project is NULL."));
                continue;
            }
            if (project->IsModified()) {
                modified = true;
                break;
            }
        }
    }
    return (modified);
}


//----------------------------------------------------------------
bool WorkspaceList::ReloadWorkspaces()
/**
 * \brief Reloads the workspaces file. All workspaces in this
 * container will be removed and added again if they are available
 * in the file.
 * \return True on success; false otherwise.
 **/
{
    Clear();
    wxString workspaceconfigfile = Environment::Get()->GetWorkspacesFile(false);
    wxXmlDocument docworkspace(workspaceconfigfile);
    // Prüfen ob Datei korrekt geladen ...
    if (!docworkspace.IsOk())
    {
        wxLogWarning(_T("[penv::WorkspaceList::ReloadWorkspaces] Workspace file '%s' cannot be loaded."), workspaceconfigfile.c_str());
    }
    // ... wenn Datei korrekt geladen wurde, dann auslesen
    else if (!this->ReadNode(docworkspace.GetRoot()))
    {
        wxLogWarning(_T("[penv::WorkspaceList::ReloadWorkspaces] Error occured during loading of workspace file."));
    }
    return (true);
}




//----------------------------------------------------------------
bool WorkspaceList::ReadNode(wxXmlNode* node)
/**
 * \brief Reads a 'workspaces' xml element node.
 * See class description for more informations.
 * \param node The 'workspaces' xml element node.
 * \return True on success; false otherwise.
 **/
{
    if (node == NULL)
    {
        wxLogError(_T("[penv::WorkspaceList::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("workspaces"))
    {
        wxLogError(_T("[penv::WorkspaceList::ReadNode] Node is not an element node with name 'workspaces'."));
        return (false);
    }
    // Durch alle knoten unterhalb von 'workspaces' loopen
    wxXmlNode* workspacenode = node->GetChildren();
    while (workspacenode != NULL)
    {
        if (workspacenode->GetType() == wxXML_ELEMENT_NODE &&
            workspacenode->GetName() == _T("workspace"))
        {
            Workspace* workspace = new Workspace();
            if (!workspace->ReadNode(workspacenode))
            {
                workspacenode = workspacenode->GetNext();
                continue;
            }
            m_array->Add(workspace);
        }
        workspacenode = workspacenode->GetNext();
    }
    return (true);
}



//----------------------------------------------------------------
wxXmlNode* WorkspaceList::WriteNode()
/**
 * \brief Writes a 'workspaces' xml element node.
 * See class description for more informations.
 * \return The 'workspaces' xml element node.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("workspaces"));
    for (size_t i=0; i<m_array->Count(); i++)
    {
        Workspace* workspace = m_array->ItemPtr(i);
        if (workspace == NULL)
        {
            wxLogError(_T("[penv::WorkspaceList::WriteNode] Workspace is NULL, skipping workspace."));
            continue;
        }
        wxXmlNode* workspacenode = workspace->WriteNode();
        if (workspacenode == NULL)
        {
            wxLogError(_T("[penv::WorkspaceList::WriteNode] Can not write workspace '%s' node, skipping workspace."), workspace->GetName().c_str());
            continue;
        }
        PenvHelper::AddXmlChildNode(node, workspacenode);
    }
    return (node);
}



//----------------------------------------------------------------
void WorkspaceList::PrintHierarchy()
/**
 * \brief Prints out the whole project manager hierarchy.
 **/
{
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        Workspace* workspace = (*m_array)[i];
        wxLogMessage(_T("Workspace: %s"), workspace->GetName().c_str());
        for (size_t j=0; j<workspace->GetProjectList()->Count(); ++j)
        {
            Project* project = workspace->GetProjectList()->GetItem(j);
            wxLogMessage(_T("  Project: %s"), project->GetName().c_str());
            PrintHierarchy(project->GetProjectItemList(), 2);
        }
    }
}

//----------------------------------------------------------------
void WorkspaceList::PrintHierarchy(ProjectItemList* list, int depth)
/**
 * \brief Recursive method for PrintHierarchy(). Used only
 * internally.
 * \param list The ProjectItemList to print.
 **/
{
    wxString indentation;
    for (size_t i=0; i<depth; ++i) indentation.Append(_T("  "));
    for (size_t i=0; i<list->Count(); ++i)
    {
        wxLogMessage(_T("%sProjectItem: %s"), indentation.c_str(), list->GetItem(i)->GetName().c_str());
        if (list->GetItem(i)->GetItemList() != NULL) {
            PrintHierarchy(list->GetItem(i)->GetItemList(), depth+1);
        }
    }
}


//----------------------------------------------------------------
void WorkspaceList::OnItemActivated(CommandEvent& event)
/**
 * \brief This event is connected to the PENV_EVT_PROJECTTREECTRL_ITEM_ACTIVATED
 * and introduces a standard behaivor for it. There are checks done for this and
 * calls for every ProjectItem, Workspace or Project the approriate Event see
 * in the List:<br>
 * <ul>
 * <li><b>Workspace:</b> Calls PENV_EVT_ACTIVATED_WORKSPACE. Sends in the wxVariant(void*)
 * a the Workspace* for the activated workspace.</li>
 * <li><b>Project:</b> Calls PENV_EVT_ACTIVATED_PROJECT. Sends in the wxVariant(void*)
 * a the Project* for the activated project.</li>
 * <li><b>ProjectItemFile:</b> Calls PENV_EVT_ACTIVATED_PROJECTITEMFILE. Sends in the wxVariant(void*)
 * a the ProjectItemFile* for the activated project item file.</li>
 * <li><b>ProjectItemDirectory:</b> Calls PENV_EVT_ACTIVATED_PROJECTITEMDIRECTORY. Sends in the wxVariant(void*)
 * a the ProjectItemDirectory* for the activated project item directory.</li>
 * <li><b>ProjectItemLinkedItems:</b> Calls PENV_EVT_ACTIVATED_PROJECTITEMLINKEDITEMS. Sends in the wxVariant(void*)
 * a the ProjectItemLinkedItems* for the activated project item linked items.</li>
 * <li><b>ProjectItemSubProject:</b> Calls PENV_EVT_ACTIVATED_PROJECTITEMSUBPROJECT. Sends in the wxVariant(void*)
 * a the ProjectItemSubProject* for the activated project item sub project.</li>
 * </ul>
 * \param event The event sends via the wxVariant(void*) a ProjectTreeItemData
 * pointer.
 **/
{
    ProjectTreeItemData* data = (ProjectTreeItemData*)event.GetVariant().GetVoidPtr();
    if (data != NULL)
    {
        //data->LogClassInfo();
        if (data->GetType() == penvTI_ProjectItem)
        {
            ProjectItem* item = data->GetProjectItem();
            if (item != NULL)
            {
                if (item->GetType() == penvPI_File)
                {
                    wxVariant variant = (void*)item;
                    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_ACTIVATED_PROJECTITEMFILE"), variant);
                }
                else if (item->GetType() == penvPI_Directory)
                {
                    wxVariant variant = (void*)item;
                    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_ACTIVATED_PROJECTITEMDIRECTORY"), variant);
                }
                else if (item->GetType() == penvPI_LinkedItems)
                {
                    wxVariant variant = (void*)item;
                    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_ACTIVATED_PROJECTITEMLINKEDITEMS"), variant);
                }
                else if (item->GetType() == penvPI_SubProject)
                {
                    wxVariant variant = (void*)item;
                    Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_ACTIVATED_PROJECTITEMSUBPROJECT"), variant);
                }
            }
        }
        else if (data->GetType() == penvTI_Project)
        {
            wxVariant variant = (void*)data->GetProject();
            Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_ACTIVATED_PROJECT"), variant);
        }
        else if (data->GetType() == penvTI_Workspace)
        {
            wxVariant variant = (void*)data->GetWorkspace();
            Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_ACTIVATED_WORKSPACE"), variant);
        }
    }
}


} // namespace penv

