/*
 * packagelist.h - Declaration of the PackageList class
 *
 * Author: Steffen Ott
 * Date: 10/01/2007
 *
 */

#ifndef _PENV_PACKAGELIST_H_
#define _PENV_PACKAGELIST_H_

#include "wxincludes.h"
#include "package.h"


/**
 * \brief This macro initializes a static package. You should use
 * the macro in a implementation file. The given class must be inherited
 * from a package. See Package() class for a detailed description
 * how to create a package.
 **/
#define PENV_INITALIZE_STATIC_PACKAGE(class_name) \
static class_name* initializer_package = new class_name();
// TODO SO: MUSS KRITERIUM Eventuell in eine initialisierungs liste aufnehmen und erst
//     im Application::OnInit() initialisieren, anstatt statisch
//     Eventuell die Erstellte Klasse in ein anderes Array einfügen mit
//     einer Initalisierungs Funktion um die wirkliche Package Klasse
//     dann zu initialisieren. Wegen der Ausgabe von Fehlern, ausserdem flexibler
//     wenn Methoden ausgeführt werden, die noch nicht erlaubt sind.
// class TestPackage_StaticPackageInitializer : public StaticPackageInitializer
// {
//      Package* CreatePackage()
//      {
//         return (new TestPackage());
//      }
// }


namespace penv {

/**
 * \brief The PackageList class is a container for packages. All packages
 * registered in the framework are in this class. Normally you does not
 * need to access this class manually. Also you does not need to add
 * a package or remove it from the list. Adding and removing manually
 * can result in errors or unexpected behaivor, if you don't know what you
 * are doing. To add a package to this class use the macro
 * PENV_INITALIZE_STATIC_PACKAGE().
 * \todo Initialize the package later. With the upper mentioned information.
 * \todo Sort to the correct dependency and detect cyclic depedency.
 * \todo Save package information into the configuration file.
 **/
class PackageList
{
    private:
        friend class Environment;
        friend class Application;
        PackageList();
        PackageList(const PackageList& package);
        ~PackageList();
        PackageList& operator=(const PackageList& package);
        int InitalizePackages();
        int DeinitalizePackages();
        void SortDependencies(Array<Package>& array, bool upward = true);

    public:
        wxArrayString* GetPackages();
        Package* GetItem(const wxString& id);
        bool Add(Package* package);
        bool Remove(const wxString& id);
        void Clear();
        size_t Count();

    private:
        WX_DECLARE_STRING_HASH_MAP(Package*, PackagesHashMap);
        PackagesHashMap* m_hashmap;
};

} // namespace penv


#endif // _PENV_PACKAGELIST_H_
