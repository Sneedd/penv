/*
 * commandmenutreeviewdroptarget.h - Declaration of the CommandMenuTreeViewDropTarget class
 *
 * Author: Steffen Ott
 * Date: 04/28/2008
 *
 */


#ifndef _PENV_UI_COMMANDMENUTREEVIEWDROPTARGET_H_
#define _PENV_UI_COMMANDMENUTREEVIEWDROPTARGET_H_

#include "../wxincludes.h"


namespace penv {


/**
 * \brief The CommandMenuTreeViewDropTarget is the target for
 * a drag and drop operation from the listctrl in the CommandMenuWindow
 * panel. Because this drop target is inherited from wxTextDropTarget,
 * the target only except strings, which are the command or event ids.
 * \todo Rename CommandMenuTreeViewDropTarget in CommandMenuTreeCtrlDropTarget
 **/
class CommandMenuTreeViewDropTarget : public wxTextDropTarget
{
    private:
        CommandMenuTreeViewDropTarget();
    public:
        CommandMenuTreeViewDropTarget(wxTreeCtrl* ctrl);
        virtual ~CommandMenuTreeViewDropTarget();

        bool OnDropText(wxCoord x, wxCoord y, const wxString& data);
        wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def);

    private:
        wxTreeCtrl* m_ctrl;
};

} // namespace penv

#endif // _PENV_UI_COMMANDMENUTREEVIEWDROPTARGET_H_
