/*
 * environment.cpp - Implementation of the Environment class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#include "environment.h"
#include "expect.h"
#include "penvhelper.h"
#include "commandcallback.h"

namespace penv {


//----------------------------------------------------------------
/**
 * \brief Only internally used.
 **/
class EnvironmentSingleton
{
    public:
        Environment env;

};


static EnvironmentSingleton instance;
static PackageList* packagelist;
static CommandList* commandlist;
static WorkspaceList* workspacelist;



//----------------------------------------------------------------
Environment::Environment()
/**
 * \brief Constructor, initialize the Environment object.
 * The actual initialization takes place in the Initialization()
 * Method, which will be called from Application::OnInit().
 **/
{
    m_application = NULL;
    m_frame = NULL;
    m_statusbar = NULL;
    m_windowlist = NULL;
    m_commandlist = NULL;
    m_workspacelist = NULL;
    m_packagelist = NULL;
    m_propertylist = NULL;
    m_configpath = penvCOMMON_CONFIGURATIONDIRECTORY;
    m_usehomedirectories = penvCOMMON_CONFIGURATIONUSEHOMEDIRECTORIES;
    m_allusers = penvCOMMON_CONFIGURATIONFORALLUSERS;
    m_size = wxSize(800, 600);
    m_position = wxPoint(5, 5);
    m_maximized = false;
    m_defaultdocument = _T("");
}


//----------------------------------------------------------------
Environment::Environment(const Environment& env)
/**
 * \brief Copyconstructor (private).
 * \param env djklfas.
 **/
{
    if (&env == this) return;
}


//----------------------------------------------------------------
Environment& Environment::operator=(const Environment& env)
/**
 * \brief Assignment operator (private).
 * \param env djklfas.
 **/
{
    if (&env == this) return (*this);
    return (*this);
}


//----------------------------------------------------------------
Environment::~Environment()
/**
 * \brief Destructor.
 **/
{
    if (m_windowlist != NULL) delete m_windowlist;
    if (m_commandlist != NULL) delete m_commandlist;
    if (m_workspacelist != NULL) delete m_workspacelist;
    if (m_packagelist != NULL) delete m_packagelist;
    if (m_propertylist != NULL) delete m_propertylist;
}



//----------------------------------------------------------------
void Environment::Initialize(Application* application)
/**
 * \brief Initializes all objects included in this class.
 * \param application The application object.
 **/
{
    m_application = application;
    m_frame = m_application->GetFrame();
    m_statusbar = m_frame->GetStatusBar();
    m_commandlist = new CommandList(m_frame, m_frame->GetManager(), m_frame->GetMenuBar());
    commandlist = m_commandlist;
    m_windowlist = new WindowList(m_frame, m_frame->GetManager(), m_frame->GetNotebook());
    m_workspacelist = new WorkspaceList();
    workspacelist = m_workspacelist;
    m_propertylist = new PropertyList();
    m_packagelist = m_packagelist;
}



//----------------------------------------------------------------
void Environment::InitializeEvents()
/**
 * \brief Connect all common events which will be executed by the penv
 * Framework. All events of that type, begin with 'PENV_EVT_'.
 **/
{
    m_commandlist->GetCommand(_T("PENV_EVT_WINDOW_REGISTERED"))->
        Connect(new penv::CommandCallback<Environment>(this,&Environment::OnPenvWindowRegistered));
    m_commandlist->GetCommand(_T("PENV_EVT_WINDOW_UNREGISTERED"))->
        Connect(new penv::CommandCallback<Environment>(this,&Environment::OnPenvWindowUnRegistered));
    m_commandlist->GetCommand(_T("PENV_EVT_WINDOW_ADDED"))->
        Connect(new penv::CommandCallback<Environment>(this,&Environment::OnPenvWindowAdded));
    m_commandlist->GetCommand(_T("PENV_EVT_WINDOW_CLOSED"))->
        Connect(new penv::CommandCallback<Environment>(this,&Environment::OnPenvWindowClosed));
}


//----------------------------------------------------------------
Environment* Environment::Get()
/**
 * \brief Access to the environment singleton object.
 * \return Environment singleton object.
 **/
{
    return (&(instance.env));
}


//----------------------------------------------------------------
PackageList* Environment::GetPackageList()
/**
 * \brief Returns the PackageList object, which contains all
 * packages registered in this framework.
 * \return PackageList object.
 **/
{
    if (unlikely(m_packagelist == NULL))
    {
        if (likely(packagelist != NULL))
        {
            m_packagelist = packagelist;
        }
        else
        {
            m_packagelist = new PackageList();
            packagelist = m_packagelist;
        }
    }
    return (m_packagelist);
}



//----------------------------------------------------------------
WindowList* Environment::GetWindowList()
/**
 * \brief Returns the WindowList object, which contains all
 * windows registered in this framework.
 * \return WindowList object.
 **/
{
    return (m_windowlist);
}



//----------------------------------------------------------------
CommandList* Environment::GetCommandList()
/**
 * \brief Returns the CommandList object, which contains all
 * commands and events registered in this framework.
 * \return CommandList object.
 **/
{
    if (m_commandlist != commandlist)
    {
        m_commandlist = commandlist;
    }
    return (m_commandlist);
}


//----------------------------------------------------------------
WorkspaceList* Environment::GetWorkspaceList()
/**
 * \brief Returns the WorkspaceList object, which contains all
 * workspaces, projects and project items in this framework.
 * \return WorkspaceList object.
 **/
{
    if (m_workspacelist != workspacelist)
    {
        m_workspacelist = workspacelist;
    }
    return (m_workspacelist);
}



//----------------------------------------------------------------
PropertyList* Environment::GetPropertyList()
/**
 * \brief Returns the PropertyList object, which could be used to
 * store main configuration for the application. Some configuration
 * properties already definied in the environment.
 * \return PropertyList object.
 **/
{
    return (m_propertylist);
}


//----------------------------------------------------------------
ApplicationFrame* Environment::GetFrame()
/**
 * \brief Returns the ApplicationFrame object, which is the
 * top level window of this application.
 * \return The top level window.
 **/
{
    return (m_frame);
}


//----------------------------------------------------------------
Application* Environment::GetApplication()
/**
 * \brief Returns the Application object.
 * \return Application object.
 **/
{
    return (m_application);
}

//----------------------------------------------------------------
StatusBar* Environment::GetStatusBar()
/**
 * \brief Returns the status bar of the top level window.
 * \return Status bar.
 **/
{
    return (m_statusbar);
}


//----------------------------------------------------------------
const wxString& Environment::GetConfigPath()
/**
 * \brief Returns the configuration path for the application.
 * This will something like 'config' or even an empty sting.
 * Other possibible paths could be '../../../config' or '/ect/penv/config'.
 * \return Configuration path.
 **/
{
    return (m_configpath);
}


//----------------------------------------------------------------
void Environment::SetConfigPath(const wxString& config)
/**
 * \brief Sets the configuration path for the application.
 * See GetConfigPath().
 * \param config Configuration path.
 **/
{
    m_configpath = config;
}


//----------------------------------------------------------------
void Environment::UseHomeDirectories(bool use)
/**
 * \brief Sets if home directories are used to store the
 * configuration.
 * \param use True if home directories are used.
 **/
{
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
bool Environment::SaveConfiguration()
/**
 * \brief Saves the configuration of the application.
 * \return True on success; false otherwise.
 **/
{
    // Konfigurationsdatei ermittlen
    wxString configfile = GetConfigurationFile(true);
    wxXmlDocument doc;
    doc.SetFileEncoding(_T("UTF-8"));
    wxXmlNode* rootnode = PenvHelper::CreateXmlNode(_T("configuration"));
    doc.SetRoot(rootnode);
    wxXmlNode* envnode = WriteEnvironmentConfiguration();
    PenvHelper::AddXmlChildNode(rootnode, envnode);
    wxXmlNode* menubarnode = m_commandlist->WriteMenuBarConfiguration();
    PenvHelper::AddXmlChildNode(rootnode, menubarnode);
    wxXmlNode* toolbarnode = m_commandlist->WriteToolBarsConfiguration();
    PenvHelper::AddXmlChildNode(rootnode, toolbarnode);
    rootnode->AddChild(m_commandlist->WriteShortcutsConfiguration());
    return (doc.Save(configfile, 1));
}



//----------------------------------------------------------------
bool Environment::LoadConfiguration()
/**
 * \brief Loads the configuration for the application.
 * \return True on success; false otherwise.
 **/
{
    // Konfigurationsdatei ermittlen
    wxString configfile = GetConfigurationFile(false);
    if (configfile.IsEmpty())
    {
        wxLogError(_T("[penv::Environment::LoadConfiguration] No configuration file found."));
        return (false);
    }
    // Laden der Konfigurationsdatei
    wxXmlDocument document;
    if (unlikely(!document.Load(configfile, _T("UTF-8"))))
    {
        wxLogError(_T("[penv::Environment::LoadConfiguration] Error loading XML configuration file \"%s\"."), configfile.c_str());
        return (false);
    }
    // Testen ob die Konfigurationsdatei eine korekte Datei ist
    wxXmlNode* rootnode = document.GetRoot();
    if (unlikely(rootnode == NULL))
    {
        wxLogError(_T("[penv::Environment::LoadConfiguration] Root node \"configuration\" not found in file \"%s\"."),configfile.c_str());
        return (false);
    }
    if (unlikely(rootnode->GetName() != _T("configuration")))
    {
        wxLogError(_T("[penv::Environment::LoadConfiguration] Root node not named \"configuration\" in file \"%s\"."),configfile.c_str());
        return (false);
    }
    // Typische Informationen aus der Konfigurationsdatei laden
    wxXmlNode* childnode = rootnode->GetChildren();
    while (childnode != NULL)
    {
        // Wenn Node kein Element muss es übersprungen werden
        if (childnode->GetType() != wxXML_ELEMENT_NODE) {
            childnode = childnode->GetNext();
            continue;
        }
        if (childnode->GetName() == _T("environment"))
        {
            if (unlikely(!LoadEnvironmentConfiguration(childnode)))
            {
                wxLogError(_T("[penv::Environment::LoadConfiguration] Loading environment configuration failed."));
                return (false);
            }
        }
        else if (childnode->GetName() == _T("menubar"))
        {
            if (unlikely(!m_commandlist->LoadMenuBarConfiguration(childnode)))
            {
                wxLogError(_T("[penv::Environment::LoadConfiguration] Loading menubar configuration failed."));
                return (false);
            }
        }
        else if (childnode->GetName() == _T("toolbars"))
        {
            if (unlikely(!m_commandlist->LoadToolBarsConfiguration(childnode)))
            {
                wxLogError(_T("[penv::Environment::LoadConfiguration] Loading toolbars configuration failed."));
                return (false);
            }
        }
        else if (childnode->GetName() == _T("shortcuts"))
        {
            if (unlikely(!m_commandlist->LoadShortcutsConfiguration(childnode)))
            {
                wxLogError(_T("[penv::Environment::LoadConfiguration] Loading shortcut configuration failed."));
                return (false);
            }
        }
        // Nächstes Childnode abarbeiten
        childnode = childnode->GetNext();
    }

    // Laden des Workspaces
    wxString workspaceconfigfile = GetWorkspacesFile(false);
    wxXmlDocument docworkspace(workspaceconfigfile);
    // Prüfen ob Datei korrekt geladen ...
    if (!docworkspace.IsOk())
    {
        wxLogWarning(_T("[penv::Environment::LoadConfiguration] Workspace file '%s' cannot be loaded."), workspaceconfigfile.c_str());
    }
    // ... wenn Datei korrekt geladen wurde, dann auslesen
    else if (!m_workspacelist->ReadNode(docworkspace.GetRoot()))
    {
        wxLogWarning(_T("[penv::Environment::LoadConfiguration] Error occured during loading of workspace file."));
    }
    return (true);
}




//----------------------------------------------------------------
const wxString& Environment::GetApplicationPath()
/**
 * \brief Returns the application path to the directory where
 * the main executable lies.
 * \return Application path.
 **/
{
    return (m_applicationpath);
}



//----------------------------------------------------------------
const wxString& Environment::GetExecuteablePath()
/**
 * \brief Returns the path to the main executable of this
 * application.
 * \return Path to executable.
 **/
{
    return (m_executablepath);
}



//----------------------------------------------------------------
const wxString& Environment::GetStartupWorkingPath()
/**
 * \brief Returns the startup working folder which was given by
 * the start up parameters. This value is normally the same
 * as the GetApplicationPath() returns.
 * \return Startup working folder.
 **/
{
    return (m_startupworkingfolder);
}



//----------------------------------------------------------------
void Environment::SetDefaultDocument(const wxString& classid)
/**
 * \brief Sets the default window id when adding a new
 * project item. The project item will be initialized with
 * that class id.<br>For example: If you have a text editor
 * which should always be loaded, then you can set this document
 * window id here in order that this window should always load
 * the file.
 * \param classid The window class id.
 **/
{
    m_defaultdocument = classid;
}



//----------------------------------------------------------------
const wxString& Environment::GetDefaultDocument() const
/**
 * \brief Returns the default window id. See also
 * SetDefaultDocument().
 * \return The window class id.
 **/
{
    return (m_defaultdocument);
}




//----------------------------------------------------------------
void Environment::SetApplicationPath(const wxString& apppath)
/**
 * \brief Sets the application path.
 * \param apppath Application path.
 **/
{
    m_applicationpath = apppath;
}


//----------------------------------------------------------------
void Environment::SetExecuteablePath(const wxString& execpath)
/**
 * \brief Sets the executable path.
 * \param execpath Executable path.
 **/
{
    m_executablepath = execpath;
}


//----------------------------------------------------------------
void Environment::SetStartupWorkingPath(const wxString& workingpath)
/**
 * \brief Sets the working path.
 * \param workingpath Working path.
 **/
{
    m_startupworkingfolder = workingpath;
}



//----------------------------------------------------------------
bool Environment::LoadEnvironmentConfiguration(wxXmlNode* node)
/**
 * \brief Loads the 'environment' xml element node. which contains
 * main configuration for the application.
 * \param node The 'environment' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Fehler abfangen!!
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::Environment::LoadEnvironmentConfiguration] Xml node is NULL."));
        return (false);
    }
    if (unlikely(node->GetName() != _T("environment"))) {
        wxLogError(_T("[penv::Environment::LoadEnvironmentConfiguration] Not an \"element\" xml node."));
        return (false);
    }
    wxXmlNode* child = node->GetChildren();
    while (child != NULL)
    {
        if (child->GetType() != wxXML_ELEMENT_NODE)
        {
            child = child->GetNext();
            continue;
        }
        if (child->GetName() == _T("penvconfig"))
        {
            if (!ReadNodePenvConfig(child)) {
                wxLogError(_T("[penv::Environment::LoadEnvironmentConfiguration] Could not read node 'penvconfig'."));
                return (false);
            }
        }
        else if (child->GetName() == _T("properties"))
        {
            // Eigenschaften auslesen
            if (unlikely(!m_propertylist->ReadNode(child))) {
                wxLogError(_T("[penv::Environment::LoadEnvironmentConfiguration] Failed to load properties from the environment."));
                return (false);
            }
        }
        else
        {
            wxLogWarning(_T("[penv::Environment::LoadEnvironmentConfiguration] No recognized element \"%s\"."),child->GetName().c_str());
        }
        // Zum nächsten Knoten springen
        child = child->GetNext();
    }
    return (true);
}



//----------------------------------------------------------------
bool Environment::ReadNodePenvConfig(wxXmlNode* node)
/**
 * \brief Read out a 'penvconfig' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'penvconfig' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Fehler abfangen!!
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::Environment::ReadPenvConfigNode] Xml node is NULL."));
        return (false);
    }
    if (unlikely(node->GetType() != wxXML_ELEMENT_NODE)) {
        wxLogError(_T("[penv::Environment::ReadPenvConfigNode] Not a element node."));
        return (false);
    }
    if (unlikely(node->GetName() != _T("penvconfig"))) {
        wxLogError(_T("[penv::Environment::ReadPenvConfigNode] Not a 'penvconfig' xml node."));
        return (false);
    }
    wxXmlNode* child = node->GetChildren();
    while (child != NULL)
    {
        if (child->GetType() == wxXML_ELEMENT_NODE)
        {
            if (child->GetName() == _T("penvversion"))
            {
                Version version;
                version.SetVersion(child->GetNodeContent());
                if (version != penvVERSION)
                {
                    wxLogWarning(_T("[penv::Environment::LoadEnvironmentConfiguration] penv version in config file does not match build version of penv."));
                }
            }
            else if (child->GetName() == _T("applicationname"))
            {
                m_applicationname = child->GetNodeContent();
            }
            else if (child->GetName() == _T("applicationversion"))
            {
                bool unicode = false;
                #ifdef UNICODE
                unicode = true;
                #endif
                m_applicationversion.SetVersion(child->GetNodeContent(), unicode);
            }
            else if (child->GetName() == _T("unicode"))
            {
                bool unicode = false;
                #ifdef UNICODE
                unicode = true;
                #endif
                wxString nodecontent = child->GetNodeContent();
                if (!unicode && (nodecontent.Upper() == _T("true")))
                {
                    wxLogError(_T("[penv::Environment::LoadEnvironmentConfiguration] Build is unicode, but application is not."));
                    return (false);
                }
            }
            else if (child->GetName() == _T("maximized"))
            {
                m_maximized = PenvHelper::ParseBoolean(child->GetNodeContent());
            }
            else if (child->GetName() == _T("size"))
            {
                wxString size = child->GetNodeContent();
                wxArrayString array;
                PenvHelper::SplitString(size, &array, ',');
                long width = 800, height = 600;
                if (array.Count() == 2)
                {
                    array[0].ToLong(&width);
                    array[1].ToLong(&height);
                }
                m_size.SetWidth(width);
                m_size.SetHeight(height);
            }
            else if (child->GetName() == _T("position"))
            {
                wxString position = child->GetNodeContent();
                wxArrayString array;
                PenvHelper::SplitString(position, &array, ',');
                long x = 5, y = 5;
                if (array.Count() == 2)
                {
                    array[0].ToLong(&x);
                    array[1].ToLong(&y);
                }
                m_position.x = x;
                m_position.y = y;
            }

        }
        child = child->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* Environment::WriteEnvironmentConfiguration()
/**
 * \brief Writes the 'environment' configuration node into the
 * given parent node. The returning node must be added to the parent
 * via AddChild().
 * \return Xml 'environment' element node.
 **/
{
    wxXmlNode* envnode = PenvHelper::CreateXmlNode(_T("environment"));
    // Write penvconfig node
    wxXmlNode* confignode = WriteNodePenvConfig();
    PenvHelper::AddXmlChildNode(envnode, confignode);
    // Properties schreiben
    wxXmlNode* propnode = m_propertylist->WriteNode();
    if (propnode == NULL) wxLogWarning(_T("[penv::Environment::WriteEnvironmentConfiguration] Cannot create property list node, error occured."));
    else PenvHelper::AddXmlChildNode(envnode, propnode);
    // Zurückgeben
    return (envnode);
}


//----------------------------------------------------------------
wxXmlNode* Environment::WriteNodePenvConfig()
/**
 * \brief Writes a 'penvconfig' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'penvconfig' xml element node; or NULL on error.
 **/
{
    wxXmlNode* confignode = PenvHelper::CreateXmlNode(_T("penvconfig"));
    // Versionstring schreiben
    wxString versioninfo = penvVERSION.GetVersion();
    wxXmlNode* versionnode = PenvHelper::CreateXmlNode(_T("penvversion"), versioninfo);
    PenvHelper::AddXmlChildNode(confignode, versionnode);
    // Anwendungsname schreiben
    wxXmlNode* appnamenode = PenvHelper::CreateXmlNode(_T("applicationname"), m_applicationname);
    PenvHelper::AddXmlChildNode(confignode, appnamenode);
    // Anwendungsversion schreiben
    wxString appversioninfo = m_applicationversion.GetVersion();
    wxXmlNode* appversionnode = PenvHelper::CreateXmlNode(_T("applicationversion"), appversioninfo);
    PenvHelper::AddXmlChildNode(confignode, appversionnode);
    // Unicode Information schreiben
    wxString unicodeinfo = _T("false");
    #ifdef UNICODE
    unicodeinfo = _T("true");
    #endif
    wxXmlNode* unicodenode = PenvHelper::CreateXmlNode(_T("unicode"), unicodeinfo);
    PenvHelper::AddXmlChildNode(confignode, unicodenode);
    // Write maximized
    wxXmlNode* maximizednode = PenvHelper::CreateXmlNode(_T("maximized"), PenvHelper::CreateBoolean(m_frame->IsMaximized()));
    PenvHelper::AddXmlChildNode(confignode, maximizednode);
    // Write size
    wxString size = wxString::Format(_T("%i,%i"), m_frame->GetSize().GetWidth(), m_frame->GetSize().GetHeight());
    wxXmlNode* sizenode = PenvHelper::CreateXmlNode(_T("size"), size);
    PenvHelper::AddXmlChildNode(confignode, sizenode);
    // Write position
    wxString position = wxString::Format(_T("%i,%i"), m_frame->GetPosition().x, m_frame->GetPosition().y);
    wxXmlNode* positionnode = PenvHelper::CreateXmlNode(_T("position"), position);
    PenvHelper::AddXmlChildNode(confignode, positionnode);
    return (confignode);
}



//----------------------------------------------------------------
wxString Environment::GetConfigurationFile(bool save)
/**
 * \brief Returns the configuration file for the application.
 * If there is no configuration file, an empty string
 * will be generated. If parameter save is true, then
 * this method will never return the 'default.xml' configuration.
 * But a user specific configuration filename, will be generated.
 * \param save True, when a cufiguration file will be saved.
 * \return Path to configuration.
 **/
{
    wxString configfile;
    wxString configpath;
    // Konfigurations Verzeichniss ermitteln und in
    //  configpath schreiben
    if (m_usehomedirectories) {
        // TODO SO: Auch unterscheiden zwischen AllUsers und Benutzernamen!!!!
        NOT_IMPLEMENTED_YET();
    } else {
        configpath = Path::Combine(m_applicationpath, m_configpath);
    }
    if (save)
    {
        // If config path does not exists
        if (unlikely(!wxDir::Exists(configpath))) {
            if (!Path::Create(configpath)) {
                wxLogError(_T("[penv::Environment::GetConfigurationFile] Could not create configuration directory \"%s\"."), configpath.c_str());
                return (wxEmptyString);
            }
        }
        if (m_allusers) {
            // Create filename config_AllUsers.xml
            wxString filename(_T("config_AllUsers.xml"));
            configfile = Path::Combine(configpath, filename);
        } else {
            // Create filename config_USERNAME.xml
            wxString filename(_T("config_"));
            filename.Append(wxGetUserName()).Append(_T(".xml"));
            configfile = Path::Combine(configpath, filename);
        }
    }
    else
    {
        // Testen ob das Verzeichniss existiert
        if (unlikely(!wxDir::Exists(configpath))) {
            wxLogError(_T("[penv::Environment::GetConfigurationFile] Configuration directory \"%s\" does not exists."), configpath.c_str());
            return (wxEmptyString);
        }
        // Nach Konfigurationsdateien testen
        // Zuerst nach der config_Benutztername testen
        wxString filename(_T("config_"));
        filename.Append(wxGetUserName()).Append(_T(".xml"));
        wxString tempfilename = Path::Combine(configpath, filename);
        if (wxFile::Exists(tempfilename)) {
            configfile = tempfilename;
        }
        else
        {
            // Falls config_Benutzername nicht existiert, nach der
            // config_AllUsers testen
            //wxLogMessage(_T("[penv::Environment::GetConfigurationFile] Configuration file \"%s\" not found, try loading \"config_AllUsers.xml\"."), tempfilename.c_str());
            filename = _T("config_AllUsers.xml");
            tempfilename = Path::Combine(configpath, filename);
            if (wxFile::Exists(tempfilename)) {
                configfile = tempfilename;
            }
            else
            {
                // Falls config_AllUsers auch nicht existiert, dann muss
                // default.xml vorhanden sein, sonst muss das Programm abrechen
                //wxLogMessage(_T("[penv::Environment::GetConfigurationFile] Configuration file \"%s\" not found, try loading \"default.xml\"."), tempfilename.c_str());
                filename = _T("default.xml");
                tempfilename = Path::Combine(configpath, filename);
                if (wxFile::Exists(tempfilename)) {
                    configfile = tempfilename;
                }
                else
                {
                    wxLogError(_T("[penv::Environment::GetConfigurationFile] No configuration file found."));
                    return (wxEmptyString);
                }
            }
        }
    }
    return (configfile);
}

//----------------------------------------------------------------
wxString Environment::GetWorkspacesFile(bool save)
/**
 * \brief Returns the path to the workspaces filename.
 * If there is no workspaces file, an empty string
 * will be generated. A user specific workspaces filename,
 * will be generated. The parameter 'save' is not used.
 * \param save True if the workspaces filename will be used for saving.
 * \return Path to workspaces file.
 **/
{

    // TODO SO
    return (Path::Combine(m_applicationpath, _T("workspace\\workspaces.xml")));
}



//----------------------------------------------------------------
void Environment::OnPenvWindowRegistered(CommandEvent& event)
/**
 * \brief This Method is called when an PENV_EVT_WINDOW_REGISTER
 * Event is called. This Method will only be registered in Debug Mode
 * to refresh the Debug Window.
 * \param event The Event
 **/
{
    DebugWindow* window = m_application->GetFrame()->GetDebugWindow();
    if (window != NULL) window->RefreshWindows();
}


//----------------------------------------------------------------
void Environment::OnPenvWindowUnRegistered(CommandEvent& event)
/**
 * \brief This Method is called when an PENV_EVT_WINDOW_UNREGISTER
 * Event is called. This Method will only be registered in Debug Mode
 * to refresh the Debug Window.
 * \param event The Event
 **/
{
    DebugWindow* window = m_application->GetFrame()->GetDebugWindow();
    if (window != NULL) window->RefreshWindows();
}


//----------------------------------------------------------------
void Environment::OnPenvWindowAdded(CommandEvent& event)
/**
 * \brief This Method is called when an PENV_EVT_WINDOW_ADDED
 * Event is called. This Method will only be registered in Debug Mode
 * to refresh the Debug Window.
 * \param event The Event
 **/
{
    DebugWindow* window = m_application->GetFrame()->GetDebugWindow();
    if (window != NULL) window->RefreshWindows();
}


//----------------------------------------------------------------
void Environment::OnPenvWindowClosed(CommandEvent& event)
/**
 * \brief This Method is called when an PENV_EVT_WINDOW_CLOSED
 * Event is called. This Method will only be registered in Debug Mode
 * to refresh the Debug Window.
 * \param event The Event
 **/
{
    DebugWindow* window = m_application->GetFrame()->GetDebugWindow();
    if (window != NULL) window->RefreshWindows();
}





} // namespace penv
