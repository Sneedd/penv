/*
 * commandtoolbartreeviewdroptarget.h - Declaration of the CommandToolBarTreeViewDropTarget class
 *
 * Author: Steffen Ott
 * Date: 04/28/2008
 *
 */


#ifndef _PENV_UI_COMMANDTOOLBARTREEVIEWDROPTARGET_H_
#define _PENV_UI_COMMANDTOOLBARTREEVIEWDROPTARGET_H_


#include "../wxincludes.h"


namespace penv {


/**
 * \brief The CommandToolBarTreeViewDropTarget is the target for
 * a drag and drop operation from the listctrl in the CommandToolBarWindow
 * panel. Because this drop target is inherited from wxTextDropTarget,
 * the target only except strings, which are the command or event ids.
 * \todo Rename CommandToolBarTreeViewDropTarget in CommandToolBarTreeCtrlDropTarget.
 **/
class CommandToolBarTreeViewDropTarget : public wxTextDropTarget
{
    private:
        CommandToolBarTreeViewDropTarget();
    public:
        CommandToolBarTreeViewDropTarget(wxTreeCtrl* ctrl);
        virtual ~CommandToolBarTreeViewDropTarget();

        bool OnDropText(wxCoord x, wxCoord y, const wxString& data);
        wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def);

    private:
        wxTreeCtrl* m_ctrl;
};


} // namespace penv

#endif // _PENV_UI_COMMANDTOOLBARTREEVIEWDROPTARGET_H_



