/*
 * projectitem.cpp - Implementation of the ProjectItemSubProject class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#include "projectitemsubproject.h"
#include "expect.h"
#include "penvhelper.h"


namespace penv {



//----------------------------------------------------------------
ProjectItemSubProject::ProjectItemSubProject()
 :  Project(NULL), ProjectItem(NULL)
/**
 * \brief Constructor (private).
 **/
{
}



//----------------------------------------------------------------
ProjectItemSubProject::ProjectItemSubProject(ProjectItemList* parent)
    : Project(NULL), ProjectItem(parent)
/**
 * \brief Constructor, initializes the ProjectItemSubProject object.
 * \param parent The parent container for this project item.
 **/
{
    m_type = penvPI_SubProject;
    Project::m_penvitemtype = penvIT_ProjectItemSubProject;
    ProjectItem::m_penvitemtype = penvIT_ProjectItemSubProject;
}



//----------------------------------------------------------------
ProjectItemSubProject::~ProjectItemSubProject()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
ProjectItemList* ProjectItemSubProject::GetItemList()
/**
 * \brief Returns the project items in this subproject item.
 * \return Project items in this subproject.
 **/
{
    return (Project::GetProjectItemList());
}



//----------------------------------------------------------------
bool ProjectItemSubProject::Save()
/**
 * \brief Saves this project item. Because this is a subproject,
 * there is nothing to be saved, because the underlying project saves
 * all informations about this project, therefore this method
 * will always return true.
 * \return True.
 **/
{
    return (true);
}



//----------------------------------------------------------------
bool ProjectItemSubProject::Load()
/**
 * \brief Loads this project item. Because this is a subproject,
 * there is nothing to be loaded, because the underlying project loads
 * all informations about this project, therefore this method
 * will always return true.
 * \return True.
 **/
{
    return (true);
}



//----------------------------------------------------------------
ProjectItem* ProjectItemSubProject::Clone()
/**
 * \brief Clones this project item and all its underlying
 * project items. The parent will not be cloned, instead
 * this will always be null.
 * \return Return the cloned project item.
 **/
{
    ProjectItemSubProject* item = new ProjectItemSubProject(NULL);
    item->Project::m_name = Project::m_name;
    item->m_type = m_type;
    item->m_windowid = wxEmptyString;
    item->m_windowtype = m_windowtype;
    item->m_virtual = m_virtual;
    item->Project::m_properties = Project::m_properties->Clone();
    item->Project::m_parent = NULL;
    item->ProjectItem::m_parent = NULL;
    item->Project::m_projectitems = Project::m_projectitems->Clone();
    item->Project::m_projectitems->SetProjectItemParent(item);
    return (item);
}



//----------------------------------------------------------------
bool ProjectItemSubProject::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'projectitem' xml element node.
 * See class description for more information.
 * \param node The 'projectitem' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Knoten muss vom Typ Element sein
    if (node == NULL)
    {
        wxLogError(_T("[penv::ProjectItemSubProject::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("projectitem"))
    {
        wxLogError(_T("[penv::ProjectItemSubProject::ReadNode] Node should be an element node with name 'projectitem'."));
        return (false);
    }
    wxString type = node->GetPropVal(_T("type"), wxEmptyString);
    if (type != _T("subproject"))
    {
        wxLogError(_T("[penv::ProjectItemSubProject::ReadNode] ProjectItem must be ProjectItemFile."));
        return (false);
    }
    Project::m_name = node->GetPropVal(_T("name"),wxEmptyString);
    if(Project::m_name.IsEmpty())
    {
       wxLogWarning(_T("[penv::ProjectItemSubProject::ReadNode] ProjectItem is nameless"));
    }
    m_virtual = PenvHelper::ParseBoolean(node->GetPropVal(_T("virtual"), _T("false")));
    // Durch alle knoten unterhalb von 'projectitems' loopen
    wxXmlNode* innernode = node->GetChildren();
    while (innernode != NULL)
    {
        if (innernode->GetType() == wxXML_ELEMENT_NODE &&
            innernode->GetName() == _T("project"))
        {
            return(ReadProjectNode(innernode));
        }
        innernode = innernode->GetNext();
    }
    wxLogError(_T("[penv::ProjectItemSubProject::ReadNode] Node 'project' in ProjectItem SubProject not found."));
    return (false);
}


//----------------------------------------------------------------
bool ProjectItemSubProject::ReadProjectNode(wxXmlNode* root)
/**
 * \brief Read out a 'project' xml element node.
 * See class description for more information.
 * \param root The 'project' xml element node.
 * \return True on success; false otherwise.
 **/
{
    if (root->GetType() != wxXML_ELEMENT_NODE || root->GetName() != _T("project"))
    {
        wxLogError(_T("[penv::ProjectItemSubProject::ReadProjectNode] Node should be an element node with name 'project'."));
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
                    Project::m_name = name;
                }
            }
            else if (node->GetName() == _T("projectitems"))
            {
                if (!Project::m_projectitems->ReadNode(node))
                {
                    wxLogError(_T("[penv::ProjectItemSubProject::ReadProjectNode] Failed reading 'projectitems' node from project."));
                    node = node->GetNext();
                    continue;
                }
            }
            else if (node->GetName() == _T("properties"))
            {
                if (!Project::m_properties->ReadNode(node))
                {
                    wxLogError(_T("[penv::ProjectItemSubProject::ReadProjectNode] Failed reading 'properties' node from project."));
                    node = node->GetNext();
                    continue;
                }
            }
        }
        node = node->GetNext();
    }
    return (true);
}



//----------------------------------------------------------------
wxXmlNode* ProjectItemSubProject::WriteNode()
/**
 * \brief Writes a 'projectitem' xml element node.
 * See class description for more information.
 * \return The 'projectitem' xml element node.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("projectitem"));
    node->AddProperty(_T("name"), Project::m_name);
    node->AddProperty(_T("type"), _T("subproject"));
    node->AddProperty(_T("virtual"), PenvHelper::CreateBoolean(m_virtual));

    wxXmlNode* projectnode = WriteProjectNode();
    PenvHelper::AddXmlChildNode(node, projectnode);

    return(node);
}



//----------------------------------------------------------------
wxXmlNode* ProjectItemSubProject::WriteProjectNode()
/**
 * \brief Writes a 'project' xml element node.
 * See class description for more information.
 * \return The 'project' xml element node.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("project"));

    wxXmlNode* nodename = PenvHelper::CreateXmlNode(_T("name"), Project::m_name);
    PenvHelper::AddXmlChildNode(node, nodename);

    wxXmlNode* nodeitems = Project::m_projectitems->WriteNode();
    if(nodeitems == NULL)
    {
        wxLogError(_T("[penv::Project::Save] projectitems: Could not write XML-Structure."));
    }
    PenvHelper::AddXmlChildNode(node, nodeitems);

    wxXmlNode* nodeprops = Project::m_properties->WriteNode();
    if(nodeitems == NULL)
    {
        wxLogError(_T("[penv::Project::Save] projectproperties: Could not write XML-Structure."));
    }
    PenvHelper::AddXmlChildNode(node, nodeprops);

    return (node);
}


} // namespace penv

