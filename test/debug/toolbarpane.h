/*
 * toolbarpane.h - Declaration of the ToolBarPane class
 *
 * Author: Steffen Ott
 * Date: 04/08/2008
 *
 */


#ifndef _TEST_TOOLBARPANE_H_
#define _TEST_TOOLBARPANE_H_

#include "../../src/penv.h"
#include "textboxbuttonpanel.h"


/**
 * \brief Test pane for Toolbar informations.
 **/
class ToolBarPane : public penv::WindowPane
{
    public:
        ToolBarPane();
        virtual ~ToolBarPane();

        penv::Window* OnCreate(penv::ProjectItem* item);
        bool OnInitializeUI(wxWindow* parent);
        bool OnLayout();

};

//================================================================
class ToolBarPanel : public TextBoxButtonPanel
{
    public:
        ToolBarPanel(wxWindow* parent) : TextBoxButtonPanel(parent) { }
        void OnButtonClick(wxCommandEvent& event);
        void WriteToolBar(penv::ToolBar* toolbar, const wxString& hashid);
};

#endif // _TEST_TOOLBARPANE_H_
