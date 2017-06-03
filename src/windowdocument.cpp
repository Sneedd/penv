/*
 * windowdocument.cpp - Implementation of the WindowDocument class
 *
 * Author: Steffen Ott
 * Date: 09/20/2007
 *
 */

#include "windowdocument.h"
#include "environment.h"
#include "expect.h"
#include "penvhelper.h"



namespace penv {


//----------------------------------------------------------------
WindowDocument::WindowDocument(const wxString& classid)
    : Window(penvWT_Document, classid)
/**
 * \brief Initializes the WindowDocument object. The given
 * class id must be unique, see also WindowList class description.
 * \param classid Class id of this window.
 **/
{
}



//----------------------------------------------------------------
WindowDocument::~WindowDocument()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
wxBitmap& WindowDocument::GetBitmap()
/**
 * \brief Returns the bitmap for the window document. Can
 * return a wxNullBitmap if no bitmap is set. The bitmap
 * appears in the tab of the notebook.
 * \return Bitmap for the document or wxNullBitmap.
 **/
{
    return (m_icon);
}



//----------------------------------------------------------------
void WindowDocument::SetBitmap(const wxBitmap& bitmap)
/**
 * \brief Sets the bitmap for the window document. Also updates
 * the bitmap of the tab in the notebook, if the document
 * is visible.
 * \param bitmap Bitmap for the window document.
 **/
{
    Environment* env = Environment::Get();
    m_icon = bitmap;
    int index = env->GetFrame()->GetNotebook()->GetPageIndex(m_window);
    if (index != -1) {
        env->GetFrame()->GetNotebook()->SetPageBitmap(index, m_icon);
        env->GetFrame()->GetNotebook()->Update();
    }
}



//----------------------------------------------------------------
wxString WindowDocument::GetCaption() const
/**
 * \brief Returns the caption of the document. This method
 * automatically adds a asterix to the caption if the caption
 * was modified.
 * \return Caption of the document.
 **/
{
    wxString caption;
    if (m_modified) caption += _T("* ");
    caption += m_caption;
    return (caption);
}


//----------------------------------------------------------------
void WindowDocument::SetCaption(const wxString& caption)
/**
 * \brief Sets the caption of the window. This method can
 * be used to set the caption in a tab again, if you call this
 * method with an empty string. This means that the caption
 * from GetCaption() will be inserted into the notebook again.
 * \param caption Caption of this window.
 **/
{
    if (!caption.IsEmpty()) {
        m_caption = caption;
    }
    Environment* env = Environment::Get();
    int index = env->GetFrame()->GetNotebook()->GetPageIndex(m_window);
    if (index != -1) {
        env->GetFrame()->GetNotebook()->SetPageText(index, GetCaption());
        env->GetFrame()->GetNotebook()->Update();
    }
}



//----------------------------------------------------------------
bool WindowDocument::IsSelected() const
/**
 * \brief Returns true if the WindowDocument is selected in
 * the center notebook.
 * \return True if window document is selected.
 **/
{
    if (m_id.IsEmpty()) return (false);
    wxAuiNotebook* notebook = Environment::Get()->GetFrame()->GetNotebook();
    return (notebook->GetPage(notebook->GetSelection()) == m_window);
}


//----------------------------------------------------------------
bool WindowDocument::InitializeUI()
/**
 * \brief Initializes the user interface components. This method
 * calls OnInitializeUI().
 * \return True on success; false otherwise.
 **/
{
    Environment* env = Environment::Get();
    wxWindow* parent = (wxWindow*)env->GetFrame()->GetNotebook();
    return (OnInitializeUI(parent));
}



//----------------------------------------------------------------
bool WindowDocument::Layout()
/**
 * \brief Updates the layout for the user interface components.
 * This method calls OnLayout().
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_window == NULL)) {
        wxLogError(_T("[penv::WindowDocument::Layout] Cannot update the window layout of a not initialized window."));
        return (false);
    }
    return (OnLayout());
}


//----------------------------------------------------------------
bool WindowDocument::Load(const wxString& filename)
/**
 * \brief Loads a file into the window document. If you want
 * to use the project items filename, you should call this
 * method with a empty string. If you provide a filename
 * then the provided filename will always have a higher
 * priority then the project items filename. This
 * method also calls OnUpdate() after the file was successfully
 * loaded.
 * \param filename File name or empty string.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_item == NULL)) {
        if (unlikely(!filename.IsEmpty())) {
            if (OnLoad(filename)) {
                OnUpdate();
                this->Modified(false);
                return (true);
            }
        }
    }
    else
    {
        // NOTE: Angegebener Dateiname hat höherer
        //       Priorität als ProjectItem!
        if (unlikely(filename.IsEmpty())) {
            if (OnLoad(m_item->GetPathString())) {
                OnUpdate();
                this->Modified(false);
                return (true);
            }
        } else {
            if (OnLoad(filename)) {
                OnUpdate();
                this->Modified(false);
                return (true);
            }
        }
    }
    return (false);
}




//----------------------------------------------------------------
bool WindowDocument::Save(const wxString& filename)
/**
 * \brief Saves a file from the window document. If you want
 * to use the project items filename, you should call this
 * method with a empty string. If you provide a filename
 * then the provided filename will always have a higher
 * priority then the project items filename.
 * \param filename File name or empty string.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_item == NULL)) {
        if (unlikely(!filename.IsEmpty())) {
            if (OnSave(filename)) {
                this->Modified(false);
                return (true);
            }
        }
    }
    else
    {
        // NOTE: Angegebener Dateiname hat höherer
        //       Priorität als ProjectItem!
        if (unlikely(filename.IsEmpty())) {
            if (OnSave(m_item->GetPathString())) {
                this->Modified(false);
                return (true);
            }
        } else {
            if (OnSave(filename)) {
                this->Modified(false);
                return (true);
            }
        }
    }
    return (false);
}


//----------------------------------------------------------------
bool WindowDocument::Update()
/**
 * \brief Forces the window content to update. This method simply
 * calls OnUpdate().
 * \return True on success; false otherwise.
 **/
{
    if (!OnUpdate()) {
        wxLogWarning(_T("[penv::Window::Update] Update failed for window with class id '%s' and open window id '%s'."), m_classid.c_str(), m_id.c_str());
        return (false);
    }
    return (true);
}




//----------------------------------------------------------------
Window* WindowDocument::OnCreate(ProjectItem* item)
/**
 * \brief This method is called to create a window. It must
 * be overridden in a inherited class. You can follow this
 * example to implement the OnCreate() method.<br>
 * Example:<br>
 * <code>
 * Window* InheritedWindowDocument::OnCreate(ProjectItem* item)
 * {
 *   InheritedWindowDocument* win = new InheritedWindowDocument(m_classid);
 *   wxString name = _T("noname");
 *   if (item != NULL) name = item->GetName();
 *   win->Initialize(name, item);
 *   win->InitializeUI();
 *   win->Layout();
 *   return (win);
 * }
 * </code>
 * \param item Project item or NULL.
 * \return Created window or NULL on error.
 **/
{
    WindowDocument* win = new WindowDocument(m_classid);
    win->Initialize(_T("warning"), NULL);
    win->InitializeUI();
    win->Layout();
    return (win);
}


//----------------------------------------------------------------
bool WindowDocument::OnInitializeUI(wxWindow* parent)
/**
 * \brief This method is called to initialize the window user
 * interface components. The standard implementation is the
 * create a panel with a label to inform that this method must
 * be overridden.
 * \param parent The parent window for this window.
 * \return True on success; false otherwise.
 **/
{
    // Symbolisch ein einfaches Panel initalisieren
    wxPanel* panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition,
        wxDefaultSize, wxTAB_TRAVERSAL);
	wxStaticText* label = new wxStaticText(panel, wxID_ANY,
        _T("Standard implementation of WindowDocument::OnInitalizeUI(). Please override the standard implementation of WindowDocument::OnInitalizeUI() and WindowDocument::OnCreate()."),
        wxDefaultPosition, wxDefaultSize, 0);
	label->Wrap(380);
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer(1, 1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizer->Add(label, 0, wxALIGN_CENTER|wxALL, 5);
	panel->SetSizer( sizer );    m_window = (wxWindow*)panel;
    return (true);
}


//----------------------------------------------------------------
bool WindowDocument::OnLayout()
/**
 * \brief This method is called to update the window layout.
 * The standard implementation is to call the wxWindow::Layout()
 * method of the window. There is no need to check if m_window
 * exists because this will be done in the Layout() method, which
 * calls this method.
 * \return True on success; false otherwise.
 **/
{
	m_window->Layout();
	return (true);
}



//----------------------------------------------------------------
bool WindowDocument::OnLoad(const wxString& filename)
/**
 * \brief This method is called to load the window content.
 * This method does not have a standard implementation and
 * will always return false.
 * \param filename The filename from where to load the content.
 * \return True on success; false otherwise.
 **/
{
    return (false);
}


//----------------------------------------------------------------
bool WindowDocument::OnSave(const wxString& filename)
/**
 * \brief This method is called to save the window content.
 * This method does not have a standard implementation and
 * will always return false.
 * \param filename The filename where to save the content.
 * \return True on success; false otherwise.
 **/
{
    return (false);
}


//----------------------------------------------------------------
bool WindowDocument::OnClose(bool& cancel)
/**
 * \brief This method is called to close the window.
 * The standard implementation is to open a MessageBox
 * if the window content is modified. This implementation
 * can be overridden in the inherited class. <br>
 * The close action can be canceled if cancel will be set
 * to true.
 * \param cancel Set this to false if you want to cancel the
 * close action.
 * \return True on success; false otherwise.
 **/
{
    if (m_modified) {
        // If document is modified, it should be saved
        Environment* env = Environment::Get();
        wxWindow* parent = env->GetFrame();
        wxString message = wxString::Format(_("Do you want to save the document \"%s\" before closing?"), m_caption.c_str());
        wxString caption = _("Question: Save document ...");
        wxMessageDialog msgbox(parent, message, caption, wxCANCEL|wxYES_NO|wxYES_DEFAULT);
        int id = msgbox.ShowModal();
        if (id == wxID_YES) {
            // Save the file
            if (unlikely(!Save())) {
                wxLogError(_T("[penv::WindowDocument::OnClose] Could not save document \"%s\"."), m_caption.c_str());
                return (false);
            }
        }
        else if (id == wxID_CANCEL) {
            cancel = true;
        }
    }
    return (true);
}


//----------------------------------------------------------------
bool WindowDocument::OnUpdate()
/**
 * \brief This method is called to update the window. The
 * standard implementation is to call wxWindow::Update()
 * but can be overridden in the inherited class.
 * \return True on success; false otherwise.
 **/
{
    m_window->Update();
    return (true);
}



//----------------------------------------------------------------
void WindowDocument::OnPageBeforeChange()
/**
 * \brief This method will be called before this page is shown.
 * This method can be overridden in the inherited class.
 **/
{
}


//----------------------------------------------------------------
void WindowDocument::OnPageAfterChange()
/**
 * \brief This method will be called after this page is shown.
 * This method can be overridden in the inherited class. The
 * standard implementation just calls Update().
 **/
{
    Update();
}


//----------------------------------------------------------------
void WindowDocument::OnPageBeforeHidden()
/**
 * \brief This method will be called before this page is hidden.
 * This method can be overridden in the inherited class.
 **/
{
}

//----------------------------------------------------------------
void WindowDocument::OnPageAfterHidden()
/**
 * \brief This method will be called after this page is hidden.
 * This method can be overridden in the inherited class.
 **/
{
}









} // namespace penv



