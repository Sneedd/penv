/*
 * windowmapping.h - Declaration of the WindowMapping class
 *
 * Author: Steffen Ott
 * Date: 03/21/2008
 *
 */

#ifndef _PENV_WINDOWMAPPING_H_
#define _PENV_WINDOWMAPPING_H_

#include "wxincludes.h"
#include "windowpane.h"


namespace penv {

/**
 * \brief This class is for the mapping of window panes to
 * a window document. Window documents cannot be mapped
 * to one pane. The mapping is necessary because not all window
 * panes should be available for a document. So this mapping
 * must be spezified.
 * \todo Implement Unregister() and Clear() methods.
 **/
class WindowMapping
{
    private:
        WindowMapping();
    public:
        WindowMapping(const wxString& windowdocumentid);
        ~WindowMapping();

        void Register(const wxString& windowpaneid);
        void UnRegister(const wxString& windowpaneid);
        void Clear();

    private:
        wxString m_windowdocumentid;
        WX_DECLARE_STRING_HASH_MAP(WindowPane*, WindowsHashMap);
        WindowsHashMap* m_mappedpanes;
};


} // namespace penv

#endif // _PENV_WINDOWMAPPING_H_
