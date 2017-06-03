/*
 * windowdocument.h - Dekleration der WindowDocument Klasse
 *
 * Author: Steffen Ott
 * Datum: 20.09.2007
 *
 */

#ifndef _PENV_WINDOWDOCUMENT_H_
#define _PENV_WINDOWDOCUMENT_H_

#include "wxincludes.h"
#include "window.h"
#include "projectitem.h"



namespace penv {

/**
 * \brief The WindowsDocument class represents a window pane on the framework.
 * Unlike WindowPane this window is displayed in the center notebook.<br>
 * In order to implement a new WindowsDocument you should inherit from this class.
 * You must override at least the following methods: OnCreate() and OnInitializeUI().
 * All OnXXX() methods can be overridden to exchange the standrd functionallity,
 * see also the appropriate method. There is also an example in the OnCreate()
 * description for more information how to implement this method.
 **/
class WindowDocument : public Window
{
    private:
        friend class WindowList;
    public:
        WindowDocument(const wxString& classid);
        virtual ~WindowDocument();

        wxBitmap& GetBitmap();
        void SetBitmap(const wxBitmap& bitmap);
        wxString GetCaption() const;
        void SetCaption(const wxString& caption = wxEmptyString);

        bool IsSelected() const;

        bool InitializeUI();
        bool Layout();
        bool Load(const wxString& filename = wxEmptyString);
        bool Save(const wxString& filename = wxEmptyString);
        bool Update();

    protected:
        virtual Window* OnCreate(ProjectItem* item = NULL);
        virtual bool OnInitializeUI(wxWindow* parent);
        virtual bool OnLayout();
        virtual bool OnLoad(const wxString& filename = wxEmptyString);
        virtual bool OnSave(const wxString& filename = wxEmptyString);
        virtual bool OnClose(bool& cancel);
        virtual bool OnUpdate();

        virtual void OnPageBeforeChange();
        virtual void OnPageAfterChange();
        virtual void OnPageBeforeHidden();
        virtual void OnPageAfterHidden();

    protected:
        wxBitmap m_icon;

};

} // namespace penv

#endif // _PENV_WINDOWDOCUMENT_H_
