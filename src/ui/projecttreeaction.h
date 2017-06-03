/*
 * projecttreeaction.h - Declaration of the projecttreeaction class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/27/2008
 *
 */

#ifndef _PENV_UI_PROJECTTREEACTION_H_
#define _PENV_UI_PROJECTTREEACTION_H_

#include "../wxincludes.h"
#include "projecttreeactiontype.h"
#include "../workspace.h"
#include "../project.h"
#include "../projectitem.h"
#include "../projectitemfile.h"
#include "../projectitemdirectory.h"
#include "../projectitemsubproject.h"
#include "../projectitemlinkeditems.h"
#include "projecttreeitemdata.h"
#include "../workspacelist.h"

namespace penv {

// forward declaration
class ProjectTreeCtrl;

/**
 * \brief The ProjectTreeAction class contains various actions
 * to change the tree items.
 * There is a mismatch between UI and normal framework classes,
 * before this is not resolved, you should not use any of this
 * methods.
 **/
class ProjectTreeAction
{
    private:
        ProjectTreeAction();
    public:
        ProjectTreeAction(ProjectTreeCtrl* treectrl);
        virtual ~ProjectTreeAction();

        ProjectTreeActionType GetType();

        size_t CountSelections() const;
        PenvItem* GetSelection(size_t index);

        virtual void OnAdd(WorkspaceList* list);
        virtual void OnAdd(ProjectList* list);
        virtual void OnAdd(ProjectItemList* list);
        virtual void OnAdd(ProjectItemList* list, ProjectItemType type);
        virtual void OnRemove(bool& cancel);
        virtual void OnMove(bool& cancel, Project* item, Workspace* destination, ProjectList* source);
        virtual void OnMove(bool& cancel, ProjectItemSubProject* item, Workspace* destination, ProjectItemList* source);
        virtual void OnMove(bool& cancel, ProjectItem* item, Project* destination, ProjectItemList* source);
        virtual void OnMove(bool& cancel, ProjectItem* item, ProjectItem* destination, ProjectItemList* source);
        virtual void OnCopy(bool& cancel, Project* item, Workspace* destination, ProjectList* source);
        virtual void OnCopy(bool& cancel, ProjectItemSubProject* item, Workspace* destination, ProjectItemList* source);
        virtual void OnCopy(bool& cancel, ProjectItem* item, Project* destination, ProjectItemList* source);
        virtual void OnCopy(bool& cancel, ProjectItem* item, ProjectItem* destination, ProjectItemList* source);
        virtual void OnRename(bool& cancel, const wxString& name);

    protected:
        ProjectTreeCtrl* m_treectrl;
        ProjectTreeActionType m_type;
};

} // namespace penv


#endif // _PENV_UI_PROJECTTREEACTION_H_


