/*
 * debugpackage.h - Declaration of the DebugPackage class
 *
 * Author: Steffen Ott
 * Date: 05/06/2008
 *
 */

#ifndef _PENV_TEST_DEBUGPACKAGE_H_
#define _PENV_TEST_DEBUGPACKAGE_H_

#include "../../src/penv.h"


/**
 * \brief The DebugPackage class adds functionality to the test
 * application to open debug windows. The windows AuiInfoPane,
 * EventDocument, ToolBarPane, ... are for debugging and viewing
 * additional information about the framework internal.
 **/
class DebugPackage : public penv::Package
{
    public:
        DebugPackage();
        ~DebugPackage();

    protected:
        int OnInitPackage();
        int OnExitPackage();

        void OnShowEventDocument(penv::CommandEvent& event);
        void OnShowToolBarPane(penv::CommandEvent& event);
        void OnShowAuiInfoPane(penv::CommandEvent& event);
        void OnShowUnitTestDocument(penv::CommandEvent& event);
        void OnPrintProjectManagerHierarchy(penv::CommandEvent& event);
        void OnPrintProjectTreeCtrl(penv::CommandEvent& event);


    private:
};


#endif // _PENV_TEST_DEBUGPACKAGE_H_
