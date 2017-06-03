/*
 * project.cpp - Implementation of the Project class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#include "project.h"
#include "expect.h"
#include "penvhelper.h"
#include "projectlist.h"
#include "workspace.h"

namespace penv {

//----------------------------------------------------------------
Project::Project()
    :PenvItem(penvIT_Project)

/**
 * \brief Constructor (private).
 **/
{
}

//----------------------------------------------------------------
Project::Project(ProjectList* parent)
    :PenvItem(penvIT_Project)

/**
 * \brief Constructor, initializes the Project object.
 * \param parent The parent list of this project.
 **/
{
    m_modified = false;
    m_name = wxEmptyString;
    m_properties = new PropertyList();
    m_projectitems = new ProjectItemList(this);
    m_parent = parent;
}


//----------------------------------------------------------------
Project::~Project()
/**
 * \brief Destructor.
 **/
{
    if (m_properties != NULL) delete m_properties;
    if (m_projectitems != NULL) delete m_projectitems;
}


//----------------------------------------------------------------
bool Project::IsModified() const
/**
 * \brief Checks if this project was modified.
 * \return True if project was modified; false otherwise.
 **/
{
    return (m_modified);
}


//----------------------------------------------------------------
void Project::Modified(bool modified)
/**
 * \brief Marks this project as modified.
 * \param modified True if Project should be marked as modified.
 **/
{
    m_modified = modified;
}

//----------------------------------------------------------------
const wxString& Project::GetName() const
/**
 * \brief Returns the name of this project.
 * \return Name of this Project.
 **/
{
    return (m_name);
}



//----------------------------------------------------------------
void Project::SetName(const wxString& name)
/**
 * \brief Sets the name of this project. Automatically
 * marks this project as modified.
 * \param name Name of this project.
 **/
{
    m_name = name;
    m_modified = true;
}


//----------------------------------------------------------------
const wxFileName& Project::GetFileName() const
/**
 * \brief Returns the absolute filename of this project.
 * \return Absoulute filename of the project.
 **/
{
    return (m_filename);
}


//----------------------------------------------------------------
wxString Project::GetFileNameString() const
/**
 * \brief Returns the absolute filename of this project
 * as string.
 * \return Absoulute filename of the project.
 **/
{
    return (m_filename.GetPath());
}

//----------------------------------------------------------------
void Project::SetFileName(const wxFileName& filename)
/**
 * \brief Sets the filename of this project.
 * \param filename Filename of this project.
 **/
{
    m_filename = filename;
}




//----------------------------------------------------------------
void Project::SetFileName(const wxString& filename)
/**
 * \brief Sets the filename of this project.
 * \param filename Filename of this project.
 **/
{
    m_filename.SetPath(filename);
}



//----------------------------------------------------------------
ProjectList* Project::GetParent()
/**
 * \brief Returns the parent container of this project. Could
 * return NULL if not included into a container.
 * \return Parent container of this project.
 **/
{
    return (m_parent);
}

//----------------------------------------------------------------
void Project::SetParent(ProjectList* projectlist)
/**
 * \brief Sets the parent conatiner of this project.
 * \param projectlist Parent container of this project.
 **/
{
    m_parent = projectlist;
}

//----------------------------------------------------------------
const wxTreeItemId& Project::GetTreeItemId() const
/**
 * \brief Returns the tree item id of the treectrl which displays
 * this project.
 * \return Tree item id of the project.
 **/
{
    return (m_treeitem);
}


//----------------------------------------------------------------
void Project::SetTreeItemId(const wxTreeItemId& treeitemid)
/**
 * \brief Sets the tree item id of the treectrl which displays
 * this project.
 * \param treeitemid Tree item id of the project.
 **/
{
    m_treeitem = treeitemid;
}


//----------------------------------------------------------------
ProjectItemList* Project::GetProjectItemList()
/**
 * \brief Retruns the project items in this project.
 * \return Project items in this project.
 **/
{
    return (m_projectitems);
}

//----------------------------------------------------------------
PropertyList* Project::GetPropertyList()
/**
 * \brief Returns the properties of this project.
 * \return Properties of this project.
 **/
{
    return (m_properties);
}

//----------------------------------------------------------------
Project* Project::Clone()
/**
 * \brief Clones this class and returns it.
 * Parent will not be cloned, instead this
 * variable will be NULL.
 * \return Cloned instance of this class.
 **/
{
    Project* project = new Project(NULL);
    project->m_name = m_name;
    project->m_modified = false;
    project->m_properties = m_properties->Clone();
    project->m_projectitems = m_projectitems->Clone();
    project->m_parent = NULL;
    return (project);
}



//----------------------------------------------------------------
bool Project::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'project' xml element node.
 * See class description for more information.
 * \param node The 'project' xml element node.
 * \return True on success; false otherwise.
 **/
{
    if(node == NULL)
    {
        wxLogError(_T("[penv::Project::ReadNode] Node must be not NULL."));
        return(false);
    }
    if(node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("project"))
    {
        wxLogError(_T("[penv::Project::ReadNode] Node should be an element node with name 'project'."));
        return (false);
    }
    if (!node->GetPropVal(_T("name"), &m_name))
    {
        wxLogWarning(_T("[penv::Project::ReadNode] Name for project not spezified, using empty string."));
        m_name = wxEmptyString;
    }
    wxString path = node->GetNodeContent();
    if (!Path::IsAbsolute(path))
    {
        // Relativer Pfad
        wxString wspath = m_parent->GetParent()->GetFileNameString();
        wspath = Path::RemoveLastEntry(wspath);
        path = Path::Combine(wspath, path);
    }
    if (!wxFile::Exists(path))
    {
        wxLogError(_T("[penv::Project::ReadNode] Project filename '%s' does not exist."), path.c_str());
        return (false);
    }
    m_filename.SetPath(path);
    if (!Load())
    {
        wxLogError(_T("[penv::Project::ReadNode] Reading project filename '%s' failed."), path.c_str());
        return (false);
    }
    return (true);
}



//----------------------------------------------------------------
wxXmlNode* Project::WriteNode()
/**
 * \brief Writes a 'project' xml element node.
 * See class description for more information.
 * \return The 'project' xml element node.
 **/
{
    wxString relativepath = Path::MakeRelative(m_parent->GetParent()->GetFileNameString(), m_filename.GetPath());
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("project"), relativepath);
    node->AddProperty(_T("name"), m_name);
    return (node);
}



//----------------------------------------------------------------
bool Project::Save()
/**
 * \brief Saves the appropriate project file.
 * The structure of project file is defined in class destription.
 * \return True on success, otherwise false.
 **/
{
    wxXmlDocument doc;
    doc.SetFileEncoding(_T("UTF-8"));
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("project"));
    doc.SetRoot(node);

    wxXmlNode* nodename = PenvHelper::CreateXmlNode(_T("name"), m_name);
    PenvHelper::AddXmlChildNode(node, nodename);

    wxXmlNode* nodeitems = m_projectitems->WriteNode();
    if(nodeitems == NULL)
    {
        wxLogError(_T("[penv::Project::Save] projectitems: Could not write XML-Structure."));
    }
    PenvHelper::AddXmlChildNode(node, nodeitems);

    wxXmlNode* nodeprops = m_properties->WriteNode();
    if(nodeitems == NULL)
    {
        wxLogError(_T("[penv::Project::Save] projectproperties: Could not write XML-Structure."));
    }
    PenvHelper::AddXmlChildNode(node, nodeprops);

    return (node);
}

//----------------------------------------------------------------
bool Project::Load()
/**
 * \brief Reads out the appropriate project file.
 * The structure of project file is defined in class destription.
 * \return True on success, otherwise false.
 **/
{
    wxString filename = m_filename.GetPath();
    wxXmlDocument doc(filename, _T("UTF-8"));
    if (!doc.IsOk())
    {
        wxLogError(_T("[penv::Project::Load] Could not read file '%s'."), filename.c_str());
        return (false);
    }
    wxXmlNode* root = doc.GetRoot();
    if (root->GetType() != wxXML_ELEMENT_NODE || root->GetName() != _T("project"))
    {
        wxLogError(_T("[penv::Project::Load] Node should be an element node with name 'project'."));
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
            else if (node->GetName() == _T("projectitems"))
            {
                if (!m_projectitems->ReadNode(node))
                {
                    wxLogError(_T("[penv::Project::Load] Failed reading 'projectitems' node from workspace file '%s'."), filename.c_str());
                    node = node->GetNext();
                    continue;
                }
            }
            else if (node->GetName() == _T("properties"))
            {
                if (!m_properties->ReadNode(node))
                {
                    wxLogError(_T("[penv::Project::Load] Failed reading 'properties' node from workspace file '%s'."), filename.c_str());
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


