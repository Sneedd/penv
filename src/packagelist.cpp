/*
 * packagelist.cpp - Implementation of the PackageList class
 *
 * Author: Steffen Ott
 * Date: 10/01/2007
 *
 */

#include "packagelist.h"
#include "expect.h"
#include "penvhelper.h"

namespace penv {


//----------------------------------------------------------------
PackageList::PackageList()
/**
 * \brief Constructor, initialize the PackageList object. This constructor
 * is only used framework internally.
 **/
{
    m_hashmap = new PackagesHashMap();
}


//----------------------------------------------------------------
PackageList::PackageList(const PackageList& package)
/**
 * \brief Copyconstructor (private).
 **/
{
    // Not used.
}


//----------------------------------------------------------------
PackageList::~PackageList()
/**
 * \brief Destructor.
 **/
{
    Clear();
    if (m_hashmap != NULL) delete m_hashmap;
}



//----------------------------------------------------------------
PackageList& PackageList::operator=(const PackageList& package)
/**
 * \brief Assignment operator (private).
 **/
{
    if (&package == this) return (*this);
    return (*this);
}


//----------------------------------------------------------------
int PackageList::InitalizePackages()
/**
 * \brief Initializes all packages registered in this class. The packages
 * will be initialized upward after their dependencies. On initialization of each
 * package the OnInitPackage() method will be called.
 * \return Error code.
 **/
{
    if (unlikely(m_hashmap->size() <= 0)) {
        wxLogError(_T("[penv::PackageList::InitalizePackages] No packages registered to the environment."));
        return (penvPKG_FATALERROR);
    }
    // Iterate through hashmap and add them to the array
    PackagesHashMap::iterator itr;
    Array<Package> array(false);
    array.Alloc(m_hashmap->size());
    for(itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        array.Add(itr->second);
    }

    // Sort packages after the dependencies
    SortDependencies(array);

    // Iterate through array and call their appropriate initalize methods
    int errorcode = penvPKG_NOERROR;
    for (size_t i=0; i<array.Count(); ++i)
    {
        int actualerrorcode = array[i]->OnInitPackage();
        array[i]->m_initalizationstatus = actualerrorcode;
        if (errorcode > actualerrorcode) errorcode = actualerrorcode;
    }
    return (errorcode);
}


//----------------------------------------------------------------
int PackageList::DeinitalizePackages()
/**
 * \brief Deinitialize all packages registered in this class. The packages
 * will be deinitialized downward after their dependencies. On deinitialization
 * of each package the OnExitPackage() method will be called.
 * \return Error code.
 **/
{
    // Iterate through hashmap and add them to the array
    PackagesHashMap::iterator itr;
    Array<Package> array(false);
    array.Alloc(m_hashmap->size());
    for(itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        array.Add(itr->second);
    }

    // Sort packages after the dependencies
    SortDependencies(array, false);

    // Iterate through array and call their appropriate deinitalize methods
    int errorcode = penvPKG_NOERROR;
    for (size_t i=0; i<array.Count(); ++i)
    {
        int actualerrorcode = array[i]->OnExitPackage();
        if (errorcode > actualerrorcode) errorcode = actualerrorcode;
    }
    return (errorcode);
}


//----------------------------------------------------------------
void PackageList::SortDependencies(Array<Package>& array, bool upward)
/**
 * \brief Sort the packages after their depedencies.
 * \param array The array to sort.
 * \param upward True to sort the minimal depedencies to the top;
 * false to sort the maximal depedencies to the top.
 **/
{
    // TODO
    NOT_IMPLEMENTED_YET();
    return;
}


//----------------------------------------------------------------
wxArrayString* PackageList::GetPackages()
/**
 * \brief Returns all package ids in a array. You must destroy
 * the returned array after using it.
 * \return Array with package ids.
 **/
{
    wxArrayString* array = new wxArrayString(m_hashmap->size());
    for (PackagesHashMap::iterator itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        array->Add(itr->first);
    }
    return (array);
}



//----------------------------------------------------------------
Package* PackageList::GetItem(const wxString& id)
/**
 * \brief Returns the package with the given package id.
 * \param id Package id of wanted package.
 * \return The package if found; NULL otherwise.
 **/
{
    PackagesHashMap::iterator itr = m_hashmap->find(id);
    if (unlikely(itr == m_hashmap->end())) {
        return (NULL);
    }
    return (itr->second);
}


//----------------------------------------------------------------
bool PackageList::Add(Package* package)
/**
 * \brief Adds a package to the end of this class.
 * \param package Adding package.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(package == NULL)) {
        wxLogError(_T("[penv::PackageList::Add] Argument 'package' is NULL."));
        return (false);
    }
    // No empty package id allowed
    if (unlikely(package->GetId().IsEmpty())) {
        wxLogError(_T("[penv::PackageList::Add] Cannot add a package with empty id."));
        return (false);
    }
    wxString id = package->GetId();
    // Checks if package id already exists
    PackagesHashMap::iterator itr = m_hashmap->find(package->m_id);
    if (unlikely(itr != m_hashmap->end())) {
        wxLogError(_T("[penv::PackageList::Add] A package with the id '%s' already exists."), package->GetId().c_str());
        return (false);
    }
    // Add package
    (*m_hashmap)[package->GetId()] = package;
    return (true);
}


//----------------------------------------------------------------
bool PackageList::Remove(const wxString& id)
/**
 * \brief Removes the package with the given id from this class.
 * \param id The package id of the package which should be removed.
 * \return True on success; false otherwise.
 **/
{
    return (m_hashmap->erase(id) == 1);
}



//----------------------------------------------------------------
void PackageList::Clear()
/**
 * \brief Removes all packages from this class.
 **/
{
    for (PackagesHashMap::iterator itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        Package* package = itr->second;
        if (likely(package != NULL)) delete package;
        itr->second = NULL;
    }
    m_hashmap->clear();
}





//----------------------------------------------------------------
size_t PackageList::Count()
/**
 * \brief Returns the number of packages registered in this class.
 * \return Number of registered packages.
 **/
{
    return (m_hashmap->size());
}



} // namespace penv


