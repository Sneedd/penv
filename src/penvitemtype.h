/*
 * penvitemtype.h - Enumerator for the type of a penv item
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 06/30/2008
 *
 */

#ifndef _PENV_PENVITEMTYPE_H_
#define _PENV_PENVITEMTYPE_H_

namespace penv {

enum PenvItemType
{
    /**
     * \brief Specifies the type Workspace.
     **/
   penvIT_Workspace,

    /**
     * \brief Specifies the type Project.
     **/
   penvIT_Project,

    /**
     * \brief Specifies the type ProjectItem.
     **/
   penvIT_ProjectItem,

   /**
    * \brief Specifies the type ProjectItemFile.
    **/
   penvIT_ProjectItemFile,

   /**
    * \brief Specifies the type ProjectItemSubProject.
    **/
   penvIT_ProjectItemSubProject,

   /**
    * \brief Specifies the type ProjectItemLinkedItems.
    **/
   penvIT_ProjectItemLinkedItems,

   /**
    * \brief Specifies the type ProjectItemDirectory.
    **/
   penvIT_ProjectItemDirectory
};
} //namespace penv

#endif // _PENV_PENVITEMTYPE_H_

