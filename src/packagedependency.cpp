/*
 * packagedependency.cpp - Implementation of the PackageDependency class
 *
 * Author: Steffen Ott
 * Date: 04/06/2007
 *
 */

#include "packagedependency.h"

namespace penv {


//----------------------------------------------------------------
PackageDependency::PackageDependency()
/**
 * \brief Constructor, initializes the PackageDependency object.
 **/
{
}


//----------------------------------------------------------------
PackageDependency::PackageDependency(const wxString& packageid, const Version& version)
/**
 * \brief Constructor, initializes the PackageDependency object.
 * \param packageid The package id of the dependency.
 * \param version The package version of the dependency.
 **/
{
    m_packageid = packageid;
    m_version = version;
}


//----------------------------------------------------------------
PackageDependency::~PackageDependency()
/**
 * \brief Destructor.
 **/
{
}



//----------------------------------------------------------------
bool PackageDependency::operator==(const PackageDependency& object)
/**
 * \brief Compares two dependencies if they are equal.
 * \param object Dependency to compare with this dependency.
 * \return True if the dependencies are equal; false otherwise.
 **/
{
    if (m_packageid == object.m_packageid) {
        return (m_version == object.m_version);
    }
    return (false);
}


//----------------------------------------------------------------
bool PackageDependency::operator!=(const PackageDependency& object)
/**
 * \brief Compares two dependencies if they are unequal.
 * \param object Dependency to compare with this dependency.
 * \return True if the dependencies are unequal; false otherwise.
 **/
{
    if (m_packageid != object.m_packageid) {
        return (m_version != object.m_version);
    }
    return (false);
}


//----------------------------------------------------------------
bool PackageDependency::operator<=(const PackageDependency& object)
/**
 * \brief Compares two dependencies if they are smaller or equal.
 * \param object Dependency to compare with this dependency.
 * \return True if the dependencies are smaller or equal; false otherwise.
 **/
{
    if (m_packageid == object.m_packageid) {
        return (m_version <= object.m_version);
    }
    return (false);
}


//----------------------------------------------------------------
bool PackageDependency::operator>=(const PackageDependency& object)
/**
 * \brief Compares two dependencies if they are bigger or equal.
 * \param object Dependency to compare with this dependency.
 * \return True if the dependencies are bigger or equal; false otherwise.
 **/
{
    if (m_packageid == object.m_packageid) {
        return (m_version >= object.m_version);
    }
    return (false);
}


//----------------------------------------------------------------
bool PackageDependency::operator<(const PackageDependency& object)
/**
 * \brief Compares two dependencies if they are smaller.
 * \param object Dependency to compare with this dependency.
 * \return True if the dependencies are smaller; false otherwise.
 **/
{
    if (m_packageid == object.m_packageid) {
        return (m_version < object.m_version);
    }
    return (false);
}


//----------------------------------------------------------------
bool PackageDependency::operator>(const PackageDependency& object)
/**
 * \brief Compares two dependencies if they are bigger.
 * \param object Dependency to compare with this dependency.
 * \return True if the dependencies are bigger; false otherwise.
 **/
{
    if (m_packageid == object.m_packageid) {
        return (m_version > object.m_version);
    }
    return (false);
}



//----------------------------------------------------------------
const wxString& PackageDependency::GetId() const
/**
 * \brief Returns the package id of the dependency.
 * \return Package id of the dependency.
 **/
{
    return (m_packageid);
}


//----------------------------------------------------------------
void PackageDependency::SetId(const wxString& id)
/**
 * \brief Sets the package id of the dependency.
 * \param id Package id of the dependency.
 **/
{
    m_packageid = id;
}



//----------------------------------------------------------------
const Version& PackageDependency::GetVersion() const
/**
 * \brief Returns the package version of the dependency.
 * \return Package version of the dependency.
 **/
{
    return (m_version);
}


//----------------------------------------------------------------
void PackageDependency::SetVersion(const Version& version)
/**
 * \brief Sets the package version of the dependency.
 * \param version Package version of the dependency.
 **/
{
    m_version = version;
}


} // namespace penv


