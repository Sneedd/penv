/*
 * windowpane.cpp - Implementation of the WindowPane class
 *
 * Author: Steffen Ott
 * Date: 09/23/2007
 *
 */

#include "windowpane.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"


namespace penv {


//----------------------------------------------------------------
WindowPane::WindowPane(const wxString& classid)
    : Window(penvWT_Pane, classid)
/**
 * \brief Initializes the WindowDocument object. The given
 * class id must be unique, see also WindowList class description.
 * \param classid Class id of this window.
 **/
{
    // Default pane properties
    m_paneinfo.
        // Dock properties
        Dock().Dockable(true).
        TopDockable(true).BottomDockable(true).RightDockable(true).LeftDockable(true).
        // Floating properties
        Floatable(true).FloatingPosition(100,100).FloatingSize(100,100).
        // Size properties
        BestSize(100,100).MinSize(100,100).MaxSize(-1,-1).Resizable(true).
        // Button properties
        CloseButton(true).MaximizeButton(true).MinimizeButton(false).PinButton(false).
        // Other properties
        CaptionVisible(true).DestroyOnClose(false).Movable(true).PaneBorder(true).
        Gripper(false).GripperTop(false)
    ;
    m_visible = true;
    m_destroyonclose = true;
}



//----------------------------------------------------------------
WindowPane::~WindowPane()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
wxString WindowPane::GetCaption() const
/**
 * \brief Returns the caption of the document.
 * \return Caption of the pane.
 **/
{
    return (m_caption);
}


//----------------------------------------------------------------
void WindowPane::SetCaption(const wxString& caption)
/**
 * \brief Sets the caption of the window. Automatically
 * updates caption in window pane if pane is visible.
 * \param caption Caption of this window.
 **/
{
    if (!caption.IsEmpty()) {
        m_caption = caption;
    }
    if (m_visible) {
        Environment* env = Environment::Get();
        env->GetFrame()->GetManager()->GetPane(m_window).Caption(GetCaption());
        env->GetFrame()->GetManager()->Update();
    }
}



//----------------------------------------------------------------
wxSize WindowPane::GetPaneSize() const
/**
 * \brief Returns the pane size of this window.
 * It will always return the bestsize of the pane, except
 * when the windowpane is a registered pane. If this pane
 * is a registered window, then the previous definition is
 * returned. See also SetPaneSize().
 * \return Best pane size of the window pane.
 **/
{
    if (!m_visible) {
        // Get PaneInfo and return the actual size of the window pane
        Environment* env = Environment::Get();
        wxAuiPaneInfo info = env->GetFrame()->GetManager()->GetPane(m_window);
        // If IsOk() returns false, window is not found
        if (info.IsOk()) return (info.best_size);
    }
    else
    {
        return (m_paneinfo.best_size);
    }
    return (wxSize(-1,-1));
}


//----------------------------------------------------------------
void WindowPane::SetPaneSize(const wxSize& size)
/**
 * \brief Sets the best pane size for this window pane.
 * The window manager will be imediatly updated, if this is a
 * open window.
 * \param size New pane size.
 **/
{
    if (m_visible)
    {
        if (unlikely(m_window == NULL)) {
            wxLogWarning(_T("[penv::WindowPane::SetPaneSize] Window in pane with id \"%s\" is NULL. Cannot change size."), m_id.c_str());
            m_paneinfo.BestSize(size).FloatingSize(size);
            return;
        }
        // Get manager and update pane size
        Environment* env = Environment::Get();
        wxAuiManager* manager = env->GetFrame()->GetManager();
        manager->GetPane(m_window).BestSize(size).FloatingSize(size);
        // Manager should be updated to accept the change
        manager->Update();
    }
    else
    {
        m_paneinfo.BestSize(size).FloatingSize(size);
    }
}


//----------------------------------------------------------------
void WindowPane::SetPaneButtons(bool close, bool maximize)
/**
 * \brief Sets the pane buttons for a window pane.
 * Because this is a WindowPane object it will always
 * return penvWT_Pane.
 * \return Window type.
 **/
{
   if (!m_visible)
   {
        // Get manager and update pane size
        Environment* env = Environment::Get();
        wxAuiManager* manager = env->GetFrame()->GetManager();
        manager->GetPane(m_window).MaximizeButton(maximize).CloseButton(close);
        // Manager should be updated to accept the change
        manager->Update();
    }
    else
    {
        m_paneinfo.MaximizeButton(maximize).CloseButton(close);
    }
}



//----------------------------------------------------------------
void WindowPane::SetDestroyOnClose(bool destroyonclose)
/**
 * \brief Sets if the window pane will be hiding or destroyed,
 * when close button is pressed. The default setting is true.
 * \param destroyonclose True to destroy window pane instead of hiding it.
 **/
{
    m_destroyonclose = destroyonclose;
}



//----------------------------------------------------------------
bool WindowPane::IsDestroyOnClose() const
/**
 * \brief Returns if the detroy on close option is set. If this option
 * is set to true, then the window will be detroyed on close. If this
 * option is set to false then the window will hide on close, even when
 * WindowList::Close() is called. If the window is just hidden, then
 * it can be made visible when Visible(true) is called.
 * \return True if destroy an close flag is set.
 **/
{
    return (m_destroyonclose);
}



//----------------------------------------------------------------
bool WindowPane::IsVisible() const
/**
 * \brief Returns if the window pane is visible.
 * \return True when window pane is visible; false otherwise.
 **/
{
    return (m_visible);
}


//----------------------------------------------------------------
void WindowPane::Visible(bool visible)
/**
 * \brief Sets the visibility of the window pane.
 * \param visible True when window pane should be visible;
 * false to hide it.
 **/
{
    if (m_visible == visible) return;
    if (!OnVisibleChanging(visible)) return;
    wxAuiManager* manager = Environment::Get()->GetFrame()->GetManager();
    if (visible)
    {
        // Show pane
        manager->AddPane(m_window, m_paneinfo);
        m_window->Show();
    }
    else
    {
        // Hide pane
        // First save pane info
        m_paneinfo = manager->GetPane(m_window);
        // Then detach pane
        m_window->Hide();
        manager->DetachPane(m_window);
    }
    manager->Update();
    m_visible = visible;
}


//----------------------------------------------------------------
bool WindowPane::InitializeUI()
/**
 * \brief Initializes the user interface components. This method
 * calls OnInitializeUI().
 * \return True on success; false otherwise.
 **/
{
    Environment* env = Environment::Get();
    wxWindow* parent = (wxWindow*)env->GetFrame();
    return (OnInitializeUI(parent));
}



//----------------------------------------------------------------
bool WindowPane::Layout()
/**
 * \brief Updates the layout for the user interface components.
 * This method calls OnLayout().
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(m_window == NULL)) {
        wxLogError(_T("[penv::WindowPane::Layout] Cannot update the window layout of a not initialized window."));
        return (false);
    }
    return (OnLayout());
}




//----------------------------------------------------------------
bool WindowPane::Load(const wxString& filename)
/**
 * \brief Loads a file into the window pane. If you want
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
                return (true);
            }
        } else {
            if (OnLoad(filename)) {
                OnUpdate();
                return (true);
            }
        }
    }
    return (false);
}




//----------------------------------------------------------------
bool WindowPane::Save(const wxString& filename)
/**
 * \brief Saves a file from the window pane. If you want
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
                return (true);
            }
        } else {
            if (OnSave(filename)) {
                return (true);
            }
        }
    }
    return (false);
}


//----------------------------------------------------------------
bool WindowPane::Update()
/**
 * \brief Forces the window content to update. This method simply
 * calls OnUpdate().
 * \return True on success; false otherwise.
 **/
{
    if (OnUpdate()) {
        wxLogWarning(_T("[penv::Window::Update] Update failed for window with class id '%s' and open window id '%s'."), m_classid.c_str(), m_id.c_str());
        return (false);
    }
    return (true);
}



//----------------------------------------------------------------
Window* WindowPane::OnCreate(ProjectItem* item)
/**
 * \brief This method is called to create a window. It must
 * be overridden in a inherited class. You can follow this
 * example to implement the OnCreate() method.<br>
 * Example:<br>
 * <code>
 * Window* InheritedWindowPane::OnCreate(ProjectItem* item)
 * {
 *   InheritedWindowPane* win = new InheritedWindowPane(m_classid);
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
    WindowPane* win = new WindowPane(m_classid);
    win->Initialize(_T("warning"), NULL);
    win->InitializeUI();
    win->Layout();
    return (win);
}



//----------------------------------------------------------------
bool WindowPane::OnInitializeUI(wxWindow* parent)
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
        _T("Standard implementation of WindowPane::OnInitalizeUI(). Please override the standard implementation of WindowPane::OnInitalizeUI() and WindowPane::OnCreate()."),
        wxDefaultPosition, wxDefaultSize, 0);
	label->Wrap(380);
	wxFlexGridSizer* sizer;
	sizer = new wxFlexGridSizer(1, 1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizer->Add(label, 0, wxALIGN_CENTER|wxALL, 5);
	panel->SetSizer( sizer );
	m_window = (wxWindow*)panel;
    return (true);
}



//----------------------------------------------------------------
bool WindowPane::OnLayout()
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
bool WindowPane::OnLoad(const wxString& filename)
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
bool WindowPane::OnSave(const wxString& filename)
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
bool WindowPane::OnClose(bool& cancel)
/**
 * \brief This method is called to close the window.
 * The standard implementation will always return true.<br>
 * The close action can be canceled if cancel will be set
 * to true.
 * \param cancel Set this to false if you want to cancel the
 * close action.
 * \return True on success; false otherwise.
 **/
{
    return (true);
}



//----------------------------------------------------------------
bool WindowPane::OnUpdate()
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
bool WindowPane::OnVisibleChanging(bool visibility)
/**
 * \brief This method is called bevore visibility was changed.
 * The standard implementation simply ever returns true. You
 * can cancel the change of visibility by returning false.
 * \param visibility The new visibility of the window. True
 * to show window. False to hide it.
 * \return True to change visibility.
 **/
{
    return (true);
}



//----------------------------------------------------------------
wxAuiPaneInfo& WindowPane::GetPaneInfo()
/**
 * \brief Returns the pane info for this window.
 * Returns the internally stored pane info if the window
 * is not visible, otherwise this method will return the
 * pane info which is stored in the wxAuiManager.<br>
 * Because this framework overrides some functionality of the
 * Aui Framework, you cannot set all properties of a
 * aui pane. You can use this method to set some properties
 * for the wxAuiPaneInfo object. The following properties
 * should already work:<br>
 * Dock(), Dockable(), TopDockable(), BottomDockable(),
 * RightDockable(), LeftDockable(), Floatable(),
 * FloatingPosition(), MinSize(), MaxSize(), Resizable(),
 * MinimizeButton(), PinButton(), CaptionVisible(),
 * Movable(), PaneBorder(), Gripper(), GripperTop().<br>
 * The following properties doesn't work:<br>
 * DestroyOnClose() use WindowPane::SetDestroyOnClose() instead<br>
 * For the methods FloatingSize() and BestSize() please
 * use the WindowPane::SetPaneSize() method.<br>
 * For the methods CloseButton() and MaximizeButton()
 * use the WindowPane::SetPaneButtons() method.
 * \return Pane info for this window.
 **/
{
    if (m_visible)
    {
        Environment* env = Environment::Get();
        return (env->GetFrame()->GetManager()->GetPane(m_window));
    }
    else
    {
        return (m_paneinfo);
    }
}


//----------------------------------------------------------------
void WindowPane::SetPaneInfo(const wxAuiPaneInfo& paneinfo)
/**
 * \brief Sets the pane info for this window.
 * Only sets the internally stored pane info. If the
 * window is visible, this method will not update the
 * wxAuiManager. See also GetPaneInfo().
 * \param paneinfo Pane info for this window.
 **/
{
    m_paneinfo = paneinfo;
}






} // namespace penv



