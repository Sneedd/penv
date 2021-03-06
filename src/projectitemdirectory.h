/*
 * projectitemdirectory.h - Declaration of the ProjectItemDirectory class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#ifndef _PENV_PROJECTITEMDIRECTORY_H_
#define _PENV_PROJECTITEMDIRECTORY_H_

#include "wxincludes.h"
#include "projectitem.h"
#include "container/objarray.h"
#include "penvhelper.h"

namespace penv {

/**
 * \brief The ProjectItemDirectory class represents a directory as a
 * project item. This class is inherited from ProjectItem and therefore
 * if contains all attributes from ProjectItem, like the PropertyList and
 * so on. A ProjectItemList is included to handle the project items. The
 * xml definition of a projectitem element is slightly different from
 * the other project items:<br>
 * <code>
 * &lt;projectitem name="project items name" type="directory" virtual="false"&gt;<br>
 * &nbsp;&nbsp;&lt;path&gt;path to the directory&lt;/path&gt;<br>
 * &nbsp;&nbsp;&lt;!-- Project items should be here --&gt;<br>
 * &nbsp;&nbsp;&lt;properties&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- See PropertyList class description --&gt;<br>
 * &nbsp;&nbsp;&lt;/properties&gt;<br>
 * &lt;/projectitem&gt;<br>
 * </code>
 **/
class ProjectItemDirectory : public ProjectItem
{
    private:
        friend class ProjectItemList;
        ProjectItemDirectory();
        DECLARE_NO_COPY_CONSTRUCTOR(ProjectItemDirectory);
        DECLARE_NO_ASSIGNMENT_OPERATOR(ProjectItemDirectory);

    public:
        ProjectItemDirectory(ProjectItemList* parent);
        virtual ~ProjectItemDirectory();


        virtual wxString GetPathString() const;
        virtual wxFileName GetPath() const;

        virtual ProjectItemList* GetItemList();

        virtual bool Save();
        virtual bool Load();
        virtual ProjectItem* Clone();
        virtual bool ReadNode(wxXmlNode* node);
        virtual wxXmlNode* WriteNode();

    protected:
        wxFileName m_directory;
        ProjectItemList* m_innerlist;

};

} // namespace penv

#endif // _PENV_PROJECTITEMDIRECTORY_H_
