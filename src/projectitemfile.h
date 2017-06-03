/*
 * projectitemfile.h - Declaration of the ProjectItemFile class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#ifndef _PENV_PROJECTITEMFILE_H_
#define _PENV_PROJECTITEMFILE_H_

#include "wxincludes.h"
#include "projectitem.h"
#include "penvhelper.h"

namespace penv {

/**
 * \brief The ProjectItemFile class represents a file as a
 * project item. This class is inherited from ProjectItem and therefore
 * if contains all attributes from ProjectItem, like the PropertyList and
 * so on. The xml definition of a ProjectItemFile element is slightly
 * different from the other project items:<br>
 * <code>
 * &lt;projectitem name="project items name" type="file" virtual="false"&gt;<br>
 * &nbsp;&nbsp;&lt;filename&gt;path to the filename&lt;/filename&gt;<br>
 * &nbsp;&nbsp;&lt;properties&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- See PropertyList class description --&gt;<br>
 * &nbsp;&nbsp;&lt;/properties&gt;<br>
 * &lt;/projectitem&gt;<br>
 * </code>
 **/
class ProjectItemFile : public ProjectItem
{
    private:
        friend class ProjectItemList;
        ProjectItemFile();
        DECLARE_NO_COPY_CONSTRUCTOR(ProjectItemFile);
        DECLARE_NO_ASSIGNMENT_OPERATOR(ProjectItemFile);
    public:
        ProjectItemFile(ProjectItemList* parent);
        virtual ~ProjectItemFile();

        virtual wxString GetPathString() const;
        virtual wxFileName GetPath() const;
        virtual ProjectItemList* GetItemList();

        virtual bool Save();
        virtual bool Load();
        virtual ProjectItem* Clone();
        virtual bool ReadNode(wxXmlNode* node);
        virtual wxXmlNode* WriteNode();

    private:
        wxFileName m_filename;
};

} // namespace penv


#endif // _PENV_PROJECTITEMFILE_H_
