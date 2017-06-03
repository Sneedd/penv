/*
 * environment.h - Declaration of the Environment class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */

#ifndef _PENV_ENVIRONMENT_H_
#define _PENV_ENVIRONMENT_H_

#include "wxincludes.h"
#include "workspacelist.h"
#include "windowlist.h"
#include "commandlist.h"
#include "packagelist.h"
#include "propertylist.h"
#include "application.h"
#include "applicationframe.h"
#include "version.h"


namespace penv {

/**
 * \brief The Environment class is the main point for every action on the
 * project environment framework. This class is implemented as a singleton
 * class, where the static method Get() is used to gather this obejct.<br>
 * Example: <code>Environment* env = Environment::Get();</code><br><br>
 * The framework is separeted in 4 different modules:<br>
 * The <b>command environment</b> where the commands will be managed and their
 * connections to a toolbar, menubar or shortcuts. You can get this module
 * via the GetCommandList() method.<br>
 * The <b>window environment</b> where every inherited document window or
 * window pane can be registered and shown. You will be able to create an
 * conection between an window and projectitem, to handle different files
 * with differnt windows. You can access this module by calling the
 * GetWindowList() method from this class.<br>
 * The <b>project environment</b> is for handling workspaces, projects and
 * projectitems. It can be access through GetWorkspaceList().<br>
 * The Environment class also have an PropertiesList which can be used for
 * writing configurations for the whole application.
 **/
class Environment
{
    private:
        friend class Application;
        friend class EnvironmentSingleton; // Magic
        Environment();
        Environment(const Environment& env);
        Environment& operator=(const Environment& env);
        ~Environment();
        void Initialize(Application* application);
        void InitializeEvents();

    public:
        // Singleton Zugriff
        static Environment* Get();
        WindowList* GetWindowList();
        CommandList* GetCommandList();
        WorkspaceList* GetWorkspaceList();
        PackageList* GetPackageList();
        PropertyList* GetPropertyList();
        ApplicationFrame* GetFrame();
        Application* GetApplication();
        StatusBar* GetStatusBar();


    public:
        const wxString& GetConfigPath();
        void SetConfigPath(const wxString& config);

        void UseHomeDirectories(bool use=true);
        bool SaveConfiguration();
        bool LoadConfiguration();

        const wxString& GetApplicationPath();
        const wxString& GetExecuteablePath();
        const wxString& GetStartupWorkingPath();

        void SetDefaultDocument(const wxString& classid);
        const wxString& GetDefaultDocument() const;

    private:
        void SetApplicationPath(const wxString& apppath);
        void SetExecuteablePath(const wxString& execpath);
        void SetStartupWorkingPath(const wxString& workingpath);

        bool LoadEnvironmentConfiguration(wxXmlNode* node);
        bool ReadNodePenvConfig(wxXmlNode* node);
        wxXmlNode* WriteEnvironmentConfiguration();
        wxXmlNode* WriteNodePenvConfig();
    public:
        wxString GetConfigurationFile(bool save);
        wxString GetWorkspacesFile(bool save);

    private: // Interne registrierte Events
        void OnPenvWindowRegistered(CommandEvent& event);
        void OnPenvWindowUnRegistered(CommandEvent& event);
        void OnPenvWindowAdded(CommandEvent& event);
        void OnPenvWindowClosed(CommandEvent& event);


    private:
        WindowList* m_windowlist;       // Fensterverwaltung
        CommandList* m_commandlist;     // Befehle und Events
        WorkspaceList* m_workspacelist; // Die Arbeitsbereiche und Projekte
        PackageList* m_packagelist;     // Die Pakete in dem Framework
        PropertyList* m_propertylist;   // Die Eigenschaften des Frameworks
        ApplicationFrame* m_frame;      // Das Hauptfenster
        Application* m_application;     // Das Anwendungs Objekt
        StatusBar* m_statusbar;         // Die Statusbar
        unsigned long m_mainthread;     // ID des Haupthreads
        wxString m_configpath;          // Konfigurationspfad (bsp. "config")
        bool m_usehomedirectories;      // Sollen Heimverzeichnisse verwendet werden?
        bool m_allusers;                // Zum speichern immer AllUsers verwenden?
        bool m_maximized;
        wxSize m_size;
        wxPoint m_position;
        wxString m_defaultdocument;

        wxString m_executablepath;      // Pfad zur Datei der Anwendung
        wxString m_startupworkingfolder;// Pfad zum Verzeichnis der Anwendung
        wxString m_applicationpath;     // Pfad zur Datei der Anwendung
        wxString m_applicationname;     // Anwendungsname
        Version m_applicationversion;   // Version der Anwendung
        bool m_uivisible;

};

} // namespace penv


#endif // _PENV_ENVIRONMENT_H_
