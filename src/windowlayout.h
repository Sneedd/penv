/*
 * windowlayout.h - Declaration of the WindowLayout class
 *
 * Author: Steffen Ott
 * Date: 05/01/2008
 *
 */

#ifndef _PENV_WINDOWLAYOUT_H_
#define _PENV_WINDOWLAYOUT_H_


#include "wxincludes.h"

namespace penv {


/**
 * \brief The WindowLayout class represent the window layout
 * for opened windows. The layout for the for opened windows
 * cannot be updated automatically. So this class is returned
 * from WindowList::ReadNodeOpenWindows() to help the user
 * to update the Layout manually.
 * \todo This class needs a complete rewrite, don't use it.
 **/
class WindowLayout
{
    public:
        WindowLayout();
        ~WindowLayout();

        const wxAuiPaneInfo& GetPaneInfo(size_t index);
        const wxString& GetClassId(size_t index);
        const wxString& GetProjectItem(size_t index);
        void Add(const wxAuiPaneInfo& paneinfo, const wxString& classid, const wxString& projectitem);
        size_t Count() const;


    private:
        wxAuiPaneInfoArray m_array;
        wxArrayString m_classidarray;
        wxArrayString m_projectitemarray;
};


} // namespace penv

#endif // _PENV_WINDOWLAYOUT_H_
