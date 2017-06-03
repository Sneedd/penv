/*
 * htmlpane.h - Declaration of the HtmlPane class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */


#ifndef _TEST_HTMLPANE_H_
#define _TEST_HTMLPANE_H_

#include "../src/penv.h"



/**
 * \brief The HtmlPane class is a HTML window in a pane.
 * There is no additional functionality and therefore only for testing.
 **/
class HtmlPane : public penv::WindowPane
{
    public:
        HtmlPane();
        virtual ~HtmlPane();
    protected:
        penv::Window* OnCreate(penv::ProjectItem* item);
        bool OnInitializeUI(wxWindow* parent);
        bool OnLayout();

    private:
        wxHtmlWindow* m_htmlwindow;

};



#endif // _TEST_HTMLPANE_H_
