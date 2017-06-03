/*
 * projectreeitemdata.cpp - Implementation of the ProjectTreeItemData class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/29/2008
 *
 */


#include "projecttreeitemdata.h"

namespace penv {


//----------------------------------------------------------------
ProjectTreeItemData::ProjectTreeItemData()
    : ProjectTreeItemDataBase()
    , wxTreeItemData()
/**
 * \brief Constructor, initializes a empty ProjectTreeItemData object.
 **/
{
}



//----------------------------------------------------------------
ProjectTreeItemData::ProjectTreeItemData(Project* project)
    : ProjectTreeItemDataBase(project)
    , wxTreeItemData()
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a project.
 * \param project Project attached to this object.
 **/
{
}



//----------------------------------------------------------------
ProjectTreeItemData::ProjectTreeItemData(Workspace* workspace)
    : ProjectTreeItemDataBase(workspace)
    , wxTreeItemData()
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a workspace.
 * \param project Workspace attached to this object.
 **/
{
}



//----------------------------------------------------------------
ProjectTreeItemData::ProjectTreeItemData(ProjectItem* item)
    : ProjectTreeItemDataBase(item)
    , wxTreeItemData()
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a project item.
 * \param project Project item attached to this object.
 **/
{
}



//----------------------------------------------------------------
ProjectTreeItemData::~ProjectTreeItemData()
/**
 * \brief Destructor.
 **/
{
    // Nothing to do.
}



} // namespace penv
