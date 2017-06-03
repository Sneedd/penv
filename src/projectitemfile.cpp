/*
 * projectitemfile.cpp - Implementation of the ProjectItem class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#include "projectitemfile.h"
#include "expect.h"
#include "penvhelper.h"
#include "projectitemlist.h"
#include "project.h"
#include "environment.h"


namespace penv {


//----------------------------------------------------------------
ProjectItemFile::ProjectItemFile()
    : ProjectItem(NULL)
/**
 * \brief Constructor (private).
 **/
{
    wxLogError(_T("aaaaaarrrgh"));
    wxASSERT(false);
}


//----------------------------------------------------------------
ProjectItemFile::ProjectItemFile(ProjectItemList* parent)
    : ProjectItem(parent)
/**
 * \brief Constructor, initializes the ProjectItemFile object.
 * \param parent The parent ProjectItemList who contains this item.
 **/
{
    m_type = penvPI_File;
    m_windowtype = Environment::Get()->GetDefaultDocument();
    m_penvitemtype = penvIT_ProjectItemFile;
}



//----------------------------------------------------------------
ProjectItemFile::~ProjectItemFile()
/**
 * \brief Destructor.
 **/
{
}



//----------------------------------------------------------------
wxString ProjectItemFile::GetPathString() const
/**
 * \brief Returns the path of this item as an string.
 * \return Full path to item as wxString.
 **/
{
    return (m_filename.GetPath());
}




//----------------------------------------------------------------
wxFileName ProjectItemFile::GetPath() const
/**
 * \brief Returns the path of this item.
 * \return Full path to item as wxFileName.
 **/
{
    return (m_filename);
}


//----------------------------------------------------------------
ProjectItemList* ProjectItemFile::GetItemList()
/**
 * \brief This method always returns NULL for ProjectItemFile.
 * \return Return NULL.
 **/
{
    return (NULL);
}


//----------------------------------------------------------------
bool ProjectItemFile::Save()
/**
 * \brief Saves this project item. Because this is a file,
 * a save action will check if the project item is connected
 * to a window and then saves the projectitem. Because no
 * saving code exists in this class.
 * \return True on succcess; false otherwise.
 **/
{
    if (m_virtual) {
        // TODO
        wxVariant variant(_T("TODO"));
        Environment::Get()->GetCommandList()->Execute(_T("PENV_EVT_SAVE_VIRTUAL_PROJECTITEMFILE"), variant);
        return (true);
    }
    // Wenn kein Fenster zu ändern der Datei geöffnet wurde,
    // kann die Datei auch nicht verändert worden sein.
    if (unlikely(m_windowid.IsEmpty()))
    {
        wxLogWarning(_T("[penv::ProjectItemFile::Save] NO window connected to projectitemfile."));
        return (false);
    }
    // Window Pointer aus WindowList holen
    Environment* env = Environment::Get();
    Window* window = env->GetWindowList()->GetWindow(m_windowid);
    if (unlikely(window == NULL)) {
        wxLogWarning(_T("[penv::ProjectItemFile::Save] Window is not open for project item \"%s\". So project item is not modified."),m_name.c_str());
        return (false);
    }
    // Speichern aufrufen
    ProjectItemFile* item = (ProjectItemFile*)this;
    return (window->Save(item->GetPathString()));
}



//----------------------------------------------------------------
bool ProjectItemFile::Load()
/**
 * \brief Loads this project item. Because this is a file,
 * it will load the project item via the load method of the
 * registered window class. If the project item is already open
 * then the load method will reload the information from the file.
 * \return True on success; false otherwise.
 **/
{
    // No window type then abort
    if (m_windowtype.IsEmpty()) {
        return (false);
    }
    // Find window type in windowlist
    WindowList* list = Environment::Get()->GetWindowList();
    if (!list->IsRegistered(m_windowtype)) {
        wxLogError(_T("[penv::ProjectItemFile::Load] The window class id '%s' in project item file '%s' is not registered in the environment."), m_windowtype.c_str(), m_name.c_str());
        return (false);
    }
    // Check if window must be created
    if (m_windowid.IsEmpty() || !list->ExistsWindow(m_windowid)) {
        // Create the window
        Window* window = list->CreateWindow(m_windowtype, this);
        list->Add(window, true);
        m_windowid = window->GetId();
        if (!window->Load(m_filename.GetPath())) {
            wxLogError(_T("[penv::ProjectItemFile::Load] Error occured while loading file '%s'."), m_filename.GetPath().c_str());
            return (false);
        }
    } else {
        // Bring the window to the front
        list->ShowWindow(m_windowid, true);
    }
    return (true);
}



//----------------------------------------------------------------
ProjectItem* ProjectItemFile::Clone()
/**
 * \brief Clones this project item. The parent will not
 * be cloned, instead this will always be null.
 * \return Return the cloned project item.
 **/
{
    ProjectItemFile* item = new ProjectItemFile(NULL);
    item->m_name = m_name;
    item->m_type = m_type;
    item->m_windowid = wxEmptyString;
    item->m_windowtype = m_windowtype;
    item->m_virtual = m_virtual;
    item->m_properties = m_properties->Clone();
    item->m_parent = NULL;
    return (item);
}

//----------------------------------------------------------------
bool ProjectItemFile::ReadNode(wxXmlNode* node)
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
        wxLogError(_T("[penv::ProjectItemFile::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("projectitem"))
    {
        wxLogError(_T("[penv::ProjectItemFile::ReadNode] Node should be an element node with name 'projectitem'."));
        return (false);
    }
    wxString type = node->GetPropVal(_T("type"), wxEmptyString);
    if (type != _T("file"))
    {
        wxLogError(_T("[penv::ProjectItemFile::ReadNode] ProjectItem must be ProjectItemFile."));
        return (false);
    }
    m_name = node->GetPropVal(_T("name"),wxEmptyString);
    if(m_name.IsEmpty())
    {
       wxLogWarning(_T("[penv::ProjectItemFile::ReadNode] ProjectItem is nameless"));
    }
    m_virtual = PenvHelper::ParseBoolean(node->GetPropVal(_T("virtual"), _T("false")));
    // Durch alle knoten unterhalb von 'projectitems' loopen
    wxXmlNode* innernode = node->GetChildren();
    while (innernode != NULL)
    {
        if (innernode->GetType() == wxXML_ELEMENT_NODE)
        {
            if (innernode->GetName() == _T("filename"))
            {
                wxString path = innernode->GetNodeContent();
                if (!Path::IsAbsolute(path))
                {
                    // Relativer Pfad
                    wxString wspath;
                    if (m_parent->GetProjectParent() != NULL) {
                        wspath = m_parent->GetProjectParent()->GetFileNameString();
                        wspath = Path::RemoveLastEntry(wspath);
                    } else {
                        wspath = m_parent->GetProjectItemParent()->GetPathString();
                    }
                    path = Path::Combine(wspath, path);
                }
                m_filename.SetPath(path);
            }
            else if (innernode->GetName() == _T("properties"))
            {
                if (!m_properties->ReadNode(innernode))
                {
                    wxLogError(_T("[penv::ProjectItemFile::ReadNode] Cannot read properties in ProjectItemFile."));
                }
            }
        }
        innernode = innernode->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* ProjectItemFile::WriteNode()
/**
 * \brief Writes a 'projectitem' xml element node.
 * See class description for more information.
 * \return The 'projectitem' xml element node.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("projectitem"));
    node->AddProperty(_T("name"), m_name);
    node->AddProperty(_T("type"), _T("file"));
    node->AddProperty(_T("virtual"), PenvHelper::CreateBoolean(m_virtual));

    wxString parentpath;
    if (m_parent->GetProjectParent() == NULL) {
        ProjectItem* item = m_parent->GetProjectItemParent();
        parentpath = item->GetPathString();
    } else {
        Project* project = m_parent->GetProjectParent();
        parentpath = project->GetFileNameString();
    }
    wxXmlNode* filename = PenvHelper::CreateXmlNode(_T("filename"),
        Path::MakeRelative(parentpath, m_filename.GetPath()));
    PenvHelper::AddXmlChildNode(node, filename);

    wxXmlNode* propnode = m_properties->WriteNode();
    PenvHelper::AddXmlChildNode(node, propnode);

    return (node);
}

} // namespace penv

