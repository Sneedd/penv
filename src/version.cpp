/*
 * version.cpp - Implementierung der Version Klasse
 *
 * Author: Steffen Ott
 * Datum: 18.10.2007
 *
 */

#include "version.h"
#include "expect.h"
#include "penvhelper.h"


namespace penv {


//----------------------------------------------------------------
Version::Version()
/**
 * \brief Constructor, initialize the version with 0.0.0.
 **/
{
    m_major = 0;
    m_minor = 0;
    m_build = 0;
    m_unicode = false;
    #if defined(__DEBUG__) || (__WXDEBUG__)
    m_debug = true;
    #else
    m_debug = false;
    #endif
}


//----------------------------------------------------------------
Version::Version(const wxString& versionstring, bool unicode)
/**
 * \brief Constructor, initialize the version with the given
 * version string. Parses the version string with SetVersion().
 * \param versionstring Version string.
 * \param unicode is this build a unicode build. The default
 * value is the build version of the wxWidgets framework.
 **/
{
    // Parsen des Versionsstrings
    SetVersion(versionstring, unicode);
    #if defined(__DEBUG__) || (__WXDEBUG__)
    m_debug = true;
    #else
    m_debug = false;
    #endif
}


//----------------------------------------------------------------
Version::Version(int major, int minor, int build, bool unicode)
/**
 * \brief Constructor, initialize the version with the given
 * version numbers.
 * \param major Major version number.
 * \param minor Minor version number.
 * \param build Build version number.
 * \param unicode is this build a unicode build. The default
 * value is the build version of the wxWidgets framework.
 **/
{
    SetVersion(major, minor, build, unicode);
    #if defined(__DEBUG__) || (__WXDEBUG__)
    m_debug = true;
    #else
    m_debug = false;
    #endif
}


//----------------------------------------------------------------
Version::~Version()
/**
 * \brief Destructor.
 **/
{
    // Nothing to do.
}



//----------------------------------------------------------------
bool Version::operator==(const Version& version)
/**
 * \brief Compares two versions if they are equal. Just compares
 * the version numbers and unicode, it does not compare debug.
 * \param version Version to compare with this object.
 * \return True if the versions are equal; false otherwise.
 **/
{
    return (m_major == version.m_major &&
        m_minor == version.m_minor &&
        m_build == version.m_build);
}



//----------------------------------------------------------------
bool Version::operator!=(const Version& version)
/**
 * \brief Compares two versions if they are unequal. Just compares
 * the version numbers and unicode, it does not compare debug.
 * \param version Version to compare with this object.
 * \return True if the versions are unequal; false otherwise.
 **/
{
    return (!(m_major != version.m_major &&
        m_minor != version.m_minor &&
        m_build != version.m_build));
}


//----------------------------------------------------------------
bool Version::operator<=(const Version& version)
/**
 * \brief Compares two versions if they are smaller or equal.
 * Just compares the version numbers and unicode, it does not
 * compare debug.
 * \param version Version to compare with this object.
 * \return True if the versions are smaller or equal; false otherwise.
 **/
{
    return (!(*this > version));
}


//----------------------------------------------------------------
bool Version::operator>=(const Version& version)
/**
 * \brief Compares two versions if they are bigger or equal.
 * Just compares the version numbers and unicode, it does not
 * compare debug.
 * \param version Version to compare with this object.
 * \return True if the versions are bigger or equal; false otherwise.
 **/
{
    return (!(*this < version));
}


//----------------------------------------------------------------
bool Version::operator<(const Version& version)
/**
 * \brief Compares two versions if they are smaller.
 * Just compares the version numbers and unicode, it does not
 * compare debug.
 * \param version Version to compare with this object.
 * \return True if the versions are smaller; false otherwise.
 **/
{
    if (m_major < version.m_major)
    {
       return(true);
    }
    if(m_major == version.m_major)
    {
        if(m_minor < version.m_minor)
        {
            return(true);
        }
        if(m_minor == version.m_minor)
        {
            if(m_build < version.m_build)
            {
                return (true);
            }
        }
    }
    return (false);
}


//----------------------------------------------------------------
bool Version::operator>(const Version& version)
/**
 * \brief Compares two versions if they are bigger.
 * Just compares the version numbers and unicode, it does not
 * compare debug.
 * \param version Version to compare with this object.
 * \return True if the versions are bigger; false otherwise.
 **/
{
    if (m_major > version.m_major)
    {
        return (true);
    }
    if (m_major == version.m_major)
    {
        if (m_minor > version.m_minor)
        {
            return (true);
        }
        if (m_minor == version.m_minor)
        {
            if(m_build > version.m_build)
            {
                return(true);
            }
        }
    }
    return (false);
}



//----------------------------------------------------------------
void Version::SetVersion(int major, int minor, int build, bool unicode)
/**
 * \brief Sets the version of this object.
 * \param major Major version number.
 * \param minor Minor version number.
 * \param build Build version number.
 * \param unicode is this build a unicode build. The default
 * value is the build version of the wxWidgets framework.
 **/
{
    m_major = major;
    m_minor = minor;
    m_build = build;
    m_unicode = unicode;
}


//----------------------------------------------------------------
bool Version::SetVersion(const wxString& versionstring, bool unicode)
/**
 * \brief Parses a version string like '%a.%b.%c' and writes it
 * into this class. See GetVersion() for more information.
 * \param versionstring Version string.
 * \param unicode is this build a unicode build. The default
 * value is the build version of the wxWidgets framework.
 * \return True on success; false otherwise;
 **/
{
    wxString number;
    int which = 0; // 0 = major, 1 = minor, 2 = build
    bool error = false;
    for (size_t i=0; i<versionstring.Length(); ++i)
    {
        if (versionstring[i] == ' '  || versionstring[i] == '\n' ||
            versionstring[i] == '\r' || versionstring[i] == '\t') {
            continue;
        }
        if (versionstring[i] >= '0' || versionstring[i] <= '9') {
            number += versionstring[i];
        }
        if (versionstring[i] == '.') {
            if (which == 0) {
                error = error && number.ToLong((long*)&m_major);
            } else if (which == 1) {
                error = error && number.ToLong((long*)&m_minor);
            }
            which++;
        }
    }
    if (which == 0) {
        return (false);
    }
    if (which == 1) {
        m_build = 0;
        error = error && number.ToLong((long*)&m_minor);
    }
    if (which == 2) {
        error = error && number.ToLong((long*)&m_build);
    }
    return (error);
}


//----------------------------------------------------------------
wxString Version::GetVersion(const wxString& format) const
/**
 * \brief Returns a version string with the delivered format.
 * The following placeholder are available: <br>
 * <ul>
 * <li> %a = major version number</li>
 * <li> %b = minor version number</li>
 * <li> %c = build version number</li>
 * <li> %u = if unicode build, 'unicode' will be written at this point</li>
 * <li> %d = if debug build, 'debug' will be written at this point</li>
 * </ul>
 * <br>Example:<br>
 * <code>
 * wxString format = version.GetVersion(_T("%a.%b.%c %u %d"));<br>
 * // Possible returns a string like this: "1.0.1018 unicode debug"<br>
 * </code>
 * \param format format of the version string.
 * \return Version string.
 **/
{
    // Parsen des Format Strings
    wxString version;
    for (size_t i=0; i<format.Length(); ++i)
    {
        if (format[i] == '%') {
            i++;
            if (format[i] == 'a') version += wxString::Format(_T("%i"),m_major);
            if (format[i] == 'b') version += wxString::Format(_T("%i"),m_minor);
            if (format[i] == 'c') version += wxString::Format(_T("%i"),m_build);
            if (format[i] == 'u') version += _T("unicode");
            if (format[i] == 'd') version += _T("debug");
        } else {
            version += format[i];
        }
    }
    return (version);
}


//----------------------------------------------------------------
void Version::GetVersion(int& major, int& minor, int& build)
/**
 * \brief Returns the version as integers in a refernce.
 * \param major Major version number.
 * \param minor Minor version number.
 * \param build Build version number.
 **/
{
    major = m_major;
    minor = m_minor;
    build = m_build;
}


//----------------------------------------------------------------
void Version::Unicode(bool isunicode)
/**
 * \brief Sets the versions unicode information.
 * \param isunicode True if this version is unicode.
 **/
{
    m_unicode = isunicode;
}


//----------------------------------------------------------------
bool Version::IsUnicode() const
/**
 * \brief Returns if this version is a unicode build.
 * \return True if this version is a unicode build; false otherwise.
 **/
{
    return (m_unicode);
}


//----------------------------------------------------------------
bool Version::IsDebug() const
/**
 * \brief Returns if this version is a debug build.
 * \return True if this version is a debug build; false otherwise.
 **/
{
    return (m_debug);
}


} // namespace penv



