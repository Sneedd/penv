#ifndef _TEST_AUIINFOPANE_H_
#define _TEST_AUIINFOPANE_H_

#include "../../src/penv.h"
#include "textboxbuttonpanel.h"


/**
 * \brief Test pane for Toolbar informations.
 **/
class AuiInfoPane : public penv::WindowPane
{
    public:
        AuiInfoPane();
        virtual ~AuiInfoPane();

        penv::Window* OnCreate(penv::ProjectItem* item);
        bool OnInitializeUI(wxWindow* parent);
        bool OnLayout();

};

//================================================================
class AuiInfoPanePanel : public TextBoxButtonPanel
{
    public:
        AuiInfoPanePanel(wxWindow* parent) : TextBoxButtonPanel(parent) { }
        void OnButtonClick(wxCommandEvent& event);
        void WriteNotebookPageInfo(wxAuiNotebook* notebook, size_t index);
        void WritePaneInfo(wxAuiPaneInfo& paneinfo);
};


#endif // _TEST_AUIINFOPANE_H_
