/*
 * projecttreeitemid.h - Declaration of the Projecttreeitemid class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/29/2008
 *
 */

#ifndef _PENV_UI_PROJECTTREEITEMID_H_
#define _PENV_UI_PROJECTTREEITEMID_H_

#include "../wxincludes.h"
#include "projecttreeitemdata.h"
#include "projecttreeitemselectionmode.h"

namespace penv{


/**
 * \brief The ProjectTreeItemId class is for accessing easier
 * the TreeItems in the ProjectTreeCtrl class. This class will
 * be returned when searching for a item or other actions, which
 * result in getting a tree item.
 **/
class ProjectTreeItemId
{
    public:
        ProjectTreeItemId();
        virtual ~ProjectTreeItemId();
        ProjectTreeItemId(wxTreeItemId id);
        void SetData(ProjectTreeItemData* data);
        ProjectTreeItemData* GetData() const;

        void SetId(wxTreeItemId id);
        wxTreeItemId GetId() const;

        bool IsOk() const;
        ProjectTreeItemId GetNext();
        ProjectTreeItemId GetPrev();
        ProjectTreeItemId GetParent();
        ProjectTreeItemId GetFirstChild();

        bool HasParent() const;
        bool HasChild() const;

        bool GetItemBold() const;
        void SetItemBold(bool bold = true);

        wxString GetItemText() const;

        void SetItemTextColor(const wxColour& color);
        wxColor GetItemTextColor() const;

        bool IsSelected() const;
        void SetSelection(ProjectTreeItemSelectionMode mode);

    private:
        wxTreeCtrl* GetTreeCtrl() const;

    private:
        wxTreeItemId m_id;
};
}//namespace penv

#endif // _PENV_UI_PROJECTTREEITEMID_H_
