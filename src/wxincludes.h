/*
 * wxincludes.h - Include Liste mit immer wieder benötigten Includes der
 *                wxWidgets Bibliothek
 *
 * Author: Steffen Ott
 * Datum: 18.09.2007
 *
 */

#ifndef _PENV_WXINCLUDES_H_
#define _PENV_WXINCLUDES_H_


#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>      // Erstmal Alles
    #include <wx/aui/aui.h> // Aui
    #include <wx/string.h>  // wxString
    #include <wx/panel.h>   // wxPanel
    #include <wx/hashmap.h> // wxHashMap
    #include <wx/event.h>   // wxEventHandler
    #include <wx/xml/xml.h>
    #include <wx/filename.h>
    #include <wx/listctrl.h>
    #include <wx/log.h>
    #include <wx/dir.h>
    #include <wx/file.h>
    #include <wx/treebase.h>
    #include <wx/treectrl.h>
    #include <wx/html/htmlwin.h>
    #include <wx/dnd.h>
    #include <wx/cmdline.h>
#endif




#endif  //_PENV_WXINCLUDES_H_
