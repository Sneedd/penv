/*
 * workspace.h - Declaration of the Workspace class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/01/2007
 *
 */


#ifndef _PENV_WORKSPACE_H_
#define _PENV_WORKSPACE_H_

#include "projectlist.h"
#include "penvitem.h"

namespace penv {


/**
 * \brief The Workspace class holds every information about a workspace.
 * The workspace can be saved into a file by calling the Save()-Method,
 * where the filename will be used to save into the specific file. <br>
 * There is a PropertyList to store additional informations with the
 * workspace.<br>
 * The definition of the 'workspace' node in a WorkspaceList file:<br>
 * <code>
 * &lt;workspace name="projectname"&gt;path to workspace file&lt;/workspace&gt;<br>
 * </code>
 * The definition of the 'workspace' node in a workspace file:<br>
 * <code>
 * &lt;workspace&gt;<br>
 * &nbsp;&nbsp;&lt;name&gt;workspace name&lt;/name&gt;<br>
 * &nbsp;&nbsp;&lt;projects&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- Projects should be here --&gt;<br>
 * &nbsp;&nbsp;&lt;/projects&gt;<br>
 * &nbsp;&nbsp;&lt;properties&gt;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- See PropertyList class description --&gt;<br>
 * &nbsp;&nbsp;&lt;/properties&gt;<br>
 * &lt;/workspace&gt;<br>
 * </code>
 * \todo Save relative path form workspace list file to workspace file, if possible.
 **/
class Workspace : public PenvItem
{
    public:
        Workspace();
        ~Workspace();

        const wxString& GetName() const;
        void SetName(const wxString& name);

        bool IsModified() const;
        void Modified(bool modified=true);

        const wxFileName& GetFileName() const;
        wxString GetFileNameString() const;
        void SetFileName(const wxFileName& filename);
        void SetFileName(const wxString& filename);

        const wxTreeItemId& GetTreeItemId() const;
        void SetTreeItemId(const wxTreeItemId& treeitemid);

        ProjectList* GetProjectList() const;
        PropertyList* GetPropertyList() const;

        Workspace* Clone();

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

        bool Save();
        bool Load();

    protected:
        wxString m_name;
        wxFileName m_filename;
        bool m_modified;
        ProjectList* m_projects;
        PropertyList* m_properties;
        wxTreeItemId m_treeitem;
};


} // namespace penv

#endif // _PENV_WORKSPACE_H_
