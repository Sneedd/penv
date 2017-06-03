/*
 * windowpane.h - Declaration of the WindowPane class
 *
 * Author: Steffen Ott
 * Date: 09/23/2007
 *
 */


#ifndef _PENV_WINDOWPANE_H_
#define _PENV_WINDOWPANE_H_

#include "wxincludes.h"
#include "window.h"


namespace penv {


/**
 * \brief The WindowsPane class represents a window pane on the framework.
 * Unlike WindowDocument this window can float and dock whereever the user
 * wants. The positions, size and other properties are stored in a
 * wxAuiPaneInfo object. The window pane can be mapped to a WindowDocument
 * so that if the window document changes the window pane can change.
 * This is useful then some documents doesn't need all window panes, which
 * are normally used as tool or support windows.<br>
 * If SetDestroyOnClose(true) is set, the window will be destroyed if the user
 * closes the window. If this is not set the window will hide and could make
 * visible again by calling <code>Visible(true)</code>. If you want to detroy
 * the window, you should set this window to SetDestroyOnClose(false) and call
 * the WindowList::Close() method.<br>
 * Also a WindowPane can hide or show by simply calling Visible(). This is
 * used by the WindowMapping features, because it only hides a window, instead
 * of destroying it.<br>
 * In order to implement a new WindowPane you should inherit from this class.
 * You must override at least the following methods: OnCreate() and OnInitializeUI().
 * All OnXXX() methods can be overridden to exchange the standrd functionallity,
 * see also the appropriate method. There is also an example in the OnCreate()
 * description for more information how to implement this method.
 **/
class WindowPane : public Window
{
    private:
        friend class WindowList;
    public:
        WindowPane(const wxString& classid);
        virtual ~WindowPane();

        wxString GetCaption() const;
        void SetCaption(const wxString& caption);

        wxSize GetPaneSize() const;
        void SetPaneSize(const wxSize& size);

        void SetPaneButtons(bool close=true, bool maximize=true);
        void SetDestroyOnClose(bool destroyonclose=true);
        bool IsDestroyOnClose() const;

        bool IsVisible() const;
        void Visible(bool visible=true);


        bool InitializeUI();
        bool Layout();
        bool Load(const wxString& filename = wxEmptyString);
        bool Save(const wxString& filename = wxEmptyString);
        bool Update();

    protected:
        virtual Window* OnCreate(ProjectItem* item);
        virtual bool OnInitializeUI(wxWindow* parent);
        virtual bool OnLayout();
        virtual bool OnLoad(const wxString& filename = wxEmptyString);
        virtual bool OnSave(const wxString& filename = wxEmptyString);
        virtual bool OnClose(bool& cancel);
        virtual bool OnUpdate();
        virtual bool OnVisibleChanging(bool visibility);

    protected:
        wxAuiPaneInfo& GetPaneInfo();
        void SetPaneInfo(const wxAuiPaneInfo& paneinfo);


    private:
        bool m_visible;
        bool m_destroyonclose;
        wxAuiPaneInfo m_paneinfo;

};

} // namespace penv

#endif // _PENV_WINDOWPANE_H_
