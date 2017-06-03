/*
 * projecttreeactiontype.h - Declaration of the ProjectTreeActionType enumerator
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/27/2008
 *
 */

#ifndef _PENV_UI_PROJECTTREEACTIONTYPE_H_
#define _PENV_UI_PROJECTTREEACTIONTYPE_H_

namespace penv {


/**
 * \brief The ProjectTreeActionType enumerator spezifies,
 * what action is executed on a ProjectTreeCtrl.
 **/
enum ProjectTreeActionType
{
    /**
     * \brief Action is a add or insert action.
     **/
    penvPA_Add,

    /**
     * \brief Action is a remove action.
     **/
    penvPA_Remove,

    /**
     * \brief Action is a copy action.
     **/
    penvPA_Copy,

    /**
     * \brief Action is a move action.
     **/
    penvPA_Move,

    /**
     * \brief Action is a rename action.
     **/
    penvPA_Rename
};


} // namespace penv


#endif //_PENV_UI_PROJECTTREEACTIONTYPE_H_

