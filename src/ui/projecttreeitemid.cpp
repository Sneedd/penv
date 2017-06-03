/*
 * projecttreeitemid.h - Declaration of the Projecttreeitemid class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/29/2008
 *
 */

#include "projecttreeitemid.h"
#include "../environment.h"

namespace penv{
//----------------------------------------------------------------
ProjectTreeItemId::ProjectTreeItemId()
/**
 * \brief Constructor initializes the ProjectTreeItemId class
 **/
{
}

//----------------------------------------------------------------
ProjectTreeItemId::ProjectTreeItemId(wxTreeItemId id)
/**
 * \brief Constructor initializes the ProjectTreeItemId class
 **/
{
    m_id = id;
}


//----------------------------------------------------------------
ProjectTreeItemId::~ProjectTreeItemId()
/**
 * \brief Destructor
 **/
{
}


//----------------------------------------------------------------
void ProjectTreeItemId::SetData(ProjectTreeItemData* data)
/**
 * \brief Sets the ProjectTreeItemData.
 * The data will not be destroyed when a new ItemData is set.
 * \param data The ProjectTreeItemData.
 **/
{
    GetTreeCtrl()->SetItemData(m_id, data);
}


//----------------------------------------------------------------
ProjectTreeItemData* ProjectTreeItemId::GetData() const
/**
 * \brief Returns ProjectTreeItemData.
 * \return ProjectTreeItemData.
 **/
{
    return ((ProjectTreeItemData*)GetTreeCtrl()->GetItemData(m_id));
}


//----------------------------------------------------------------
void ProjectTreeItemId::SetId(wxTreeItemId id)
/**
 * \brief Sets the inner TreeItemId of the TreeItem.
 * \param id The TreeItemId.
 **/
{
    m_id = id;
}


//----------------------------------------------------------------
wxTreeItemId ProjectTreeItemId::GetId() const
/**
 * \brief Returns the inner TreeItemId of the TreeItem.
 * \return The inner TreeItemId of the TreeItem.
 **/
{
    return (m_id);
}


//----------------------------------------------------------------
bool ProjectTreeItemId::IsOk() const
/**
 * \brief Returns true if the Item is a valid item.
 * \return true if the item is valid, false otherwise.
 **/
{
    return (m_id.IsOk());
}


//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeItemId::GetNext()
/**
 * \brief Returns the next item in project tree.
 * \return next item in project tree.
 **/
{
    return (ProjectTreeItemId(GetTreeCtrl()->GetNextSibling(m_id)));
}


//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeItemId::GetPrev()
/**
 * \brief Returns the previous item in project tree.
 * \return previous item in project tree.
 **/
{
    return (ProjectTreeItemId(GetTreeCtrl()->GetPrevSibling(m_id)));

}


//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeItemId::GetParent()
/**
 * \brief Returns the parent item in project tree.
 * \return parent item in project tree.
 **/
{
    return (ProjectTreeItemId(GetTreeCtrl()->GetItemParent(m_id)));
}


//----------------------------------------------------------------
ProjectTreeItemId ProjectTreeItemId::GetFirstChild()
/**
 * \brief Returns the first child item in project tree.
 * \return first child item in project tree.
 **/
{
    unsigned int cookievalue = 0xC00C1FC;
    wxTreeItemIdValue cookie = (wxTreeItemIdValue)cookievalue;
    wxTreeCtrl* treectrl = GetTreeCtrl();
    ProjectTreeItemId id(treectrl->GetFirstChild(m_id,cookie));
    return (id);
}


//----------------------------------------------------------------
bool ProjectTreeItemId::HasParent() const
/**
 * \brief Returns true if the Item has a parent item.
 * \return true if the item has a parent item, false otherwise.
 **/
{
    return (GetTreeCtrl()->GetItemParent(m_id).IsOk());
}


//----------------------------------------------------------------
bool ProjectTreeItemId::HasChild() const
/**
 * \brief Returns true if the Item has a child item.
 * \return true if the item has a child item, false otherwise.
 **/
{
    return (GetTreeCtrl()->GetChildrenCount(m_id, false) != 0);
}


//----------------------------------------------------------------
bool ProjectTreeItemId::GetItemBold() const
/**
 * \brief Returns true if the item is bold.
 * \return true if the item item is bold, false otherwise.
 **/
{
    return (GetTreeCtrl()->IsBold(m_id) == true);
}


//----------------------------------------------------------------
void ProjectTreeItemId::SetItemBold(bool bold)
/**
 * \brief Sets the tree item bold.
 * \param bold If the item should be bold = true.
 **/
{
    GetTreeCtrl()->SetItemBold(m_id,bold);
}


//----------------------------------------------------------------
wxString ProjectTreeItemId::GetItemText() const
/**
 * \brief Returns the item text for the tree item.
 * \return Text of the item.
 **/
{
    return GetTreeCtrl()->GetItemText(m_id);
}


//----------------------------------------------------------------
void ProjectTreeItemId::SetItemTextColor(const wxColour& color)
/**
 * \brief Sets the item text color for the tree item.
 * \param color of the item.
 **/
{
    GetTreeCtrl()->SetItemTextColour(m_id, color);
}


//----------------------------------------------------------------
wxColor ProjectTreeItemId::GetItemTextColor() const
/**
 * \brief Returns the item text color for the tree item.
 * \return color of the item.
 **/
{
    return (GetTreeCtrl()->GetItemTextColour(m_id));
}


//----------------------------------------------------------------
bool ProjectTreeItemId::IsSelected() const
/**
 * \brief Returns if the item is selected.
 * \return true if item is selected, false otherwise.
 **/
{
    return (GetTreeCtrl()->IsSelected(m_id));
}


//----------------------------------------------------------------
void ProjectTreeItemId::SetSelection(ProjectTreeItemSelectionMode mode)
/**
 * \brief Sets the mode for the selection.
 * \param mode The mode of the selection.
 **/
{
    if(mode == penvSM_Toggle)
    {
        wxTreeCtrl* treectrl = GetTreeCtrl();
        bool selected = treectrl->IsSelected(m_id);
        treectrl->SelectItem(m_id, !selected);
    }
    else if(mode == penvSM_SelectSingle)
    {
        wxTreeCtrl* treectrl = GetTreeCtrl();
        treectrl->UnselectAll();
        treectrl->SelectItem(m_id, true);
    }
    else if(mode == penvSM_Unselect)
    {
        wxTreeCtrl* treectrl = GetTreeCtrl();
        treectrl->SelectItem(m_id, false);
    }
    else if(mode == penvSM_SelectMutiple)
    {
        NOT_IMPLEMENTED_YET();
    }
}


//----------------------------------------------------------------
wxTreeCtrl* ProjectTreeItemId::GetTreeCtrl() const
/**
 * \brief Returns the inner TreeCtrl of ItemTreeCtrl.
 * \return The inner TreeCtrl of ItemTreeCtrl
 **/
{
    return (Environment::Get()->GetFrame()->GetProjectTreeCtrl()->GetTreeCtrl());
}


}//namespace penv

