/*
 * projectreeitemdata.h - Declaration of the ProjectTreeItemData class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/29/2008
 *
 */



#ifndef _PENV_UI_PROJECTTREEITEMDATA_H_
#define _PENV_UI_PROJECTTREEITEMDATA_H_

#include "../wxincludes.h"
#include "../project.h"
#include "../workspace.h"
#include "../projectitem.h"
#include "projecttreeitemtype.h"
#include "projecttreeitemdatabase.h"

namespace penv {


/**
 * \brief The ProjectTreeItemData class holds information for a treeitem.
 * The information for the tree item is the appropriate item connected
 * to the tree item. The tree item data will be used for faster access to
 * a workspace, project or project item within the tree ctrl.
 **/
class ProjectTreeItemData : public ProjectTreeItemDataBase, public wxTreeItemData
{
    public:
        ProjectTreeItemData();
        ProjectTreeItemData(Project* project);
        ProjectTreeItemData(Workspace* workspace);
        ProjectTreeItemData(ProjectItem* item);
        virtual ~ProjectTreeItemData();

    protected:


};

} // namespace penv

#endif // _PENV_UI_PROJECTTREEITEMDATA_H_
