/*
 * packagedependency.h - Deklaration of the PackageDependency class
 *
 * Author: Steffen Ott
 * Date: 04/06/2007
 *
 */

#ifndef _PENV_PACKAGEDEPENDENCY_H_
#define _PENV_PACKAGEDEPENDENCY_H_


#include "wxincludes.h"
#include "version.h"

namespace penv {


/**
 * \brief The PackageDependency class contains information about a
 * package dependecy. The package dependency can be added to a package
 * which needs another package. This is used for doing the correkt
 * initialisation of the packages or not initialize a package because
 * it's dependencies are not met.
 **/
class PackageDependency
{
    public:
        PackageDependency();
        PackageDependency(const wxString& packageid, const Version& version);
        ~PackageDependency();

        bool operator==(const PackageDependency& object);
        bool operator!=(const PackageDependency& object);
        bool operator<=(const PackageDependency& object);
        bool operator>=(const PackageDependency& object);
        bool operator<(const PackageDependency& object);
        bool operator>(const PackageDependency& object);

        const wxString& GetId() const;
        void SetId(const wxString& id);

        const Version& GetVersion() const;
        void SetVersion(const Version& version);

    private:
        Version m_version;
        wxString m_packageid;

};

} // namespace penv

#endif // _PENV_PACKAGEDEPENDENCY_H_
