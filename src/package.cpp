/*
 * package.cpp - Implementation of the Package class
 *
 * Author: Steffen Ott
 * Date: 10/01/2007
 *
 */


#include "package.h"
#include "penvhelper.h"
#include "environment.h"

namespace penv {

//----------------------------------------------------------------
Package::Package()
/**
 * \brief Constructor, initializes the package. This
 * constructor is private and only used framework internally.
 **/
{
    m_name = _T("DUMMY PACKAGE");
    m_id = _T("DUMMY_PACKAGE_ID");
    m_initalizationstatus = penvPKG_NOERROR;
    m_properties = new PropertyList();
    m_dependencies = new Array<PackageDependency>();
}


//----------------------------------------------------------------
Package::Package(const wxString& name, const wxString& id, const Version& version)
/**
 * \brief Constructor, initialize the package class. This constructor
 * must be executed by by the inherited class. It is nesessary that every
 * parameter is carefully choosen, because they are needed for other packages.
 * \param name Name of the package (could be everything)
 * \param id Id of the package must be unique.
 * \param version Version of the package.
 **/
{
    m_name = name;
    m_id = id;
    m_initalizationstatus = penvPKG_NOERROR;
    m_properties = new PropertyList();
    m_dependencies = new Array<PackageDependency>();
    m_initalizationstatus = penvPKG_NOERROR;
    m_version = version;
    Environment::Get()->GetPackageList()->Add(this);
}


//----------------------------------------------------------------
Package::~Package()
/**
 * \brief Destructor.
 **/
{
    if (m_properties != NULL) delete m_properties;
    if (m_dependencies != NULL) delete m_dependencies;
}



//----------------------------------------------------------------
wxString Package::GetVersionString() const
/**
 * \brief Returns the version as string for this package.
 * \return Version as string for this package.
 **/
{
    return (m_version.GetVersion());
}




//----------------------------------------------------------------
const Version& Package::GetVersion() const
/**
 * \brief Returns the version for this package.
 * \return Version for this package.
 **/
{
    return (m_version);
}



//----------------------------------------------------------------
const wxString& Package::GetName() const
/**
 * \brief Returns the name of the package. The name of a package
 * must not be unique, but can help a lot.
 * \return Name of the package.
 **/
{
    return (m_name);
}


//----------------------------------------------------------------
const wxString& Package::GetId() const
/**
 * \brief Returns the id of the package, which must be unique.
 * \return Id of this Package.
 */
{
    return (m_id);
}


//----------------------------------------------------------------
PackageDependency* Package::GetDependency(size_t index)
/**
 * \brief Returns a package dependency for this package.
 * \param index Index of the package dependecy.
 * \return Package dependency or NULL.
 **/
{
    if (index < 0 || index >= m_dependencies->Count()) {
        wxLogError(_T("[penv::Package::GetDependency] Index is out of range."));
        return (NULL);
    }
    return ((*m_dependencies)[index]);
}


//----------------------------------------------------------------
bool Package::Add(PackageDependency* dependency)
/**
 * \brief Adds a package dependency to the package. A dependecy
 * must be added, if another package will be needed by this package.
 * \param dependency The package dependency for this package.
 * \return True on success; false otherwise.
 **/
{
    if (dependency == NULL) {
        wxLogError(_T("[penv::Package::Add] Argument 'dependency' is NULL."));
        return (false);
    }
    // Check if dependency already exists, if so safely return success
    for (size_t i=0; i<m_dependencies->Count(); ++i)
    {
        if ((*(*m_dependencies)[i]) == (*dependency)) {
            return (true);
        }
    }
    m_dependencies->Add(dependency);
    return (true);
}


//----------------------------------------------------------------
bool Package::Remove(PackageDependency* dependency)
/**
 * \brief Returns the package dependency form this package.
 * \param dependency Package dependency which should be removed.
 * \return True on success; false otherwise.
 **/
{
    bool result = m_dependencies->Remove(dependency);
    if (!result) wxLogError(_T("[penv::Package::Remove] The given dependency does not exists."));
    return (result);
}


//----------------------------------------------------------------
size_t Package::Count() const
/**
 * \brief Returns the number of dependencies for this package.
 * \return Number of dependencies.
 **/
{
    return (m_dependencies->Count());
}


//----------------------------------------------------------------
bool Package::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'package' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'package' xml element node.
 * \return True on success; false otherwise.
 **/
{
    if (node == NULL) {
        wxLogError(_T("[penv::Package::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("package")) {
        wxLogError(_T("[penv::Package::ReadNode] Node is not a element node with name 'package'."));
        return (false);
    }
    // Just check the nodes and package initalizer and give warning if soething wrong
    wxString id = node->GetPropVal(_T("id"), wxEmptyString);
    wxString name = node->GetPropVal(_T("name"), wxEmptyString);
    wxString version = node->GetPropVal(_T("version"), wxEmptyString);
    if (id != m_id) {
        wxLogWarning(_T("[penv::Package::ReadNode] Initalized package id and package node id are not equal."));
    }
    if (name != m_name) {
        wxLogWarning(_T("[penv::Package::ReadNode] Initalized package name and package node name are not equal."));
    }
    if (m_version != Version(version)) {
        wxLogWarning(_T("[penv::Package::ReadNode] Initalized package version and package node version are not equal."));
    }
    wxXmlNode* child = node->GetChildren();
    while (child != NULL)
    {
        if (child->GetType() == wxXML_ELEMENT_NODE)
        {
            if (child->GetName() == _T("dependencies"))
            {
                // Parse dependencies
                wxXmlNode* dependencynode = child->GetChildren();
                while (dependencynode !=  NULL)
                {
                    if (dependencynode->GetType() == wxXML_ELEMENT_NODE &&
                        dependencynode->GetName() == _T("dependency"))
                    {
                        wxString depid = dependencynode->GetPropVal(_T("id"), wxEmptyString);
                        wxString depver = dependencynode->GetPropVal(_T("version"), wxEmptyString);
                        Version depversion;
                        if (!depversion.SetVersion(depver)) {
                            wxLogWarning(_T("[penv::Package::ReadNode] Cannot parse version from dependency in package '%s'."), m_name.c_str());
                            dependencynode = dependencynode->GetNext();
                            continue;
                        }
                        Add(new PackageDependency(depid, depversion));
                    }
                    dependencynode = dependencynode->GetNext();
                }
            }
            else if (child->GetName() == _T("properties"))
            {
                // Parse properties
                if (!m_properties->ReadNode(child)) {
                    wxLogWarning(_T("[penv::Package::ReadNode] Failed to read 'properties' node from package '%s'."), m_name.c_str());
                }
            }
        }
        child = child->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* Package::WriteNode()
/**
 * \brief Writes a 'package' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'package' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("package"));
    node->AddProperty(_T("id"), m_id);
    node->AddProperty(_T("name"), m_name);
    node->AddProperty(_T("version"), m_version.GetVersion());
    // Create dependencies
    wxXmlNode* depsnode = PenvHelper::CreateXmlNode(_T("dependencies"));
    for (size_t i=0; i<m_dependencies->Count(); ++i)
    {
        wxXmlNode* depnode = PenvHelper::CreateXmlNode(_T("dependency"));
        depnode->AddProperty(_T("id"), (*m_dependencies)[i]->GetId());
        depnode->AddProperty(_T("version"), (*m_dependencies)[i]->GetVersion().GetVersion());
        PenvHelper::AddXmlChildNode(depsnode, depnode);
    }
    PenvHelper::AddXmlChildNode(node, depsnode);
    // Create properties
    wxXmlNode* propnode = m_properties->WriteNode();
    PenvHelper::AddXmlChildNode(node, propnode);
    return (node);
}




} // namespace penv


