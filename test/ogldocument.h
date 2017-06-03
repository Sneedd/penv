/*
 * ogldocument.h - Declaration of the OGLDocument class
 *
 * Author: Steffen Ott
 * Date: 05/17/2008
 *
 */

#ifndef _TEST_OGLDOCUMENT_H_
#define _TEST_OGLDOCUMENT_H_


#include "../src/penv.h"
#include <wx/ogl/ogl.h>


/**
 * \brief The OGLDocument class is a OGL window in a document.
 * There is no additional functionality and therefore only for testing.
 **/
class OGLDocument : public penv::WindowDocument
{
    public:
        OGLDocument();
        virtual ~OGLDocument();

        penv::Window* OnCreate(penv::ProjectItem* item);
        bool OnInitializeUI(wxWindow* parent);
        bool OnLayout();

    private:
        wxShapeCanvas* m_canvas;
        wxPanel* m_panel;

};



#endif // _TEST_OGLDOCUMENT_H_
