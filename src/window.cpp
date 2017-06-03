/*
 * window.cpp - Implementation of the Window class
 *
 * Author: Steffen Ott
 * Date: 09/23/2007
 *
 */

#include "window.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"



namespace penv {


//----------------------------------------------------------------
Window::Window()
/**
 * \brief Constructor (private).
 **/
{
    m_window = NULL;
    m_type = penvWT_None;
    m_caption = wxEmptyString;
    m_id = wxEmptyString;
    m_classid = wxEmptyString;
    m_item = NULL;
    m_modified = false;
    m_registered = true;
}

//----------------------------------------------------------------
Window::Window(WindowType type, const wxString& classid)
/**
 * \brief Constructor, initializes the window class.
 * \param type The window type.
 * \param classid Class id or registered id of the window.
 **/
{
    m_classid = classid;
    m_type = type;
    m_id = wxEmptyString;
    m_caption = wxEmptyString;
    m_window = NULL;
    m_modified = false;
    m_item = NULL;
    m_registered = true;
}

//----------------------------------------------------------------
Window::~Window()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
void Window::Initialize(const wxString& caption, ProjectItem* item)
/**
 * \brief Initialize standard values for the window. This method
 * can be used before InitializeUI() in the OnCreate() method.
 * \param caption The caption of this window.
 * \param item Project item connected to the window.
 **/
{
    m_id = wxEmptyString;
    m_caption = caption;
    m_window = NULL;
    m_modified = false;
    SetProjectItem(item);
    m_registered = false;
}


//----------------------------------------------------------------
wxString Window::GetCaption() const
/**
 * \brief Returns the caption of this window. This method can be
 * overridden, to add other informations into the caption. For example
 * asterix if the content is changed.
 * \return Caption of the window.
 **/
{
    return (m_caption);
}



//----------------------------------------------------------------
void Window::SetCaption(const wxString& caption)
/**
 * \brief Sets the caption of the window.
 * \param caption Caption of this window.
 **/
{
    m_caption = caption;
}


//----------------------------------------------------------------
wxWindow* Window::GetWindow() const
/**
 * \brief Returns the internally used window of the window.
 * Normally this should be wxPanel, but could be any other window
 * which was derived from wxWindow.
 * \return Internally used window.
 **/
{
    return (m_window);
}


//----------------------------------------------------------------
WindowType Window::GetType() const
/**
 * \brief Returns the type of the window. There are two different
 * window types WindowPane and WindowDocument. See WindowType
 * enumerator for a detailed description.
 * \return Window type.
 **/
{
    return (m_type);
}


//----------------------------------------------------------------
void Window::Modified(bool modified)
/**
 * \brief Sets if the content of this window was modified.
 * \param modified True if content of this window was modified.
 **/
{
    m_modified = modified;
    // Update the caption if modified
    SetCaption(wxEmptyString);
}


//----------------------------------------------------------------
bool Window::IsModified() const
/**
 * \brief Checks if the content of window is modfied.
 * \return True if content was modified; false otherwise.
 **/
{
    return (m_modified);
}



//----------------------------------------------------------------
const wxString& Window::GetClassId() const
/**
 * \brief Returns the window class id of this window. See
 * WindowList for a more detailed description of differences
 * between class ids and open window ids.
 * \return Class id of this window.
 **/
{
    return (m_classid);
}





//----------------------------------------------------------------
const wxString& Window::GetId() const
/**
 * \brief Returns the open window id of this window. Can return
 * a empty string if this window is not opened.
 * \return Open window id or empty string.
 **/
{
    return (m_id);
}



//----------------------------------------------------------------
void Window::SetId(const wxString& id)
/**
 * \brief Sets the open window id. If window is already added
 * to the open list, then the open list will change.
 * \param id The new open window id of this window.
 **/
{
    // Prüfen ob die id schon existiert
    Environment* env = Environment::Get();
    if (env->GetWindowList()->ExistsWindow(id))
    {
        wxLogError(_T("[penv::Window::SetId] Cannot set id '%s', because this id already exists."), id.c_str());
        return;
    }
    // Prüfen, ob id geändert werden muss, wenn dieses Fenster
    //  mit der ID schon in der open list ist.
    if (env->GetWindowList()->GetWindow(m_id) == this) {
        env->GetWindowList()->ChangeWindowIds(m_id, id);
    }
    // Immer neu setzten
    m_id = id;
}


//----------------------------------------------------------------
void Window::SetProjectItem(ProjectItem* item)
/**
 * \brief Sets the project item connected to the window.
 * \param item Project item.
 **/
{
    m_item = item;
    if (m_item != NULL) {
        m_item->SetWindowId(m_id);
        if (m_item->GetWindowClassId() != m_classid) {
            wxLogWarning(_T("[penv::Window::SetProjectItem] Window class id '%s' of project item and this window class id '%s' are not equal."), m_item->GetWindowClassId().c_str(), m_classid.c_str());
        }
    }
}


//----------------------------------------------------------------
ProjectItem* Window::GetProjectItem() const
/**
 * \brief Returns the project item linked to this window.
 * This method can return NULL, if no project item is
 * linked to this window.
 * \return Project item.
 **/
{
    return (m_item);
}


//----------------------------------------------------------------
void Window::SetWindow(wxWindow* window)
/**
 * \brief Sets the wxWindow for this window.
 * This method is private and only used framework
 * internally.
 * \param window Internally used window.
 **/
{
    m_window = window;
}


//----------------------------------------------------------------
bool Window::InitializeUI()
/**
 * \brief Initializes the user interface components. This method
 * will be overridden in the WindowPane and WindowDocument classes.
 * \return True on success; false otherwise.
 **/
{
    return (false);
}



//----------------------------------------------------------------
bool Window::Layout()
/**
 * \brief Updates the layout for the user interface components. This
 * method will be overridden in the WindowPane and WindowDocument classes.
 * \return True on success; false otherwise.
 **/
{
    return (false);
}


//----------------------------------------------------------------
bool Window::Load(const wxString& filename)
/**
 * \brief Loads the given filename into the window. This method
 * will be overridden in the WindowPane and WindowDocument classes.
 * \param filename The given filename or empty string if the
 * filename from the connected project item should be used.
 * \return True on success; false otherwise.
 **/
{
    return (false);
}


//----------------------------------------------------------------
bool Window::Save(const wxString& filename)
/**
 * \brief Saves the given filename from the window. This method
 * will be overridden in the WindowPane and WindowDocument classes.
 * \param filename The given filename or empty string if the
 * filename from the connected project item should be used.
 * \return True on success; false otherwise.
 **/
{
    return (false);
}




//----------------------------------------------------------------
bool Window::Update()
/**
 * \brief Forces the window content to update. This method
 * will be overridden in the WindowPane and WindowDocument classes.
 * \return True on success; false otherwise.
 **/
{
    return (false);
}






} // namespace penv


