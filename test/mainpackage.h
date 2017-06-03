/*
 * mainpackage.h - Declaration of the MainPackage class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */



#ifndef _TEST_MAINPACKAGE_H_
#define _TEST_MAINPACKAGE_H_


#include "../src/penv.h"

/**
 * \brief The MainPackage class is the main package in the
 * test application.
 **/
class MainPackage : public penv::Package
{
    public:
        MainPackage();
        ~MainPackage();
        virtual int OnInitPackage();
        virtual int OnExitPackage();

    protected:
        void OnTextWindow(penv::CommandEvent& event);
        void OnOglWindow(penv::CommandEvent& event);
        void OnHtmlWindow(penv::CommandEvent& event);

        void OnShowCommandDialog(penv::CommandEvent& event);
        void OnQuit(penv::CommandEvent& event);
        void OnAbout(penv::CommandEvent& event);

        void OnToogleDebugWindow(penv::CommandEvent& event);
        void OnToogleMessageWindow(penv::CommandEvent& event);
        void OnToogleProjectWindow(penv::CommandEvent& event);



        void OnToogleToolBarStandard(penv::CommandEvent& event);
        void OnToogleToolBarView(penv::CommandEvent& event);
        void OnToogleToolBarTest(penv::CommandEvent& event);


    private:


};




#endif // _TEST_MAINPACKAGE_H_
