/*
 * projecttreectrldroptarget.cpp - Implementation of the ProjectTreeCtrlDropTarget class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 06/03/2008
 *
 */


#include "projecttreectrldroptarget.h"
#include "projecttreeitemdata.h"
#include "projecttreeitemdataobject.h"
#include "projecttreectrl.h"

namespace penv {

//----------------------------------------------------------------
ProjectTreeCtrlDropTarget::ProjectTreeCtrlDropTarget(ProjectTreeCtrl* treecontrol, wxDataObject* data)
    : wxDropTarget(data)
/**
 * \brief Constructor, initializes the ProjectTreeCtrlDropTarget object.
 * \param treecontrol The ProjectTreeCtrl of this drop target.
 * \param data The data object which will be used for drag and drop.
 **/
{
    m_treectrl = treecontrol;
}


//----------------------------------------------------------------
ProjectTreeCtrlDropTarget::~ProjectTreeCtrlDropTarget()
/**
 * \brief Destructor.
 **/
{
    //Nichts zu tun!
}


//----------------------------------------------------------------
wxDragResult ProjectTreeCtrlDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def)
/**
 * \brief Called after OnDrop returns true. By default this will usually GetData
 * and will return the suggested default value def.
 * \param x The x coordinate of the mouse.
 * \param y The y coordinate of the mouse.
 * \param def Suggested DnD action.
 * \return Returned DnD action.
 **/
{
    if (!GetData()) return (wxDragCancel);
    if (def == wxDragMove || def == wxDragCopy)
    {
        return (def);
    }
    return (wxDragCancel);
}


//----------------------------------------------------------------
bool ProjectTreeCtrlDropTarget::OnDrop(wxCoord x, wxCoord y)
/**
 * \brief Called when the user drops a data object on the target.
 * Return false to veto the operation.
 * \param x The x coordinate of the mouse.
 * \param y The y coordinate of the mouse.
 * \return False to veto operation; true otherwise.
 **/
{
    int flags = wxTREE_HITTEST_ONITEMICON | wxTREE_HITTEST_ONITEMLABEL;
    wxTreeItemId id = m_treectrl->GetTreeCtrl()->HitTest(wxPoint(x,y), flags);
    if (!id.IsOk())
    {
        return (false);
    }
    ProjectTreeItemData* target = (ProjectTreeItemData*)m_treectrl->GetTreeCtrl()->GetItemData(id);
    ProjectTreeItemDataObject* source = (ProjectTreeItemDataObject*)m_dataObject;
    if (source->GetType() == penvTI_Project && target->GetType() == penvTI_Workspace)
    {
        ProjectTreeAction* action = m_treectrl->GetActionHandler();
        bool cancel = false;
        if (m_dragresult == wxDragMove)
        {
            action->OnMove(cancel, source->GetProject(), target->GetWorkspace(), source->GetProject()->GetParent());
        }
        else if (m_dragresult == wxDragCopy)
        {
            action->OnCopy(cancel, source->GetProject(), target->GetWorkspace(), source->GetProject()->GetParent());
        }
        return (cancel);
    }
    else if(source->GetType() == penvTI_ProjectItem && target->GetType() == penvTI_Project)
    {
        ProjectTreeAction* action = m_treectrl->GetActionHandler();
        bool cancel = false;
        if (m_dragresult == wxDragMove)
        {
            action->OnMove(cancel, source->GetProjectItem(), target->GetProject(), source->GetProjectItem()->GetParent());
        }
        else if (m_dragresult == wxDragCopy)
        {
            action->OnCopy(cancel, source->GetProjectItem(), target->GetProject(), source->GetProjectItem()->GetParent());
        }
        return (cancel);
    }
    else if(source->GetType() == penvTI_ProjectItem && target->GetType() == penvTI_ProjectItem)
    {
        ProjectItem* item = target->GetProjectItem();
        if (item->GetType() != penvPI_File && item->GetType() != penvPI_LinkedItems)
        {
            ProjectTreeAction* action = m_treectrl->GetActionHandler();
            bool cancel = false;
            if (m_dragresult == wxDragMove)
            {
                action->OnMove(cancel, source->GetProjectItem(), target->GetProjectItem(), source->GetProjectItem()->GetParent());
            }
            else if (m_dragresult == wxDragCopy)
            {
                action->OnCopy(cancel, source->GetProjectItem(), target->GetProjectItem(), source->GetProjectItem()->GetParent());
            }
            return (true);
        }
    }
    else if(source->GetType() == penvTI_ProjectItem && target->GetType() == penvTI_Workspace)
    {
        ProjectItem* item = source->GetProjectItem();
        if (item->GetType() == penvPI_SubProject)
        {
            ProjectTreeAction* action = m_treectrl->GetActionHandler();
            bool cancel = false;
            if (m_dragresult == wxDragMove)
            {
                action->OnMove(cancel, (ProjectItemSubProject*)source->GetProjectItem(), target->GetWorkspace(), source->GetProjectItem()->GetParent());
            }
            else if (m_dragresult == wxDragCopy)
            {
                action->OnCopy(cancel, (ProjectItemSubProject*)source->GetProjectItem(), target->GetWorkspace(), source->GetProjectItem()->GetParent());
            }
            return (true);
        }
    }
    return (false);
}


//----------------------------------------------------------------
wxDragResult ProjectTreeCtrlDropTarget::OnEnter(wxCoord x, wxCoord y, wxDragResult def)
/**
 * \brief Called when the mouse enters the drop target.
 * By default, this calls OnDragOver.
 * \param x The x coordinate of the mouse.
 * \param y The y coordinate of the mouse.
 * \param def Suggested DnD action.
 * \return Returned DnD action.
 **/
{
    if (def == wxDragMove || def == wxDragCopy)
    {
        return (def);
    }
    return (wxDragCancel);
}


//----------------------------------------------------------------
wxDragResult ProjectTreeCtrlDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
/**
 * \brief Called when the mouse is being dragged over the drop target.
 * By default, this calls functions return the suggested return value def.
 * \param x The x coordinate of the mouse.
 * \param y The y coordinate of the mouse.
 * \param def Suggested DnD action.
 * \return Returned DnD action.
 **/
{
    if (def != wxDragMove && def != wxDragCopy) return (wxDragNone);
    int flags = wxTREE_HITTEST_ONITEMICON | wxTREE_HITTEST_ONITEMLABEL;
    wxTreeItemId id = m_treectrl->GetTreeCtrl()->HitTest(wxPoint(x,y), flags);
    if(!id.IsOk()) return (wxDragNone);
    ProjectTreeItemData* target = (ProjectTreeItemData*)m_treectrl->GetTreeCtrl()->GetItemData(id);
    ProjectTreeItemDataObject* source = (ProjectTreeItemDataObject*)m_dataObject;
    m_dragresult = def;

    if (source->GetType() == penvTI_Project && target->GetType() == penvTI_Workspace)
    {
        return (def);
    }
    else if (source->GetType() == penvTI_ProjectItem && target->GetType() == penvTI_Project)
    {
        return (def);
    }
    else if (source->GetType() == penvTI_ProjectItem && target->GetType() == penvTI_ProjectItem)
    {
        ProjectItem* item = target->GetProjectItem();
        if (item->GetType() != penvPI_File && item->GetType() != penvPI_LinkedItems) {
            return (def);
        }
    }
    else if(source->GetType() == penvTI_ProjectItem && target->GetType() == penvTI_Workspace)
    {
        ProjectItem* item = source->GetProjectItem();
        if (item->GetType() == penvPI_SubProject)
        {
            return (def);
        }
    }
    return (wxDragNone);
}


//----------------------------------------------------------------
void ProjectTreeCtrlDropTarget::OnLeave()
/**
 * \brief Called when the mouse leaves the drop target.
 **/
{
    // Nothing to do
}

} //namespace penv
