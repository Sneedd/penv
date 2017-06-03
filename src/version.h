/*
 * version.h - Deklaration der Version Klasse
 *
 * Author: Steffen Ott
 * Datum: 18.10.2007
 *
 */


#ifndef _PENV_VERSION_H_
#define _PENV_VERSION_H_

#include "wxincludes.h"

namespace penv {

/**
 * \brief The Version class contains version information, which
 * will be used for version checks within the framework. You
 * can also use this class for versioning you application and
 * packages. You can use SetVersion() to parse an version string
 * or GetVersion() to get the version as a string. All comparison
 * operators are implemented to compare the version of this
 * object with another.
 **/
class Version
{
    public:
        Version();
        #ifdef UNICODE
        Version(const wxString& versionstring, bool unicode=true);
        Version(int major, int minor, int build, bool unicode=true);
        #else
        Version(const wxString& versionstring, bool unicode=false);
        Version(int major, int minor, int build, bool unicode=false);
        #endif
        ~Version();

        bool operator==(const Version& version);
        bool operator!=(const Version& version);
        bool operator<=(const Version& version);
        bool operator>=(const Version& version);
        bool operator<(const Version& version);
        bool operator>(const Version& version);

        #ifdef UNICODE
        void SetVersion(int major, int minor, int build, bool unicode=true);
        bool SetVersion(const wxString& versionstring, bool unicode=true);
        #else
        void SetVersion(int major, int minor, int build, bool unicode=false);
        bool SetVersion(const wxString& versionstring, bool unicode=false);
        #endif
        wxString GetVersion(const wxString& format=_T("%a.%b.%c %u %d")) const;
        void GetVersion(int& major, int& minor, int& build);

        void Unicode(bool isunicode=true);
        bool IsUnicode() const;

        bool IsDebug() const;

    private:
        int m_major;
        int m_minor;
        int m_build;
        bool m_unicode;
        bool m_debug;
};

} // namespace penv

#endif // _PENV_VERSION_H_
