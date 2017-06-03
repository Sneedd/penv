/*
 * scintilladocument.h - Declaration of the ScintillaDocument class
 *
 * Author: Steffen Ott
 * Date: 05/16/2008
 *
 */



#ifndef _TEST_SCINTILLADOCUMENT_H_
#define _TEST_SCINTILLADOCUMENT_H_


#include "../../src/penv.h"
#include <wx/wxScintilla/wxscintilla.h>

/**
 * \brief
 **/
class ScintillaDocument : public penv::WindowDocument
{
    public:
        ScintillaDocument();
        virtual ~ScintillaDocument();

    protected:
        virtual penv::Window* OnCreate(penv::ProjectItem* item = NULL);
        virtual bool OnInitializeUI(wxWindow* parent);
        virtual bool OnLayout();
        virtual bool OnLoad(const wxString& filename = wxEmptyString);
        virtual bool OnSave(const wxString& filename = wxEmptyString);
        virtual bool OnUpdate();

        virtual void OnPageAfterChange();
        virtual void OnPageBeforeHidden();
        virtual void OnDocumentModified(wxScintillaEvent& event);

    public:
        void ZoomIn();
        void ZoomOut();

    protected:
        wxPanel*     m_panel;
        wxScintilla* m_scintilla;
};



#endif // _TEST_SCINTILLADOCUMENT_H_
