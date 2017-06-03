/*
 * projectitemtype.h - Declaration of the ProjectItemType enumerator
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#ifndef _PENV_PROJECTITEMTYPE_H_
#define _PENV_PROJECTITEMTYPE_H_


namespace penv {

/**
 * \brief The ProjectItemType enumerator spezifies the type of
 * an projectitem.
 **/
enum ProjectItemType
{
    /**
     * \brief Spezifies that the projectitem is not definied.
     **/
    penvPI_None,
    /**
     * \brief Spezifies that the projectitem is a ProjectItemFile.
     **/
    penvPI_File,
    /**
     * \brief Spezifies that the projectitem is a ProjectItemDirectory.
     **/
    penvPI_Directory,
    /**
     * \brief Spezifies that the projectitem is a ProjectItemSubProject.
     **/
    penvPI_SubProject,
    /**
     * \brief Spezifies that the projectitem is a ProjectItemLinkedItems.
     **/
    penvPI_LinkedItems
};

} // namespace penv


#endif // _PENV_PROJECTITEMTYPE_H_
