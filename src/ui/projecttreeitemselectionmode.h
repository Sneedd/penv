/*
 * projecttreeitemselectionmode.h - Declaration of the Projecttreeitemselectionmode class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/29/2008
 *
 */


#ifndef _PENV_UI_PROJECTTREEITEMSELECTIONMODE_H_
#define _PENV_UI_PROJECTTREEITEMSELECTIONMODE_H_

namespace penv{
/**
 * \brief The ProjectTreeItemSelectionMode enumerator is for setting
 * the selection mode for a tree item.
 * This enumerator is used by ProjectTreeItemId.
 **/
enum ProjectTreeItemSelectionMode
{
    /**
     * \brief Toggles the selection for the actual tree item.
     **/
    penvSM_Toggle,

    /**
     * \brief Select only the actual tree item and unselects all
     * previously selected items.
     **/
    penvSM_SelectSingle,

    /**
     * \brief Unselects the actual tree item.
     **/
    penvSM_Unselect,

    /**
     * \brief Select the actual tree item.
     **/
    penvSM_SelectMutiple
};
}//namespace penv

#endif // _PENV_UI_PROJECTTREEITEMSELECTIONMODE_H_
