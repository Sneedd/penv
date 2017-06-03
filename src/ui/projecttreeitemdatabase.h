/*
 * projectreeitemdatabase.h - Declaration of the ProjectTreeItemDataBase class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04/23/2008
 *
 */


#ifndef _PENV_UI_PROJECTTREEITEMDATABASE_H_
#define _PENV_UI_PROJECTTREEITEMDATABASE_H_

#include "../wxincludes.h"
#include "../project.h"
#include "../workspace.h"
#include "../projectitem.h"
#include "projecttreeitemtype.h"


namespace penv {

//Forward declarations
class ProjectTreeItemData;
class ProjectTreeItemDataObject;


/**
 * \brief The ProjectTreeItemDataBase class holds information for a treeitem
 * and implements this information for a data object. The data object will
 * be used to do drag and drop operations on the tree ctrl. The information
 * for the tree item is the appropriate item connected to the tree item.
 * The tree item data will be used for faster access to a workspace, project
 * or project item within the tree ctrl.
 **/
class ProjectTreeItemDataBase
{
    public:
        ProjectTreeItemDataBase();
        ProjectTreeItemDataBase(Project* project);
        ProjectTreeItemDataBase(Workspace* workspace);
        ProjectTreeItemDataBase(ProjectItem* item);
        virtual ~ProjectTreeItemDataBase();

        Project* GetProject();
        void SetProject(Project* project);

        Workspace* GetWorkspace();
        void SetWorkspace(Workspace* workspace);

        ProjectItem* GetProjectItem();
        void SetProjectItem(ProjectItem* item);

        ProjectTreeItemType GetType() const;

        PenvItem* GetPenvItem();

        virtual ProjectTreeItemDataBase* Clone() const;

        void LogClassInfo();

    protected:
        Project* m_project;
        Workspace* m_workspace;
        ProjectItem* m_item;
        ProjectTreeItemType m_type;
};


}//namespace penv


#endif // _PENV_UI_PROJECTTREEITEMDATABASE_H_



