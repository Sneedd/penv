/*
 * editorpackage.h - Declaration of the EditorPackage class
 *
 * Author: Steffen Ott
 * Date: 05/16/2008
 *
 */

#ifndef _TEST_EDITORPACKAGE_H_
#define _TEST_EDITORPACKAGE_H_

#include "../../src/penv.h"


/**
 * \brief This Package is for the Editor functionality
 * in the test application.
 **/
class EditorPackage : public penv::Package
{
    public:
        EditorPackage();
        ~EditorPackage();

    private:
        int OnInitPackage();
        int OnExitPackage();

        void OnFileNew(penv::CommandEvent& event);
        void OnFileOpen(penv::CommandEvent& event);
        void OnFileSave(penv::CommandEvent& event);
        void OnFileSaveAs(penv::CommandEvent& event);
        void OnFileClose(penv::CommandEvent& event);
        void OnFileQuit(penv::CommandEvent& event);
        void OnZoomOut(penv::CommandEvent& event);
        void OnZoomIn(penv::CommandEvent& event);

        void OnProjectItemFileActivated(penv::CommandEvent& event);
};



#endif // _TEST_EDITORPACKAGE_H_


