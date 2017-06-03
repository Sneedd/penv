/*
 * workspace.cpp - Implementation of the Workspace class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/01/2007
 *
 */


#include "workspace.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"


namespace penv {

//----------------------------------------------------------------
Workspace::Workspace()
    :PenvItem(penvIT_Workspace)

/**
 * \brief Constructor, initializes the Workspace object.
 **/
{
    m_name = wxEmptyString;
    m_modified = false;
    m_projects = new ProjectList(this);
    m_properties = new PropertyList();
}


//----------------------------------------------------------------
Workspace::~Workspace()
/**
 * \brief Destructor.
 **/
{
    if (m_projects != NULL) delete m_projects;
    if (m_properties != NULL) delete m_properties;
}


//----------------------------------------------------------------
const wxString& Workspace::GetName() const
/**
 * \brief Returns the name of the workspace.
 * \return Name of the workspace.
 **/
{
    return (m_name);
}


//----------------------------------------------------------------
void Workspace::SetName(const wxString& name)
/**
 * \brief Sets the name of the workspace. When setting
 * this item, the workspace will be marked as modified.
 * \param name New name for workspace.
 **/
{
    m_name = name;
    m_modified = true;
}



//----------------------------------------------------------------
bool Workspace::IsModified() const
/**
 * \brief Checks if workspace was modified.
 * \return True if workspace modified; false otherwise.
 **/
{
    return (m_modified);
}


//----------------------------------------------------------------
void Workspace::Modified(bool modified)
/**
 * \brief Sets if workspace was changed.
 * \param modified True if workspace modified; false otherwise.
 **/
{
    m_modified = modified;
}



//----------------------------------------------------------------
const wxFileName& Workspace::GetFileName() const
/**
 * \brief Returns the filename of the workspace file.
 * \return Filename of the workspace.
 **/
{
    return (m_filename);
}


//----------------------------------------------------------------
wxString Workspace::GetFileNameString() const
/**
 * \brief Returns the filename of the workspace as string.
 * \return Filename of the workspace as string.
 **/
{
    return (m_filename.GetPath());
}


//----------------------------------------------------------------
void Workspace::SetFileName(const wxFileName& filename)
/**
 * \brief Sets the filename of the workspace. When setting
 * this item, the workspace will be marked as modified.
 * \param filename Filename of the workspace.
 **/
{
    m_filename = filename;
    m_modified = true;
}


//----------------------------------------------------------------
void Workspace::SetFileName(const wxString& filename)
/**
 * \brief Sets the filename of the workspace. When setting
 * this item, the workspace will be marked as modified.
 * \param filename Filename of the workspace.
 **/
{
    m_filename.SetPath(filename);
    m_modified = true;
}


//----------------------------------------------------------------
const wxTreeItemId& Workspace::GetTreeItemId() const
/**
 * \brief Returns the tree item id of the treectrl who displays
 * this workspace.
 * \return Tree item id of the workspace.
 **/
{
    return (m_treeitem);
}


//----------------------------------------------------------------
void Workspace::SetTreeItemId(const wxTreeItemId& treeitemid)
/**
 * \brief Sets the tree item id of the treectrl who displays
 * this workspace.
 * \param treeitemid Tree item id of the workspace.
 **/
{
    m_treeitem = treeitemid;
}

//----------------------------------------------------------------
ProjectList* Workspace::GetProjectList() const
/**
 * \brief Returns the project list of the workspace who contains the
 * projects.
 * \return Project list with projects in this workspace.
 **/
{
    return (m_projects);
}


//----------------------------------------------------------------
PropertyList* Workspace::GetPropertyList() const
/**
 * \brief Returns the property list for additional configurations
 * to the workspace. If you want that the workspace will be saved,
 * because of changes in the properties, then set the workspace
 * to modified.
 * \return Properties for this workspace.
 **/
{
    return (m_properties);
}

//----------------------------------------------------------------
Workspace* Workspace::Clone()
/**
 * \brief Clones this class and returns it.
 * Filename and treeitem id will not be cloned.
 * \return Cloned instance of this class.
 **/
{
    Workspace* workspace = new Workspace();
    workspace->m_name = m_name;
    workspace->m_modified = false;
    workspace->m_projects = m_projects->Clone();
    workspace->m_properties = m_properties->Clone();
    return (workspace);
}

//----------------------------------------------------------------
bool Workspace::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'workspace' xml element node.
 * See class description for more information.
 * \param node The 'workspace' xml element node.
 * \return True on success; false otherwise.
 **/
{
    if (node == NULL)
    {
        wxLogError(_T("[penv::Workspace::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("workspace"))
    {
        wxLogError(_T("[penv::Workspace::ReadNode] Node should be an element node with name 'workspace'."));
        return (false);
    }
    wxString path;
    if (!node->GetPropVal(_T("name"), &m_name))
    {
        wxLogWarning(_T("[penv::Workspace::ReadNode] Name for workspace not spezified, using empty string."));
        m_name = wxEmptyString;
    }
    path = node->GetNodeContent();
    wxString workspacepath = Path::RemoveLastEntry(Environment::Get()->GetWorkspacesFile(false));
    path = Path::Combine(workspacepath, path);
    if (!wxFile::Exists(path))
    {
        wxLogError(_T("[penv::Workspace::ReadNode] Workspace filename '%s' does not exist."), path.c_str());
        return (false);
    }
    m_filename.SetPath(path);
    if (!Load())
    {
        wxLogError(_T("[penv::Workspace::ReadNode] Reading workspace filename '%s' failed."), path.c_str());
        return (false);
    }
    return (true);
}



//----------------------------------------------------------------
wxXmlNode* Workspace::WriteNode()
/**
 * \brief Writes a 'workspace' xml element node.
 * See class description for more information.
 * \return The 'workspace' xml element node.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("workspace"), m_filename.GetPath());
    node->AddProperty(_T("name"), m_name);
    return node;
}



//----------------------------------------------------------------
bool Workspace::Save()
/**
 * \brief Saves the appropriate workspace file.
 * The structure of workspace file is defined in class destription.
 * \return True on success, otherwise false.
 **/
{
    wxXmlDocument doc;
    doc.SetFileEncoding(_T("UTF-8"));
    wxXmlNode* rootnode = PenvHelper::CreateXmlNode(_T("workspace"));
    doc.SetRoot(rootnode);
    wxXmlNode * nodename = PenvHelper::CreateXmlNode(_T("name"), m_name);
    PenvHelper::AddXmlChildNode(rootnode, nodename);

    wxXmlNode* projects = m_projects->WriteNode();
    if(projects == NULL)
    {
        wxLogError(_T("[penv::Workspace::Save] Could not save workspace file '%s'."), m_filename.GetPath().c_str());
        return (false);
    }
    PenvHelper::AddXmlChildNode(rootnode, projects);

    wxXmlNode* properties = m_properties->WriteNode();
    if(properties == NULL)
    {
        wxLogError(_T("[penv::Workspace::Save] Could not save workspace file '%s'."), m_filename.GetPath().c_str());
        return (false);
    }
    PenvHelper::AddXmlChildNode(rootnode, properties);
    return (true);
}


//----------------------------------------------------------------
bool Workspace::Load()
/**
 * \brief Reads out the appropriate workspace file.
 * The structure of workspace file is defined in class destription.
 * \return True on success, otherwise false.
 **/
{
    wxString filename = m_filename.GetPath();
    wxXmlDocument doc(filename, _T("UTF-8"));
    if (!doc.IsOk())
    {
        wxLogError(_T("[penv::Workspace::ReadXmlDocument] Could not read file '%s'."), filename.c_str());
        return (false);
    }
    wxXmlNode* root = doc.GetRoot();
    if (root->GetType() != wxXML_ELEMENT_NODE || root->GetName() != _T("workspace"))
    {
        wxLogError(_T("[penv::Workspace::ReadXmlDocument] Node should be an element node with name 'workspace'."));
        return (false);
    }
    wxXmlNode* node = root->GetChildren();
    while (node != NULL)
    {
        if (node->GetType() ==  wxXML_ELEMENT_NODE)
        {
            if (node->GetName() == _T("name"))
            {
                wxString name = node->GetNodeContent();
                if (!name.IsEmpty())
                {
                    m_name = name;
                }
            }
            else if (node->GetName() == _T("projects"))
            {
                if (!m_projects->ReadNode(node))
                {
                    wxLogError(_T("[penv::Workspace::ReadXmlDocument] Failed reading 'projects' node from workspace file '%s'."), filename.c_str());
                    node = node->GetNext();
                    continue;
                }
            }
            else if (node->GetName() == _T("properties"))
            {
                if (!m_properties->ReadNode(node))
                {
                    wxLogError(_T("[penv::Workspace::ReadXmlDocument] Failed reading 'properties' node from workspace file '%s'."), filename.c_str());
                    node = node->GetNext();
                    continue;
                }
            }
        }
        node = node->GetNext();
    }
    return (true);
}





} // namespace penv


