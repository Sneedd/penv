/*
 * windowmapping.cpp - Implementation of the WindowMapping class
 *
 * Author: Steffen Ott
 * Date: 03/21/2008
 *
 */



#include "windowmapping.h"

#include "environment.h"
#include "penvhelper.h"


namespace penv {

//----------------------------------------------------------------
WindowMapping::WindowMapping()
/**
 * \brief Constructor (private).
 **/
{
}



//----------------------------------------------------------------
WindowMapping::WindowMapping(const wxString& windowdocumentid)
/**
 * \brief Constructor, initializes the WindowMapping object.
 * \param windowdocumentid The window document id for which the window
 * mapping is needed.
 **/
{
    m_windowdocumentid = windowdocumentid;
    m_mappedpanes = new WindowsHashMap();
}



//----------------------------------------------------------------
WindowMapping::~WindowMapping()
/**
 * \brief Destructor.
 **/
{
    Clear();
    if (m_mappedpanes != NULL) delete m_mappedpanes;
}


//----------------------------------------------------------------
void WindowMapping::Register(const wxString& windowpaneid)
/**
 * \brief Registers a window pane to the window document of
 * this class.
 * \param windowpaneid The window pane id which should be mapped
 * the the window document.
 **/
{
    Environment* env = Environment::Get();
    // Hole Fenster Typ ID
    Window* win = env->GetWindowList()->GetRegistered(windowpaneid);
    if (win != NULL)
    {
        if (win->GetId().IsEmpty())
        {
            // Überprüfen ob Window ein Pane ist
            if (win->GetType() == penvWT_Pane)
            {
                (*m_mappedpanes)[windowpaneid] = (WindowPane*)win;
                return;
            }
            else
            {
                wxLogError(_T("[penv::WindowMapping::Register] Cannot map window '%s' to document window '%s', because the window is not an window pane."), windowpaneid.c_str(), m_windowdocumentid.c_str());
                return;
            }
        }
    }
    // Wenn nicht vom Typ Fenster ID, dann vielleicht eines der geöffneten Fenster IDs
    win = env->GetWindowList()->GetWindow(windowpaneid);
    if (win != NULL)
    {
        if (win->GetId() == windowpaneid)
        {
            // Überprüfen ob Window ein Pane ist
            if (win->GetType() == penvWT_Pane)
            {
                (*m_mappedpanes)[windowpaneid] = (WindowPane*)win;
                return;
            }
            else
            {
                wxLogError(_T("[penv::WindowMapping::Register] Cannot map window '%s' to document window '%s', because the window is not an window pane."), windowpaneid.c_str(), m_windowdocumentid.c_str());
                return;
            }
        }
    }
    wxLogError(_T("[penv::WindowMapping::Register] Cannot map window '%s' to document window '%s', because the window is not available."), windowpaneid.c_str(), m_windowdocumentid.c_str());
}


//----------------------------------------------------------------
void WindowMapping::UnRegister(const wxString& windowpaneid)
/**
 * \brief Unregisters a window pane for the window document
 * connected to this class.
 * \param windowpaneid The window pane id for this document.
 **/
{
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
void WindowMapping::Clear()
/**
 * \brief Clears all mapping in this class.
 **/
{
    NOT_IMPLEMENTED_YET();
}


} // namespace penv
