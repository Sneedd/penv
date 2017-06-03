/*
 * projectitemlinkeditems.h - Declaration of the ProjectItemLinkedItems class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#ifndef _PENV_PROJECTITEMLINKEDITEMS_H_
#define _PENV_PROJECTITEMLINKEDITEMS_H_

#include "wxincludes.h"
#include "projectitem.h"
#include "container/objarray.h"
#include "penvhelper.h"

namespace penv {

/**
 * \brief The ProjectItemLinkedItems class represents a group of project
 * items with one main item. This class is inherited from ProjectItem and
 * therefore if contains all attributes from ProjectItem, like the
 * PropertyList and so on. A ProjectItemList is included to handle the
 * project items. The xml definition of a projectitem element is slightly
 * different from the other project items:<br>
 * <code>
 * &lt;projectitem name="project item name" type="linkeditem" virtual="false"&gt;<br>
 * &nbsp;&nbsp;&lt;mainitem&gt;name of the main item&lt;/path&gt;<br>
 * &nbsp;&nbsp;&lt;!-- Project items should be here plus the main item --&gt;<br>
 * &nbsp;&nbsp;&lt;properties&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- See PropertyList class description --&gt;<br>
 * &nbsp;&nbsp;&lt;/properties&gt;<br>
 * &lt;/projectitem&gt;<br>
 * </code>
 **/
class ProjectItemLinkedItems : public ProjectItem
{
    private:
        friend class ProjectItemList;
        ProjectItemLinkedItems();
        DECLARE_NO_COPY_CONSTRUCTOR(ProjectItemLinkedItems);
        DECLARE_NO_ASSIGNMENT_OPERATOR(ProjectItemLinkedItems);
    public:
        ProjectItemLinkedItems(ProjectItemList* parent);
        virtual ~ProjectItemLinkedItems();

        virtual ProjectItemList* GetItemList();
        ProjectItem* GetMainItem();

        virtual bool Save();
        virtual bool Load();
        virtual ProjectItem* Clone();
        virtual bool ReadNode(wxXmlNode* node);
        virtual wxXmlNode* WriteNode();

    private:
        ProjectItemList* m_innerlist;
};

} // namespace penv


#endif // _PENV_PROJECTITEMLINKEDITEMS_H_
