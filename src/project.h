/*
 * project.h - Declaration of the Project class
 *
 * Author: Steffen Ott, Michael Schwierz
 * date: 02.10.2007
 *
 */

#ifndef _PENV_PROJECT_H_
#define _PENV_PROJECT_H_

#include "wxincludes.h"
#include "propertylist.h"
#include "projectitemlist.h"
#include "penvitem.h"

namespace penv {

/**
 * \brief The Project class represents a project in the framework.
 * Like a workspace the project must be saved into a own file. The file
 * can be saved or loaded with Save() or Load(). The ReadNode() and
 * WriteNode() methods are for inserting the paths and names into a
 * workspace file. So we have to xml node definitions for a project.<br>
 * The definition of the 'project' node in a workspace file:<br>
 * <code>
 * &lt;project name="projectname"&gt;relative path to project file&lt;/project&gt;<br>
 * </code>
 * The definition of the 'project' node in a project file:<br>
 * <code>
 * &lt;project&gt;<br>
 * &nbsp;&nbsp;&lt;name&gt;projectname&lt;/name&gt;<br>
 * &nbsp;&nbsp;&lt;projectitems&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- Project items should be here --&gt;<br>
 * &nbsp;&nbsp;&lt;/projectitems&gt;<br>
 * &nbsp;&nbsp;&lt;properties&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- See PropertyList class description --&gt;<br>
 * &nbsp;&nbsp;&lt;/properties&gt;<br>
 * &lt;/project&gt;<br>
 * </code>
 * \todo Make Project standard constructor private.
 **/
class Project : public PenvItem
{
    public:
        friend class ProjectList;
        Project();
    public:
        Project(ProjectList* parent);
        ~Project();

        bool IsModified() const;
        void Modified(bool modified = true);

        const wxString& GetName() const;
        void SetName(const wxString& name);

        const wxFileName& GetFileName() const;
        wxString GetFileNameString() const;
        void SetFileName(const wxFileName& filename);
        void SetFileName(const wxString& filename);

        ProjectList* GetParent();
        void SetParent(ProjectList* projectlist);

        const wxTreeItemId& GetTreeItemId() const;
        void SetTreeItemId(const wxTreeItemId& treeitemid);

        ProjectItemList* GetProjectItemList();
        PropertyList* GetPropertyList();

        Project* Clone();
        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

        bool Save();
        bool Load();

    protected:
        bool m_modified;
        wxString m_name;
        wxFileName m_filename;
        PropertyList* m_properties;
        ProjectItemList* m_projectitems;
        wxTreeItemId m_treeitem;
        ProjectList* m_parent;
};

} // namespace penv

#endif // _PENV_PROJECT_H_


