/*
 * path.cpp - Implementation of the Path class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04/29/2008
 *
 */



#include "path.h"
#include "penvhelper.h"


namespace penv {



//----------------------------------------------------------------
Path::Path()
/**
 * \brief Constructor (private).
 **/
{
}

//----------------------------------------------------------------
Path::~Path()
/**
 * \brief Destructor (private).
 **/
{
}




//----------------------------------------------------------------
bool Path::IsAbsolute(const wxString& path)
/**
 * \brief Checks if the given path is absoulute.
 * \param path Path.
 * \return True if path is absolute; false otherwise.
 **/
{
    #if defined(__WINDOWS__) || (__WXWINE__)
        // Windows basierte Betriebssysteme
        if (path.IsEmpty()) return (false);
        if (path.Length() > 1)
        {
            if (path[1] == ':') return (true);
        }
        return (false);
    #elif defined(__LINUX__ ) || (__UNIX__) || (__UNIX_LIKE__) || (__SOLARIS__)
        // Linux, Unix basierte Betriebssysteme
        if (path.IsEmpty()) return (false);
        if (path[0] == '/') return (true);
        return (false);
    #elif
        #warning Operatingsystem not supported
    #endif
}



//----------------------------------------------------------------
wxString Path::Combine(const wxString& path, const wxString& appendpath)
/**
 * \brief Combines to paths to one. The method supports the '..', '\', '/'
 * and '.' spezial characters in a relative path which should be appended
 * to the path.
 * \param path The main path, should be absolute.
 * \param appendpath The path with spezial charakters, which should be
 * appended.
 * \return Combined path.
 **/
{
    // TODO BUG:
//    path = _T("C:\\windows\\system32\\conf");
//    append = _T("..");
//    wxLogMessage(PenvHelper::PathCombine(path, append));

    wxArrayString strgarray_append;
    PenvHelper::SplitString(Normalize(appendpath), &strgarray_append, wxFileName::GetPathSeparator());
    if (strgarray_append.Count() == 1) {
        if (strgarray_append[0] == _T(".")) {
            return (Normalize(path));
        }
        else if (strgarray_append[0] == _T("..")) {
            return (Normalize(RemoveLastEntry(path)));
        }
        else
        {
            return (Normalize(path + wxFileName::GetPathSeparator() + appendpath));
        }
    }
    wxArrayString strgarray_path;
    PenvHelper::SplitString(Normalize(path), &strgarray_path, wxFileName::GetPathSeparator());
    size_t index_append = 0;
    size_t index_path = strgarray_path.Count();
    for (size_t i=0; i<strgarray_append.Count(); ++i)
    {
        if (strgarray_append[i] == _T(".")) {
            index_append = i;
        }
        else if (strgarray_append[i] == _T("..")) {
            index_append = i;
            index_path--;
        }
        else {
            index_append = i;
            break;
        }
    }
    if (index_path < 0) return (wxEmptyString);
    wxString result;
    for (size_t i=0; i<index_path; ++i) {
        result.Append(strgarray_path[i]).Append(wxFileName::GetPathSeparator());
    }
    for (size_t i=index_append; i<strgarray_append.Count(); ++i) {
        result.Append(strgarray_append[i]).Append(wxFileName::GetPathSeparator());
    }
    if (result.Last() == '/' || result.Last() == '\\') {
        return (result.Mid(0, result.Length()-1));
    }
    return (result);
}


//----------------------------------------------------------------
wxString Path::MakeRelative(const wxString& referencepath, const wxString& absolutepath)
/**
 * \brief Creates a relative path from two absoulte paths. The reference
 * path will be needed to convert the absolutepath into a relativ path.
 * This method is the opposite of PathCombine().
 * \param referencepath The referenced path for the relative path.
 * \param absolutepath The absolute path from which the relative path should be created.
 * \return The relative path.
 **/
{
    wxArrayString strgarray_ref;
    PenvHelper::SplitString(Normalize(referencepath), &strgarray_ref, wxFileName::GetPathSeparator());
    wxArrayString strgarray_abs;
    PenvHelper::SplitString(Normalize(absolutepath), &strgarray_abs, wxFileName::GetPathSeparator());
    int index = 0;
    for (size_t i=0; i<strgarray_ref.Count() && i<strgarray_abs.Count(); ++i)
    {
        if (strgarray_ref[i] == strgarray_abs[i]) {
            index++;
        } else {
            break;
        }
    }
    wxString result;
    for (size_t i=index; i<strgarray_ref.Count(); ++i) {
        result.Append(_T("..")).Append(wxFileName::GetPathSeparator());
    }
    for (size_t i=index; i<strgarray_abs.Count(); ++i) {
        result.Append(strgarray_abs[i]).Append(wxFileName::GetPathSeparator());
    }
    if (result.Last() == '/' || result.Last() == '\\') {
        return (result.Mid(0, result.Length()-1));
    }
    return (result);
}



//----------------------------------------------------------------
wxString Path::RemoveLastEntry(const wxString& path)
/**
 * \brief Removes the last entry from a path. The last entry
 * can be a filename or directory. The given path will be normalized
 * before the path will be removed.
 * \param path Path where last entry should be removed.
 * \return Path with removed entry.
 **/
{
    wxString normed = Normalize(path);
    int index = normed.Find(wxFileName::GetPathSeparator(), true);
    if (index == -1) return (_T(""));
    return (normed.SubString(0, (size_t)(index-1)));
}


//----------------------------------------------------------------
wxString Path::GetLastEntry(const wxString& path)
/**
 * \brief Returns the last entry of a path. This could be a
 * directory name or a filename with extension.
 * \param path The path from which the last entry should be returned.
 * \return The last entry of a path.
 **/
{
    wxString normed = Normalize(path);
    int index = normed.Find(wxFileName::GetPathSeparator(), true);
    return (normed.Mid((size_t)(index+1)));
}


//----------------------------------------------------------------
wxString Path::Normalize(const wxString& path)
/**
 * \brief Normalizes the given path. This means that all
 * path separators will be changed to the operating system
 * standard separator. <br>
 * Example: "C:\/windows//system32\\\config/" will be converted
 * to "C:\windows\system32\config".
 * \param path Path to normalize.
 * \return Normalized path.
 **/
{
    if (path.IsEmpty()) {
        return (path);
    }
    // Pfad separators exchange
    wxString normed;
    bool state = true;
    for (size_t i=0; i<path.Length(); ++i)
    {
        if (path[i] == '/' || path[i] == '\\') {
            if (state) {
                state = false;
                normed.Append(wxFileName::GetPathSeparator());
            }
        } else {
            state = true;
            normed.Append(path[i]);
        }
    }
    // Remove last path separator
    if (normed.Last() == '/' || normed.Last() == '\\') {
        return (normed.Mid(0, normed.Length()-1));
    }
    return (normed);
}


//----------------------------------------------------------------
bool Path::Create(const wxString& path)
/**
 * \brief Creates the given absolute path. This method fails
 * if this path is not a absolute path or path cannot be
 * created.
 * \param path Absolute path to create.
 * \return True on success; false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (true);
}




} // namespace penv
