/*
 * penvhelper.h - Declaration of the PenvHelper class
 *
 * Author: Steffen Ott
 * Date: 10/02/2007
 *
 */


#ifndef _PENV_PENVHELPER_H_
#define _PENV_PENVHELPER_H_

#include "wxincludes.h"
#include "expect.h"
#include "globals.h"
#include "path.h"


/**
 * \brief The NOT_IMPLEMENTED_YET() macro just writes an information
 * to the log file, that something is not implemented.
 **/
#define NOT_IMPLEMENTED_YET() \
    wxLogError(_T("Not implemented yet. File: %s Line: %i"), _T(__FILE__), __LINE__);

/**
 * \brief Declares a private copy constructor in a class.
 * After using this macro make sure that you switch to public
 * or whatever.
 **/
#define DECLARE_NO_COPY_CONSTRUCTOR(classname) \
    private: \
    classname(classname& source) {}

/**
 * \brief Declares a private assignment operator in a class.
 * After using this macro make sure that you switch to public
 * or whatever.
 **/
#define DECLARE_NO_ASSIGNMENT_OPERATOR(classname) \
    private: \
    classname& operator=(const classname& source) {return (*this);}


namespace penv {

/**
 * \brief The PenvHelper class supports the framework with periodic
 * functions which will be needed for the framework. All methods here
 * are static so you can access them with <code>PenvHelper::MethodName</code>.
 **/
class PenvHelper
{
    private:
        PenvHelper();
        ~PenvHelper();

    public:
        static wxString GenerateRandomHexValues(int bytes);
        static bool ParseBoolean(const wxString& boolstring);
        static wxString CreateBoolean(bool boolean);
        static void SplitString(const wxString& string,
            wxArrayString* array, wxChar separator = '/');
        static wxXmlNode* CreateXmlNode(const wxString& elementname,
            const wxString& textcontent = wxEmptyString);
        static bool AddXmlChildNode(wxXmlNode* parent, wxXmlNode* node);


};


} // namespace penv

#endif // _PENV_PENVHELPER_H_
