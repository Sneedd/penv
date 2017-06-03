/*
 * eventdocument.h - Declaration of the EventDocument class
 *
 * Author: Steffen Ott
 * Date: 04/08/2008
 *
 */


#ifndef _TEST_EVENTDOCUMENT_H
#define _TEST_EVENTDOCUMENT_H

#include "../../src/penv.h"
#include "textboxbuttonpanel.h"

/**
 * \brief This class prints out the event table(s)
 * for every component who uses the wxEvtHandler.
 **/
class EventDocument : public penv::WindowDocument
{
    public:
        EventDocument();
        virtual ~EventDocument();

        penv::Window* OnCreate(penv::ProjectItem* item);
        bool OnInitializeUI(wxWindow* parent);
        bool OnLayout();
};

//================================================================
class EventDocumentPanel : public TextBoxButtonPanel
{
    public:
        EventDocumentPanel(wxWindow* parent) : TextBoxButtonPanel(parent) { }
        void OnButtonClick(wxCommandEvent& event);
        void WriteEvtHandler(wxEvtHandler* handler, const wxString& information);
        wxString ResolveEventTypes(int eventtype);
};


#endif // _TEST_EVENTDOCUMENT_H
