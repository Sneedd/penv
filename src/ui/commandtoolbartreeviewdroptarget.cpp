/*
 * commandtoolbartreeviewdroptarget.cpp - Implementation of the CommandToolBarTreeViewDropTarget class
 *
 * Author: Steffen Ott
 * Date: 04/28/2008
 *
 */



#include "commandtoolbartreeviewdroptarget.h"
#include "../command.h"
#include "../penvhelper.h"
#include "../environment.h"
#include "commandtoolbartreedata.h"


namespace penv {

//----------------------------------------------------------------
CommandToolBarTreeViewDropTarget::CommandToolBarTreeViewDropTarget()
/**
 * \brief Constructor (private).
 **/
{
}


//----------------------------------------------------------------
CommandToolBarTreeViewDropTarget::CommandToolBarTreeViewDropTarget(wxTreeCtrl* ctrl)
/**
 * \brief Constructor, initializes the CommandToolBarTreeViewDropTarget object.
 * As a drop target this class needs the treectrl where strings can be droped.
 * \param ctrl Tree control where strings can be droped.
 **/
{
    m_ctrl = ctrl;
}


//----------------------------------------------------------------
CommandToolBarTreeViewDropTarget::~CommandToolBarTreeViewDropTarget()
/**
 * \brief Destructor.
 **/
{
}


//----------------------------------------------------------------
bool CommandToolBarTreeViewDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
/**
 * \brief Occurs when the text is droped onto the target.
 * \param x X position of the mouse.
 * \param y Y position of the mouse.
 * \param data Data which will be dropped.
 * \return True on drop is ok; false otherwise.
 **/
{
    // Prüfen ob über einem Item gedroppt wird
    int flags = wxTREE_HITTEST_ONITEMLABEL;
    wxTreeItemId id = m_ctrl->HitTest(wxPoint(x, y), flags);
    if (id.IsOk())
    {
        // Testen ob Element über Root gedroppt wird
        if (m_ctrl->GetRootItem() == id)
        {
            return (false);
        }
        // Holen des Commands und testen ob korrekt
        Command* cmd = Environment::Get()->GetCommandList()->GetCommand(data);
        if (cmd == NULL) return (false);
        // Image setzten
        // TODO SO: Image
        int imgindex = -1;
        // Testen ob auf einer ToolBar
        // (Eine ToolBar ist immer ein element nach root)
        if (m_ctrl->GetRootItem() == m_ctrl->GetItemParent(id))
        {
            m_ctrl->InsertItem(id, 0, cmd->GetName(), imgindex, imgindex,
                new CommandToolBarTreeData(cmd->GetId()));
            m_ctrl->Update();
            return (true);
        }
        m_ctrl->InsertItem(m_ctrl->GetItemParent(id), id, cmd->GetName(),
            imgindex, imgindex, new CommandToolBarTreeData(cmd->GetId()));
        m_ctrl->Update();
        return (true);
    }
    return (false);
}



//----------------------------------------------------------------
wxDragResult CommandToolBarTreeViewDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
/**
 * \brief Checks if a drag and drop operation can is allowed on the
 * drop target.
 * \param x X position of the mouse.
 * \param y Y position of the mouse.
 * \param def Predefiend drag result.
 * \return Returning drag result action.
 **/
{
    // Prüfen ob über einem Item gedroppt wird
    int flags = wxTREE_HITTEST_ONITEMLABEL;
    wxTreeItemId id = m_ctrl->HitTest(wxPoint(x, y), flags);
    if (id.IsOk() && m_ctrl->GetRootItem() != id)
    {
        return (wxDragMove);
    }
    return (wxDragNone);
}


} // namespace penv







