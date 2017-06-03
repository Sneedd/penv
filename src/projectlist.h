/*
 * projectlist.h - Declaration of the ProjectList class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/01/2007
 *
 */

#ifndef _PENV_PROJECTLIST_H_
#define _PENV_PROJECTLIST_H_

#include "project.h"
#include "container/objarray.h"


namespace penv {

/**
 * \brief The ProjectList class is a container for projects. Main functionality is
 * adding, removing, copying and moving projects.<br>
 * The class can be represented in a xml file. The definition of a 'projects' xml
 * element:<br>
 * <code>
 *  &lt;projects&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- project elements should be here --&gt;<br>
 *  &lt;/projects&gt;<br>
 * </code>
 **/
class ProjectList
{
    private:
        friend class Workspace;
        ProjectList();
        ProjectList(Workspace* parent);
        ~ProjectList();

    public:
        Project* GetItem(size_t index);
        int GetItemIndex(Project* project);
        bool Add(Project* prj);
        bool Remove(size_t index);
        void Clear();
        size_t Count();

        Workspace* GetParent();
        void SetParent(Workspace* workspace);

        bool MoveProject(size_t index, Workspace* dest);
        bool CopyProject(size_t index, Workspace* dest);

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();
        ProjectList* Clone();

    private:
        Array<Project>* m_array;
        Workspace* m_parent;
};

} // namespace penv

#endif // _PENV_PROJECTLIST_H_
