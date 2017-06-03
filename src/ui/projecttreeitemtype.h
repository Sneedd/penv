/*
 * projecttreeitemtype.h
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 04/23/2008
 *
 */

#ifndef _PENV_UI_PROJECTTREEITEMTYPE_H_
#define _PENV_UI_PROJECTTREEITEMTYPE_H_

namespace penv {


/**
 * \brief The ProjectTreeItemType enumerator spezifies the
 * type of a tree item in the ProjectTreeCtrl class.
 **/
enum ProjectTreeItemType
{
    /**
     * \brief Specifies that the tree item is a project.
     **/
    penvTI_Project,
    /**
     * \brief Specifies that the tree item is a workspace.
     **/
    penvTI_Workspace,
    /**
     * \brief Specifies that the tree item is a project item.
     **/
    penvTI_ProjectItem
};


} // namespace penv


#endif // _PENV_UI_PROJECTTREEITEMTYPE_H_
