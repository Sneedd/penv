/*
 * projecttreeaction.cpp - Implementation of the projecttreeaction class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/27/2008
 *
 */




#include "projecttreeaction.h"
#include "projecttreectrl.h"

namespace penv {


//----------------------------------------------------------------
ProjectTreeAction::ProjectTreeAction()
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
ProjectTreeAction::ProjectTreeAction(ProjectTreeCtrl* treectrl)
/**
 * \brief
 **/
{
    m_treectrl = treectrl;
}


//----------------------------------------------------------------
ProjectTreeAction::~ProjectTreeAction()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
ProjectTreeActionType ProjectTreeAction::GetType()
/**
 * \brief Returns the type of the executed action.
 * \return Type of the action.
 **/
{
    return (m_type);
}


//----------------------------------------------------------------
size_t ProjectTreeAction::CountSelections() const
/**
 * \brief Returns the number of selections in the treeview.
 * \return Number of selections.
 **/
{
    if (penvPROJECTTREECTRL_SELECTIONMODE == wxTR_SINGLE)
    {
        if (m_treectrl->GetTreeCtrl()->GetSelection().IsOk()) {
            return (1);
        } else {
            return (0);
        }
    }
    else
    {
        wxArrayTreeItemIds array;
        return (m_treectrl->GetTreeCtrl()->GetSelections(array));
    }
    return (0);
}


//----------------------------------------------------------------
PenvItem* ProjectTreeAction::GetSelection(size_t index)
/**
 * \brief Returns a selection after its index.
 * \param index Index of the selection.
 * \return The ProjectTreeItemData object of the selection.
 **/
{
    if (penvPROJECTTREECTRL_SELECTIONMODE == wxTR_SINGLE)
    {
        wxTreeItemId id = m_treectrl->GetTreeCtrl()->GetSelection();
        if (id.IsOk()) {
            ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetTreeCtrl()->GetItemData(id);
            return (data->GetPenvItem());
        }
    }
    else
    {
        wxArrayTreeItemIds array;
        m_treectrl->GetTreeCtrl()->GetSelections(array);
        if (index >= array.Count()) {
            wxLogError(_T("[penv::ProjectTreeAction::GetSelection] Index is out of range."));
        } else {
            ProjectTreeItemData* data = (ProjectTreeItemData*)m_treectrl->GetTreeCtrl()->GetItemData(array[index]);
            return (data->GetPenvItem());
        }
    }
    return (NULL);
}



//----------------------------------------------------------------
void ProjectTreeAction::OnAdd(WorkspaceList* list)
/**
 * \brief This method is called to add a new workspace to the
 * workspace list. This method must be overridden, for example if
 * you want to open a dialog to create a workspace with other
 * properties.
 * \param list The place where the workspace will be added to.
 **/
{
    // Create new unique qorkspace name
    wxString workspacename = _T("");
    int counter = 0;
    while (workspacename.IsEmpty())
    {
        workspacename = wxString::Format(_T("new workspace %i"), counter);
        for (size_t i=0; i<list->Count(); ++i)
        {
            if (workspacename == list->GetItem(i)->GetName()) {
                workspacename = _T("");
                counter++;
            }
        }
    }
    // Create workspace with previously created name
    Workspace* workspace = new Workspace();
    workspace->SetName(workspacename);
    workspace->Modified(true);
    // Add workspace to workspace list
    list->Add(workspace);
    // Add workspace to treectrl
    m_treectrl->AddWorkspace(workspace);
}


//----------------------------------------------------------------
void ProjectTreeAction::OnAdd(ProjectList* list)
/**
 * \brief This method is called to add a new project to the
 * project list. This method must be overridden, for example if
 * you want to open a dialog to create a project with other
 * properties.
 * \param list The place where the project will be added to.
 **/
{
    // Create new unique qorkspace name
    wxString projectname = _T("");
    int counter = 0;
    while (projectname.IsEmpty())
    {
        projectname = wxString::Format(_T("new project %i"), counter);
        for (size_t i=0; i<list->Count(); ++i)
        {
            if (projectname == list->GetItem(i)->GetName()) {
                projectname = _T("");
                counter++;
            }
        }
    }
    // Create workspace with previously created name
    Project* project = new Project(list);
    project->SetName(projectname);
    project->Modified(true);
    // Add workspace to workspace list
    list->Add(project);
    // Add workspace to treectrl
    wxTreeItemId id = list->GetParent()->GetTreeItemId();
    m_treectrl->AddProject(id, project);
}



//----------------------------------------------------------------
void ProjectTreeAction::OnAdd(ProjectItemList* list)
/**
 * \brief This method is called to add a new project item to the
 * project list. This method must be overridden, for example if
 * you want to open a dialog to create a project item with other
 * properties. The standard implementation just creates an empty
 * project item file.
 * \param list The place where the project will be added to.
 **/
{
    // Create new unique filename
    wxString itemname = _T("");
    int counter = 0;
    while (itemname.IsEmpty())
    {
        itemname = wxString::Format(_T("new_file_%i.txt"), counter);
        for (size_t i=0; i<list->Count(); ++i)
        {
            if (itemname == list->GetItem(i)->GetName()) {
                itemname = _T("");
                counter++;
            }
        }
    }
    // Create file with previously created name
    ProjectItemFile* item = new ProjectItemFile(list);
    item->SetName(itemname);
    // TODO SO: Sofort öffnen und window modified setzten!!!
    //item->Modified(true);
    // Add file to projectitem list
    list->Add(item);
    // Add file to treectrl
    wxTreeItemId id;
    if (list->GetProjectParent() != NULL) {
        id = list->GetProjectParent()->GetTreeItemId();
    } else {
        id = list->GetProjectItemParent()->GetTreeItemId();
    }
    m_treectrl->AddFile(id, item);
}


//----------------------------------------------------------------
void ProjectTreeAction::OnAdd(ProjectItemList* list, ProjectItemType type)
/**
 * \brief
 **/
{
    // Create new unique filename
    if (list == NULL) {
        wxLogError(_T("[penv::ProjectTreeAction::OnAdd] Argument 'list' is NULL."));
        return;
    }
    wxString itemname = _T("");
    int counter = 0;
    while (itemname.IsEmpty())
    {
        if (type == penvPI_File) {
            itemname = wxString::Format(_T("new_file_%i.txt"), counter);
        }
        else if (type == penvPI_Directory) {
            itemname = wxString::Format(_T("new_directory_%i"), counter);
        }
        for (size_t i=0; i<list->Count(); ++i)
        {
            if (itemname == list->GetItem(i)->GetName()) {
                itemname = _T("");
                counter++;
            }
        }
    }
    // Create file with previously created name
    ProjectItem* item = list->CreateProjectItem(type);
    item->SetName(itemname);
    if (type == penvPI_File) {
        // TODO SO: Sofort öffnen und window modified setzten!!!
        //((ProjectItemFile*)item)->Modified(true);
    }
    // Add file to projectitem list
    list->Add(item);
    // Add file to treectrl
    wxTreeItemId id;
    if (list->GetProjectParent() != NULL) {
        id = list->GetProjectParent()->GetTreeItemId();
    } else {
        id = list->GetProjectItemParent()->GetTreeItemId();
    }
    if (type == penvPI_File) {
        m_treectrl->AddFile(id, (ProjectItemFile*)item);
    }
    else if (type == penvPI_Directory) {
        m_treectrl->AddDirectory(id, (ProjectItemDirectory*)item, false);
    }
    else if (type == penvPI_LinkedItems) {
        m_treectrl->AddLinkedItems(id, (ProjectItemLinkedItems*)item);
    }
    else if (type == penvPI_SubProject) {
        m_treectrl->AddSubProject(id, (ProjectItemSubProject*)item, false);
    }
}


//----------------------------------------------------------------
void ProjectTreeAction::OnRemove(bool& cancel)
/**
 * \brief
 **/
{
}


//----------------------------------------------------------------
void ProjectTreeAction::OnMove(bool& cancel, Project* item,
    Workspace* destination, ProjectList* source)
/**
 * \brief
 **/
{
    if(destination == source->GetParent()) {
        cancel = true;
        wxLogWarning(_T("[penv::ProjectTreeAction::OnMove] Can not move project to the same workspace."));
        return;
    }
    // Moving in workspace
    int index = source->GetItemIndex(item);
    if (index == -1) {
        wxLogError(_T("[penv::ProjectTreeAction::OnMove] Project not found in workspace."));
        cancel = true;
        return;
    }
    if (!source->MoveProject(index, destination)) {
        wxLogError(_T("[penv::ProjectTreeAction::OnMove] Error occured while moving."));
        cancel= true;
        return;
    }

    // Moving in treectrl
    wxTreeItemId itemid = item->GetTreeItemId();
    wxTreeItemId sourceid = source->GetParent()->GetTreeItemId();
    wxTreeItemId destid = destination->GetTreeItemId();

    m_treectrl->GetTreeCtrl()->Delete(itemid);
    wxTreeItemId prjid = m_treectrl->AddProject(destid, item);
    m_treectrl->AddProjectItemList(prjid, item->GetProjectItemList(), true);
}


//----------------------------------------------------------------
void ProjectTreeAction::OnMove(bool& cancel, ProjectItemSubProject* item,
    Workspace* destination, ProjectItemList* source)
/**
 * \brief
 **/
{
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
void ProjectTreeAction::OnMove(bool& cancel, ProjectItem* item,
    Project* destination, ProjectItemList* source)
/**
 * \brief
 **/
{
    if (destination == source->GetProjectParent()) {
        cancel = true;
        wxLogWarning(_T("[penv::ProjectTreeAction::OnMove] Can not move project item to the same project."));
        return;
    }
    // Moving in project
    int index = source->GetItemIndex(item);
    if (index == -1) {
        wxLogError(_T("[penv::ProjectTreeAction::OnMove] Project item not found in project."));
        cancel = true;
        return;
    }
    if (!source->MoveProjectItem(index, destination)) {
        wxLogError(_T("[penv::ProjectTreeAction::OnMove] Error occured while moving."));
        cancel= true;
        return;
    }

    // Moving in treectrl
    wxTreeItemId itemid = item->GetTreeItemId();
    wxTreeItemId sourceid;
    if(source->GetProjectParent() != NULL)
    {
        sourceid = source->GetProjectParent()->GetTreeItemId();
    }
    else
    {
        sourceid = source->GetProjectItemParent()->GetTreeItemId();
    }
    wxTreeItemId destid = destination->GetTreeItemId();

    m_treectrl->GetTreeCtrl()->Delete(itemid);
    if(item->GetType() == penvPI_File)
    {
        wxTreeItemId id = m_treectrl->AddFile(destid, (ProjectItemFile*)item);
    }
    else if(item->GetType() == penvPI_Directory)
    {
        m_treectrl->AddDirectory(destid, (ProjectItemDirectory*)item, true);
    }
    else if(item->GetType() == penvPI_SubProject)
    {
        m_treectrl->AddSubProject(destid, (ProjectItemSubProject*)item,true);
    }
    else if(item->GetType() == penvPI_LinkedItems)
    {
        wxTreeItemId id = m_treectrl->AddLinkedItems(destid, (ProjectItemLinkedItems*)item);
    }
}


//----------------------------------------------------------------
void ProjectTreeAction::OnMove(bool& cancel, ProjectItem* item,
    ProjectItem* destination, ProjectItemList* source)
/**
 * \brief
 **/
{
    if (destination->GetItemList() == source) {
        cancel = true;
        wxLogWarning(_T("[penv::ProjectTreeAction::OnMove] Can not move project item to the same project item."));
        return;
    }
    // Moving in projectitem
    int index = source->GetItemIndex(item);
    if (index == -1) {
        wxLogError(_T("[penv::ProjectTreeAction::OnMove] Project item not found in project item."));
        cancel = true;
        return;
    }
    if (!source->MoveProjectItem(index, destination)) {
        wxLogError(_T("[penv::ProjectTreeAction::OnMove] Error occured while moving."));
        cancel= true;
        return;
    }

    // Moving in treectrl
    wxTreeItemId itemid = item->GetTreeItemId();
    wxTreeItemId sourceid;
    if(source->GetProjectParent() != NULL)
    {
        sourceid = source->GetProjectParent()->GetTreeItemId();
    }
    else
    {
        sourceid = source->GetProjectItemParent()->GetTreeItemId();
    }
    wxTreeItemId destid = destination->GetTreeItemId();

    m_treectrl->GetTreeCtrl()->Delete(itemid);
    if(item->GetType() == penvPI_File)
    {
        wxTreeItemId id = m_treectrl->AddFile(destid, (ProjectItemFile*)item);
    }
    else if(item->GetType() == penvPI_Directory)
    {
        m_treectrl->AddDirectory(destid, (ProjectItemDirectory*)item, true);
    }
    else if(item->GetType() == penvPI_SubProject)
    {
        m_treectrl->AddSubProject(destid, (ProjectItemSubProject*)item,true);
    }
    else if(item->GetType() == penvPI_LinkedItems)
    {
        wxTreeItemId id = m_treectrl->AddLinkedItems(destid, (ProjectItemLinkedItems*)item);
    }

}


//----------------------------------------------------------------
void ProjectTreeAction::OnCopy(bool& cancel, Project* item,
    Workspace* destination, ProjectList* source)
/**
 * \brief
 **/
{
    if(destination == source->GetParent()) {
        cancel = true;
        wxLogWarning(_T("[penv::ProjectTreeAction::OnMove] Can not copy project to the same workspace."));
        return;
    }
    // Moving in workspace
    int index = source->GetItemIndex(item);
    if (index == -1) {
        wxLogError(_T("[penv::ProjectTreeAction::OnCopy] Project not found in workspace."));
        cancel = true;
        return;
    }
    if (!source->CopyProject(index, destination)) {
        wxLogError(_T("[penv::ProjectTreeAction::OnCopy] Error occured while moving."));
        cancel= true;
        return;
    }

    // Copying in treectrl
    wxTreeItemId itemid = item->GetTreeItemId();
    wxTreeItemId sourceid = source->GetParent()->GetTreeItemId();
    wxTreeItemId destid = destination->GetTreeItemId();

    wxTreeItemId prjid = m_treectrl->AddProject(destid, item);
    m_treectrl->AddProjectItemList(prjid, item->GetProjectItemList(), true);

}


//----------------------------------------------------------------
void ProjectTreeAction::OnCopy(bool& cancel, ProjectItemSubProject* item,
    Workspace* destination, ProjectItemList* source)
/**
 * \brief
 **/
{
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
void ProjectTreeAction::OnCopy(bool& cancel, ProjectItem* item,
    Project* destination, ProjectItemList* source)
/**
 * \brief
 **/
{
    if (destination == source->GetProjectParent()) {
        cancel = true;
        wxLogWarning(_T("[penv::ProjectTreeAction::OnCopy] Can not copy project item to the same project."));
        return;
    }
    // Moving in project
    int index = source->GetItemIndex(item);
    if (index == -1) {
        wxLogError(_T("[penv::ProjectTreeAction::OnCopy] Project item not found in project."));
        cancel = true;
        return;
    }
    if (!source->CopyProjectItem(index, destination)) {
        wxLogError(_T("[penv::ProjectTreeAction::OnCopy] Error occured while moving."));
        cancel= true;
        return;
    }

    // Copying in treectrl
    wxTreeItemId itemid = item->GetTreeItemId();
    wxTreeItemId sourceid;
    if(source->GetProjectParent() != NULL)
    {
        sourceid = source->GetProjectParent()->GetTreeItemId();
    }
    else
    {
        sourceid = source->GetProjectItemParent()->GetTreeItemId();
    }
    wxTreeItemId destid = destination->GetTreeItemId();

    if(item->GetType() == penvPI_File)
    {
        wxTreeItemId id = m_treectrl->AddFile(destid, (ProjectItemFile*)item);
    }
    else if(item->GetType() == penvPI_Directory)
    {
        m_treectrl->AddDirectory(destid, (ProjectItemDirectory*)item, true);
    }
    else if(item->GetType() == penvPI_SubProject)
    {
        m_treectrl->AddSubProject(destid, (ProjectItemSubProject*)item,true);
    }
    else if(item->GetType() == penvPI_LinkedItems)
    {
        wxTreeItemId id = m_treectrl->AddLinkedItems(destid, (ProjectItemLinkedItems*)item);
    }
}


//----------------------------------------------------------------
void ProjectTreeAction::OnCopy(bool& cancel, ProjectItem* item,
    ProjectItem* destination, ProjectItemList* source)
/**
 * \brief
 **/
{
    if (destination->GetItemList() == source) {
        cancel = true;
        wxLogWarning(_T("[penv::ProjectTreeAction::OnCopy] Can not copy project item to the same project item."));
        return;
    }
    // Moving in projectitem
    int index = source->GetItemIndex(item);
    if (index == -1) {
        wxLogError(_T("[penv::ProjectTreeAction::OnCopy] Project item not found in project item."));
        cancel = true;
        return;
    }
    if (!source->CopyProjectItem(index, destination)) {
        wxLogError(_T("[penv::ProjectTreeAction::OnCopy] Error occured while moving."));
        cancel= true;
        return;
    }

    // Moving in treectrl
    wxTreeItemId itemid = item->GetTreeItemId();
    wxTreeItemId sourceid;
    if(source->GetProjectParent() != NULL)
    {
        sourceid = source->GetProjectParent()->GetTreeItemId();
    }
    else
    {
        sourceid = source->GetProjectItemParent()->GetTreeItemId();
    }
    wxTreeItemId destid = destination->GetTreeItemId();

    if(item->GetType() == penvPI_File)
    {
        wxTreeItemId id = m_treectrl->AddFile(destid, (ProjectItemFile*)item);
    }
    else if(item->GetType() == penvPI_Directory)
    {
        m_treectrl->AddDirectory(destid, (ProjectItemDirectory*)item, true);
    }
    else if(item->GetType() == penvPI_SubProject)
    {
        m_treectrl->AddSubProject(destid, (ProjectItemSubProject*)item,true);
    }
    else if(item->GetType() == penvPI_LinkedItems)
    {
        wxTreeItemId id = m_treectrl->AddLinkedItems(destid, (ProjectItemLinkedItems*)item);
    }

}




//----------------------------------------------------------------
void ProjectTreeAction::OnRename(bool& cancel, const wxString& name)
/**
 * \brief
 **/
{
}



} // namespace penv


