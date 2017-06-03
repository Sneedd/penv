/*
 * application.cpp - Implementation of the Application class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#include "application.h"
#include <cstdlib>
#include <ctime>
#include "penvhelper.h"
#include <cstdio>
#include "environment.h"
#include "package.h"

#ifdef __DEBUG__
    #ifndef __WXDEBUG__
        #warning __DEBUG__ defined but not __WXDEBUG__
    #endif
#endif

namespace penv {

IMPLEMENT_APP(Application);

//----------------------------------------------------------------
Application::Application()
/**
 * \brief Constrcutor, initializes the Application object.
 **/
{
    // Keine Initalisierungen
    m_frame = NULL;
    m_logoutput = NULL;
    m_logger = NULL;
}


//----------------------------------------------------------------
Application::~Application()
/**
 * \brief Destructor.
 **/
{
    // Nichts zu tun
}

//----------------------------------------------------------------
ApplicationFrame* Application::GetFrame()
/**
 * \brief Returns the top level window for this application.
 * \return Top level window.
 **/
{
    return (m_frame);
}


//----------------------------------------------------------------
bool Application::OnInit()
/**
 * \brief Initializes the application. Loads configuration, initializes
 * packages and shows the top level window.
 * \return True on success; false to abort application.
 **/
{
    // Randomzahlengenerator initalisieren
    srand(time(NULL));

    // Commandline parser
    m_cmdlineparser.SetCmdLine(argc, argv);
    OnInitCmdLine(m_cmdlineparser);
    int parserresult = m_cmdlineparser.Parse(false);
    if (parserresult == 0) {
        if (!OnCmdLineParsed(m_cmdlineparser)) {
            return (false);
        }
    }
    else if (parserresult == -1) {
        OnCmdLineHelp(m_cmdlineparser);
        return (false);
    }
    else {
        OnCmdLineError(m_cmdlineparser);
        return (false);
    }
    // Initialize PNG support
    wxImage::AddHandler(new wxPNGHandler());

    // Logging Klasse initalisieren
    #ifdef __WINDOWS__
        // Unter Windows kann nichts an STDOUT weitergegeben werden,
        // deshalb wird in eine Datei die Log Informationen geschrieben
        m_logoutput = fopen(penvCOMMON_LOGFILEPATH, "wa");
        wxLog* m_logger = new wxLogStderr(m_logoutput);
        wxLog::SetActiveTarget(m_logger);
    #else // erwartet __UNIX__
        // Unter Linux kann ganz normal in die STDOUT geschrieben werden
        wxLog* m_logger = new wxLogStderr();
        wxLog::SetActiveTarget(m_logger);
    #endif

    // Im Debug Modus Memory Trancing einschalten um Speicherlecks zu finden
    #ifdef __DEBUG__
    #if wxUSE_MEMORY_TRACING
    wxDebugContext::SetCheckpoint();
    #else
        #warning No Memory tracing available
    #endif
    #endif

    // Hole Environment
    Environment* env = Environment::Get();

    // Laden der Resourcen
    // TODO SO: Resourcen laden

    // Setzten der Pfade
    wxString execpath = Path::Normalize(argv[0]);
    env->SetExecuteablePath(execpath);
    wxString apppath = Path::RemoveLastEntry(execpath);
    env->SetApplicationPath(apppath);
    wxString workingpath = Path::Normalize(wxGetCwd());
    env->SetStartupWorkingPath(workingpath);
    wxLogMessage(_T("[penv::Application::OnInit] Application path is \"%s\"."), apppath.c_str());
    wxLogMessage(_T("[penv::Application::OnInit] Executablepath path is \"%s\"."), execpath.c_str());
    wxLogMessage(_T("[penv::Application::OnInit] Workingfolder is \"%s\"."), workingpath.c_str());

    // Hauptfenster initalisieren
    m_frame = new ApplicationFrame(NULL, penvCOMMON_TOPLEVELWINDOWCAPTION);
    // Initalisieren der Evironment Klasse
    env->Initialize(this);
    if (wxIsMainThread()) {
        env->m_mainthread = wxThread::GetCurrentId();
    } else {
        wxLogWarning(_T("[penv::Application::OnInit] Unable to gather main thread id. Could cause problems, when using threads."));
    }

    // Packages inialisieren und ...
    int package_errorlevel = penvPKG_NOERROR;
    if (unlikely((package_errorlevel = env->GetPackageList()->InitalizePackages()) != penvPKG_NOERROR))
    {
        wxLogWarning(_T("[penv::Application::OnInit] Some Packages failed to initalize."));
        if (package_errorlevel == penvPKG_FATALERROR)
        {
            wxMessageBox(_("Failed to initialize one or more packages. Try using the command line option '--resetpackages'."),
                _T("Package initialization failure!"), wxOK|wxICON_ERROR);
            return (false);
        }
    }
    // ... Dynamische Packages initalisieren
    if (m_resetpackages) {
        // TODO SO: dynamische Packages einstellungen reseten
        NOT_IMPLEMENTED_YET();
    }
    if (!m_nopackages) {
        // TODO SO: dynamische Packages initalisieren
        NOT_IMPLEMENTED_YET();
    }


    // Eigene events initialisieren
    env->InitializeEvents();
    // Nachdem Packages initalisiert sind, config Dateien auslesen
    if (m_loadalternativeconfig) {
        // TODO SO
        NOT_IMPLEMENTED_YET();
    }
    else if (m_loaddefaultconfig) {
        // TODO SO
        NOT_IMPLEMENTED_YET();
    }
    if (!env->LoadConfiguration()) {
        wxLogError(_T("[penv::Application::OnInit] The application could not load the configuration file. Aborting initalization."));
        return (false);
    }

    // TODO : Use Getter from Environment!
    m_frame->SetSize(env->m_size);
    m_frame->Maximize(env->m_maximized);
    m_frame->SetPosition(env->m_position);

    // Because the debug window should display something the
    //  following methods must be executed.
    if (m_frame->IsDebugWindowVisible()) {
        DebugWindow* debugwindow = m_frame->GetDebugWindow();
        debugwindow->RefreshWindows();
        debugwindow->RefreshCommands();
    }
    // Because the project tree ctrl should display something the
    //  following methods must be executed.
    if (m_frame->IsProjectTreeCtrlVisible()) {
        ProjectTreeCtrl* prjtreectrl = m_frame->GetProjectTreeCtrl();
        prjtreectrl->UpdateComponents();
    }

    // Show main window
    m_frame->Show(true);
    env->GetCommandList()->GetToolBarList()->Update();
    // Good Boy!!!
    return (true);
}



//----------------------------------------------------------------
int Application::OnExit()
/**
 * \brief OnExit is called after destroying all application windows
 * and controls, but before wxWidgets cleanup.
 * \return Return value of this function is ignored.
 **/
{
    // Deinitalisieren der Packages
    int package_errorlevel = penvPKG_NOERROR;
    if (unlikely((package_errorlevel = Environment::Get()->m_packagelist->DeinitalizePackages()) <= 0))
    {
        wxLogWarning(_T("[penv::Application::OnInit] Some Packages failed to deinitalize."));
    }

    #ifdef __DEBUG__
    #if wxUSE_MEMORY_TRACING
    wxDebugContext::PrintClasses();
    wxDebugContext::Dump();
    wxDebugContext::PrintStatistics();
    #endif
    #endif

    // Rückgabewert wird von wxWidgets ignoriert
    return (0);
}




//----------------------------------------------------------------
void Application::OnInitCmdLine(wxCmdLineParser& parser)
/**
 * \brief Called from OnInit() to initialize the command line
 * parser.
 * \param parser Command line parser handed over by OnInit().
 **/
{
    m_cmdlineparser.EnableLongOptions(true);
    m_cmdlineparser.SetLogo(_T("penv Command Line parameters"));
    m_cmdlineparser.AddSwitch(_T("h"), _T("help"), _T("Show this help message."), 0);
    m_cmdlineparser.AddSwitch(_T("r"), _T("resetpackages"), _T("Resets all dynamic packages."), 0);
    m_cmdlineparser.AddSwitch(_T("n"), _T("nopackages"), _T("Forces to not load any dynamic package."), 0);
    m_cmdlineparser.AddSwitch(_T("d"), _T("defaultconfig"), _T("Forces to load the default configuration file."), 0);
    m_cmdlineparser.AddOption(_T("c"), _T("config"), _T("Forces to load the given configuration file."), wxCMD_LINE_VAL_STRING, 0);
}



//----------------------------------------------------------------
bool Application::OnCmdLineParsed(wxCmdLineParser& parser)
/**
 * \brief Called from OnInit() after a successful parse operation.
 * \param parser Command line parser.
 * \return True on success; false otherwise.
 **/
{
    if (parser.Found(_T("h"))) {
        OnCmdLineHelp(parser);
        return (false);
    }
    if (parser.Found(_T("r"))) {
        m_resetpackages = true;
    }
    if (parser.Found(_T("n"))) {
        m_nopackages = true;
    }
    if (parser.Found(_T("d"))) {
        m_loaddefaultconfig = true;
        if (parser.Found(_T("c"), &m_alternativeconfigfile)) {
            m_cmdlineparser.SetLogo(_T("Cannot use the option 'c' with switch 'd'!\n penv Command Line parameters"));
            OnCmdLineHelp(parser);
            return (false);
        }
    }
    if (parser.Found(_T("c"), &m_alternativeconfigfile)) {
        m_loadalternativeconfig = true;
        if (parser.Found(_T("d"))) {
            m_cmdlineparser.SetLogo(_T("Cannot use the option 'c' with switch 'd'!\n penv Command Line parameters"));
            OnCmdLineHelp(parser);
            return (false);
        }
    }
    return (true);
}


//----------------------------------------------------------------
void Application::OnUnhandledException()
/**
 * \brief This function is called when an unhandled
 * C++ exception occurs inside OnRun().
 **/
{
    wxLogError(_T("[penv::Application::OnUnhandledException] Unhandled exeption occured."));
}



//----------------------------------------------------------------
bool Application::OnExceptionInMainLoop()
/**
 * \brief This function is called if an unhandled exception occurs
 * inside the main application event loop.
 * \return Always false.
 **/
{
    wxLogError(_T("[penv::Application::OnUnhandledException] Exeption in mainloop occured."));
    return (false);
}


//----------------------------------------------------------------
void Application::OnFatalException()
/**
 * \brief This function may be called if something fatal happens.
 **/
{
    wxLogError(_T("[penv::Application::OnFatalException] Fatal exeption in mainloop occured."));
}




} // namespace penv



