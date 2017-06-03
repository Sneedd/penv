/*
 * projecttreectrldroptarget.h - Declaration of the ProjectTreeCtrlDropTarget class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 06/03/2008
 *
 */


#ifndef _PENV_UI_PROJECTTREECTRLDROPTARGET_H_
#define _PENV_UI_PROJECTTREECTRLDROPTARGET_H_

#include "../wxincludes.h"

namespace penv {

// Forward declaration
class ProjectTreeCtrl;

/**
 * \brief The ProjectTreeCtrlDropTarget class is the target for
 * a drag and drop operations in the ProjectTreeCtrl.
 **/
class ProjectTreeCtrlDropTarget : public wxDropTarget
{
    public:
        ProjectTreeCtrlDropTarget(ProjectTreeCtrl* treecontrol, wxDataObject* data = NULL);
        virtual ~ProjectTreeCtrlDropTarget();

        virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);
        virtual bool OnDrop(wxCoord x, wxCoord y);
        virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def);
        virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def);
        virtual void OnLeave();

    protected:
        wxDragResult m_dragresult;
        ProjectTreeCtrl* m_treectrl;
};

}//namespace penv

#endif // _PENV_UI_PROJECTTREECTRLDROPTARGET_H_
