/*
 * commandtoolbartreedata.h - Declaration of the CommandToolBarTreeData class
 *
 * Author: Steffen Ott
 * Date: 04/28/2008
 *
 */

#ifndef _PENV_UI_COMMANDTOOLBARTREEDATA_H_
#define _PENV_UI_COMMANDTOOLBARTREEDATA_H_

#include "../wxincludes.h"

namespace penv {

/**
 * \brief The CommandToolBarTreeData contains information about a
 * tree item in the CommandToolBarWindow's tree control. The class
 * simply stores a string with the command id if the tree item is
 * a toolbar item or a string of the toolbar id if the tree item
 * is a toolbar.
 **/
class CommandToolBarTreeData : public wxTreeItemData
{
    public:
        CommandToolBarTreeData();
        CommandToolBarTreeData(const wxString& id);
        virtual ~CommandToolBarTreeData();

        void SetCommandId(const wxString& id);
        void SetToolBarId(const wxString& id);
        const wxString& GetCommandId() const;
        const wxString& GetToolBarId() const;

    private:
        wxString m_id;
};

} // namespace penv

#endif // _PENV_UI_COMMANDTOOLBARTREEDATA_H_



