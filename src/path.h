/*
 * path.h - Declaration of the Path class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04/29/2008
 *
 */


#ifndef _PENV_PATH_H_
#define _PENV_PATH_H_

#include "wxincludes.h"


namespace penv {

/**
 * \brief The Path class gives support for handling paths.
 **/
class Path
{
    private:
        Path();
        ~Path();
    public:
        static bool IsAbsolute(const wxString& path);
        static wxString Combine(const wxString& path, const wxString& appendpath);
        static wxString MakeRelative(const wxString& referencepath, const wxString& absolutepath);
        static wxString RemoveLastEntry(const wxString& path);
        static wxString GetLastEntry(const wxString& path);
        static wxString Normalize(const wxString& path);
        static bool Create(const wxString& path);


};

} // namespace penv

#endif // _PENV_PATH_H_
