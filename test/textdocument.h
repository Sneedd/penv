/*
 * textdocument.h - Declaration of the TextDocument class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */


#ifndef _TEST_TEXTDOCUMENT_H_
#define _TEST_TEXTDOCUMENT_H_

#include "../src/penv.h"


/**
 * \brief The TextDocument class is just a TextCtrl in a document.
 * There is no additional functionality and there for only for testing.
 **/
class TextDocument : public penv::WindowDocument
{
    public:
        TextDocument();
        ~TextDocument();

        penv::Window* OnCreate(penv::ProjectItem* item);
        bool OnInitializeUI(wxWindow* parent);
        bool OnLayout();

    private:
        wxTextCtrl* m_textctrl;

};

#endif // _TEST_TEXTDOCUMENT_H_


