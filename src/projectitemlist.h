/*
 * projectitemlist.h - Declaration of the ProjectItemList class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04.11.2007
 *
 *
 */


#ifndef _PENV_PROJECTITEMLIST_H_
#define _PENV_PROJECTITEMLIST_H_

#include "wxincludes.h"
#include "container/objarray.h"
#include "projectitemtype.h"
#include "penvhelper.h"

namespace penv {


/**
 * \brief The ProjectItemList is a container for ProjectItem's.
 * The ProjectItemList is used in a Project, ProjectItemDirectory
 * and ProjectItemLinkedItems.<br>
 * Here the definition of a 'projectitems' xml element:<br>
 * <code>
 *  &lt;projectitems&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- projectitem elements should be here --&gt;<br>
 *  &lt;/projectitems&gt;<br>
 * </code>
 **/
class ProjectItemList
{
    private:
        friend class Project;
        friend class ProjectItem;
        ProjectItemList();
        DECLARE_NO_COPY_CONSTRUCTOR(ProjectItemList);
        DECLARE_NO_ASSIGNMENT_OPERATOR(ProjectItemList);
    public:
        ProjectItemList(Project* parent);
        ProjectItemList(ProjectItem* parent);
        ~ProjectItemList();

        ProjectItem* GetItem(size_t index);
        int GetItemIndex(ProjectItem* item);
        bool Add(ProjectItem* prjitem);
        bool Remove(size_t index);
        void Clear();
        size_t Count();

        Project* GetProjectParent();
        void SetProjectParent(Project* project);

        ProjectItem* GetProjectItemParent();
        void SetProjectItemParent(ProjectItem* projectitem);

        bool MoveProjectItem(size_t index, Project* dest);
        bool CopyProjectItem(size_t index, Project* dest);
        bool MoveProjectItem(size_t index, ProjectItem* dest);
        bool CopyProjectItem(size_t index, ProjectItem* dest);

        ProjectItemList* Clone();
        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

        ProjectItem* CreateProjectItem(const wxString& type);
        ProjectItem* CreateProjectItem(ProjectItemType type);
    private:
        void Modified(bool modified = true);

    private:
        ProjectItem* m_projectitemparent;
        Project* m_projectparent;
        Array<ProjectItem>* m_array;
};

} // namespace penv

#endif // _PENV_PROJECTITEMLIST_H_
