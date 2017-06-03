/*
 * package.h - Declaration of the Package class
 *
 * Author: Steffen Ott
 * Date: 10/01/2007
 *
 */

#ifndef _PENV_PACKAGE_H_
#define _PENV_PACKAGE_H_

#include "wxincludes.h"
#include "propertylist.h"
#include "version.h"
#include "packagedependency.h"
#include "container/objarray.h"


namespace penv {

/**
 * \brief Error codes for initialization and
 * deinitalization of a package.
 **/
enum
{
    /**
     * \brief No error occured.
     **/
    penvPKG_NOERROR     = 1,
    /**
     * \brief Just warn that something happend.
     **/
    penvPKG_WARNING     = -10,
    /**
     * \brief Error which could stop the the application from running.
     **/
    penvPKG_ERROR       = -15,
    /**
     * \brief Really a bad error, stops the execution immediatly.
     **/
    penvPKG_FATALERROR  = -20
};


/**
 * \brief The Package class represents a package in the framework.
 * You can see a package as the main elements who contains the applciation
 * logic, windows and so on. A package is the only possibility to
 * add functionality to the framework. The packages can communicate
 * with each other over the registered commands. Is is possible
 * to call commands from one package within another package. But it
 * is recommended that only one direction should be allowed. For a
 * package it is possible to override commands from other packages.
 * Therefore the overridding package depends on the other packages,
 * which must be anounced in a package. See GetDependency(), Add(),
 * Remove() and Count().<br>
 * Package information could be in an xml file, so there are the
 * two methods ReadNode() and WriteNode() for this case. The xml
 * could look like this:<br>
 * <code>
 *  &lt;package id="UNIQUEID" name="package name" version="0.8.0406 unicode"&gt;<br>
 *  &nbsp;&nbsp;&lt;dependencies&gt;<br>
 *  &nbsp;&nbsp;&nbsp;&nbsp;&lt;dependency id="UNIQUEID2" version="0.9.1306 unicode"/&gt;<br>
 *  &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- Other dependency information for the package --&gt;<br>
 *  &nbsp;&nbsp;&lt;/dependencies&gt;<br>
 *  &nbsp;&nbsp;&lt;properties&gt;<br>
 *  &nbsp;&nbsp;&nbsp;&nbsp;&lt;!-- Properties for the package --&gt;<br>
 *  &nbsp;&nbsp;&lt;/properties&gt;<br>
 *  &lt;/package&gt;<br>
 * </code>
 * <br>
 * <b>How to create a static package:</b> A package can easily created by
 * inherit from the package class. The constructor of the inherited class
 * muss be a standard contructor. Because the standard constructor of the
 * Package class is private, you must use the alternative version of the
 * constructor.<br>
 * Here is a example how a static package will be created and initialized:<br>
 * <code>
 * class InheritedPackage : public penv::Package<br>
 * {<br>
 * &nbsp;&nbsp;public:<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;InheritedPackage() : Package(_T("Inhertited package name"), _T("UNIQUE_ID_FOR_INHERITED_PACKAGE"), Version(0,9,0408))<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;{<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// Additional dependencies information should be declared here.<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// No registration of commands or windows is allowed here!<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
 * <br>
 * &nbsp;&nbsp;protected:<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;int OnInitPackage()<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;{<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// Here you can register commands, windows or whatever.<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// It is not allowed to access UI components at this time.<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;int OnExitPackage()<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;{<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// Here you should do some cleanup, if needed<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// You should not clean up commands, windows or whatever, because<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// &nbsp;&nbsp;it will be cleaned up by the framework.<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
 * };<br>
 * <br>
 * // Add the package to the framework like this:<br>
 * PENV_INITALIZE_STATIC_PACKAGE(InheritedPackage);
 * </code>
 * <br><br>
 * <b>How to create a dynamic package:</b> This feature is not implemented
 * yet and therefore it cannot be used.
 **/
class Package
{
    private:
        friend class PackageList;
        Package();
    public:
        Package(const wxString& name, const wxString& id, const Version& version);
        virtual ~Package();

        wxString GetVersionString() const;
        const Version& GetVersion() const;
        const wxString& GetName() const;
        const wxString& GetId() const;

        PackageDependency* GetDependency(size_t index);
        bool Add(PackageDependency* dependency);
        bool Remove(PackageDependency* dependency);
        size_t Count() const;

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

    protected:
        /**
         * \brief This method will be called to initialize the package.
         * It will be called before the window is shown and therfore not
         * user interface operations are allowed here.
         * \return Error code (penvPKG_FATALERROR quits execution of the
         * application).
         **/
        virtual int OnInitPackage() = 0;

        /**
         * \brief This callback will be called to exit the package.
         * It will be called after the window is destroyed and therefore
         * no user interface operations are allowed here.
         * \return Error code.
         **/
        virtual int OnExitPackage() = 0;


    protected:
        wxString                    m_name;         ///< Name of the package.
        PropertyList*               m_properties;   ///< PropertyList connected to package.
        wxString                    m_id;           ///< Unique id for package.
        Array<PackageDependency>*   m_dependencies; ///< Package dependencies.
        Version                     m_version;      ///< Package version.
        int                         m_initalizationstatus; ///< Initialization status.
};

} // namespace penv

#endif // _PENV_PACKAGE_H_
