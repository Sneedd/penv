/*
 * projectitemsubproject.h - Declaration of the ProjectItemSubProject class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#ifndef _PENV_PROJECTITEMSUBPROJECT_H_
#define _PENV_PROJECTITEMSUBPROJECT_H_

#include "wxincludes.h"
#include "projectitem.h"
#include "project.h"
#include "penvhelper.h"

namespace penv {

/**
 * \brief The ProjectItemSubProject class represents a subproject as a
 * project item. This class is inherited from ProjectItem and Project, therefore
 * it contains all attributes from ProjectItem and Project, like the PropertyList and
 * so on. The xml definition of a projectitem element is slightly different from
 * the other project items:<br>
 * <code>
 * &lt;projectitem name="project item name" type="subproject" virtual="false"&gt;<br>
 * &nbsp;&nbsp;&lt;project&gt<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;projectitems&gt<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- Project items should be here --&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;/projectitems&gt<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;properties&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- See PropertyList class description --&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;/properties&gt;<br>
 * &nbsp;&nbsp;&lt;/project&gt;
 * &lt;/projectitem&gt;<br>
 * </code>
 **/
class ProjectItemSubProject : public Project, public ProjectItem
{
     private:
        friend class ProjectItemList;
        ProjectItemSubProject();
        DECLARE_NO_COPY_CONSTRUCTOR(ProjectItemSubProject);
        DECLARE_NO_ASSIGNMENT_OPERATOR(ProjectItemSubProject);
    public:
        ProjectItemSubProject(ProjectItemList* parent);
        virtual ~ProjectItemSubProject();

        virtual ProjectItemList* GetItemList();

        virtual bool Save();
        virtual bool Load();
        virtual ProjectItem* Clone();
        virtual bool ReadNode(wxXmlNode* node);
        bool ReadProjectNode(wxXmlNode* root);
        virtual wxXmlNode* WriteNode();
        wxXmlNode* WriteProjectNode();

    private:

};

} // namespace penv


#endif // _PENV_PROJECTITEMSUBPROJECT_H_
