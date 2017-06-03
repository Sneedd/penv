/*
 * application.h - Declaration of the Application class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#ifndef _PENV_APPLICATION_H_
#define _PENV_APPLICATION_H_

#include "wxincludes.h"
#include "applicationframe.h"



namespace penv {

/**
 * \brief The Application class is the main entry point into the
 * application. The method OnInit() is the first code which will
 * be executed, the method OnExit() is the last.
 * \todo Add a resource manager for images and strings.
 * \todo Add support for dynamic packages.
 * \todo Add command line option to spezify log file path.
 * \todo Add command line option for not saving the configuration.
 **/
class Application : public wxApp
{
    public:
        Application();
        ~Application();
        ApplicationFrame* GetFrame();

    private:
        bool OnInit();
        int OnExit();
        void OnInitCmdLine(wxCmdLineParser& parser);
        bool OnCmdLineParsed(wxCmdLineParser& parser);
        void OnUnhandledException();
        bool OnExceptionInMainLoop();
        void OnFatalException();

    private:
        wxCmdLineParser m_cmdlineparser;
        ApplicationFrame* m_frame;
        FILE* m_logoutput;
        wxLog* m_logger;

        bool m_loadalternativeconfig;
        bool m_loaddefaultconfig;
        bool m_nopackages;
        bool m_resetpackages;
        wxString m_alternativeconfigfile;
};


} // namespace penv

#endif // _PENV_APPLICATION_H_
