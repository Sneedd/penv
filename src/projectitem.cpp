/*
 * projectitem.cpp - Implementation of the ProjectItem class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */

#include "projectitem.h"
#include "projectitemlist.h"
#include "expect.h"
#include "penvhelper.h"
#include "window.h"
#include "environment.h"
#include "projectitemfile.h"
#include "projectitemdirectory.h"
#include "projectitemlinkeditems.h"
#include "projectitemsubproject.h"

namespace penv {



//----------------------------------------------------------------
ProjectItem::ProjectItem()
    :PenvItem(penvIT_ProjectItem)
/**
 * \brief Constructor (private).
 **/
{
    m_name = wxEmptyString;
    m_type = penvPI_None;
    m_windowid = wxEmptyString;
    m_windowtype = wxEmptyString;
    m_virtual = false;
    m_properties = new PropertyList();
    m_parent = NULL;
}



//----------------------------------------------------------------
ProjectItem::ProjectItem(ProjectItemList* parent)
    :PenvItem(penvIT_ProjectItem)

/**
 * \brief Constructor, initializes the ProjectItem object. Remember,
 * that this is a abstract class, so this constructor will only be called
 * in the inherited class constructor.
 * \param parent The parent container for this item.
 **/
{
    m_name = wxEmptyString;
    m_type = penvPI_None;
    m_windowid = wxEmptyString;
    m_windowtype = wxEmptyString;
    m_virtual = false;
    m_properties = new PropertyList();
    m_parent = parent;
}


//----------------------------------------------------------------
ProjectItem::~ProjectItem()
/**
 * \brief Destructor.
 **/
{
    if (m_properties != NULL) delete m_properties;
}


//----------------------------------------------------------------
const wxString& ProjectItem::GetName() const
/**
 * \brief Returns the name of the project item.
 * \return Name of the project item.
 **/
{
    return (m_name);
}


//----------------------------------------------------------------
void ProjectItem::SetName(const wxString& name)
/**
 * \brief Sets the name of the project item.
 * \param name Name of the project item.
 **/
{
    m_name = name;
}


//----------------------------------------------------------------
ProjectItemType ProjectItem::GetType() const
/**
 * \brief Returns the type of the project item.
 * \return Type of the project item.
 **/
{
    return (m_type);
}


//----------------------------------------------------------------
const wxString& ProjectItem::GetWindowId() const
/**
 * \brief Returns the window id, if this project item is connected to
 * a opend window. If the project item is not connected to a window
 * an empty string is returned.
 * \return Window id connected to this project item.
 **/
{
    return (m_windowid);
}


//----------------------------------------------------------------
void ProjectItem::SetWindowId(const wxString& windowid)
/**
 * \brief Sets the window od, if this project item should be connected
 * to a opened window. See also GetWindowId().
 * \param windowid Window id connected to this project item.
 **/
{
    // Testen ob so ein Fenster geöffnet ist
    WindowList* winlist = Environment::Get()->GetWindowList();
    if (unlikely(winlist->ExistsWindow(windowid)))
    {
        wxLogWarning(_T("[penv::ProjectItem::SetWindowType] Window id '%s' does not exists. If no window type will be registered then this could end in an unexpected behaivor."), windowid.c_str());
    }
    m_windowid = windowid;
}


//----------------------------------------------------------------
const wxString& ProjectItem::GetWindowClassId() const
/**
 * \brief Returns the window class id of this project item. The window
 * type is only needed if a window should be opened on double click
 * from the treeview.
 * \return The window type of this project item.
 **/
{
    return (m_windowtype);
}



//----------------------------------------------------------------
void ProjectItem::SetWindowClassId(const wxString& windowtype)
/**
 * \brief Sets the window class id of this project item. This
 * method generates a warning if the given window type does
 * not exist in the application. See GetWindowClassId().
 * \param windowtype The window type of this project item.
 **/
{
    // Testen ob WindowTyp existiert
    WindowList* winlist = Environment::Get()->GetWindowList();
    if (unlikely(winlist->IsRegistered(windowtype)))
    {
        wxLogWarning(_T("[penv::ProjectItem::SetWindowType] Window class id '%s' does not exists. If no window type will be registered then this could end in an unexpected behaivor."), windowtype.c_str());
    }
    m_windowtype = windowtype;
}





//----------------------------------------------------------------
bool ProjectItem::IsVirtual() const
/**
 * \brief Returns if the project item is a virtual item. Therefore
 * if this project item is virtual it does not exist physically.
 * \return True if project item is virtual; false otherwise.
 **/
{
    return (m_virtual);
}


//----------------------------------------------------------------
void ProjectItem::Virtual(bool fvirtual)
/**
 * \brief Sets if the project item is a virtual item. See
 * IsVirtual().
 * \param fvirtual True if the project item is virtual.
 **/
{
    m_virtual = fvirtual;
}


//----------------------------------------------------------------
wxString ProjectItem::GetPathString() const
/**
 * \brief Returns the path of this item as an string. If you
 * inherit from this type you should override this method
 * to make sure that this method returns the correct path.
 * \return Full path to item as wxString.
 **/
{
    return (wxEmptyString);
}



//----------------------------------------------------------------
wxFileName ProjectItem::GetPath() const
/**
 * \brief Returns the path of this item. If you
 * inherit from this type you should override this method
 * to make sure that this method returns the correct path.
 * \return Full path to item as wxFileName.
 **/
{
    return (wxFileName());
}


//----------------------------------------------------------------
ProjectItemList* ProjectItem::GetParent()
/**
 * \brief Returns the parent container for this project item.
 * \return Parent container for this project item.
 **/
{
    return (m_parent);
}



//----------------------------------------------------------------
void ProjectItem::SetParent(ProjectItemList* parent)
/**
 * \brief Sets the parent container for this project item.
 * \param parent Parent container for this project item.
 **/
{
    m_parent = parent;
}



//----------------------------------------------------------------
const wxTreeItemId& ProjectItem::GetTreeItemId() const
/**
 * \brief Returns the tree item id. This method is for the internal
 * ProjectTreeCtrl class to display this projectitem and handle
 * copy, move, ... operations.
 * \return Tree item id for this project item.
 **/
{
    return (m_treeitem);
}


//----------------------------------------------------------------
void ProjectItem::SetTreeItemId(const wxTreeItemId& treeitemid)
/**
 * \brief Sets the tree item id. This method is for the internal
 * ProjectTreeCtrl class to display this projectitem and handle
 * copy, move, ... operations.
 * \param treeitemid TreeItem connected to this ProjectItem.
 **/
{
    m_treeitem = treeitemid;
}


//----------------------------------------------------------------
PropertyList* ProjectItem::GetPropertyList()
/**
 * \brief Returns the property list for this project item.
 * \return Property list for this project item.
 **/
{
    return (m_properties);
}



//----------------------------------------------------------------
bool ProjectItem::MoveTo(Project* project)
/**
 * \brief Moves this project item into the given project.
 * There is a slightly different behaivoir on the various
 * ProjectItem types.<br>
 * <ul>
 * <li><b>ProjectItemFile:</b> Moves the file to the correct
 *   project path.</li>
 * <li><b>ProjectItemDirectory:</b> Moves the directory and all
 *   the files under this directory to the correct project path.</li>
 * <li><b>ProjectItemLinkedItems:</b> Moves all the project items
 *   in the liked items to the correct project path.</li>
 * <li><b>ProjectItemSubProject:</b> Moves the whole project and
 *   all it's project items to the correct project path.</li>
 * </ul>
 * \param project The project where to move this item.
 * \return True on success, false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (false);
}


//----------------------------------------------------------------
bool ProjectItem::MoveTo(ProjectItem* projectitem)
/**
 * \brief Moves this project item into the given project item.
 * The item can only be moved to a directory, linkeditems or
 * subproject.
 * There is a slightly different behaivoir on the various
 * ProjectItem types.<br>
 * <ul>
 * <li><b>ProjectItemDirectory:</b> Moves the directory and all
 *   the files under this directory to the correct project item path.</li>
 * <li><b>ProjectItemLinkedItems:</b> Moves all the project items
 *   in the liked items to the correct project item path.</li>
 * <li><b>ProjectItemSubProject:</b> Moves the whole project and
 *   all it's project items to the correct project item path.</li>
 * </ul>
 * \param projectitem The project item where to move this item.
 * \return True on success, false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (false);
}


//----------------------------------------------------------------
bool ProjectItem::CopyTo(Project* project)
/**
 * \brief Copies this project item into the given project.
 * There is a slightly different behaivoir on the various
 * ProjectItem types.<br>
 * <ul>
 * <li><b>ProjectItemFile:</b> Copies the file to the correct
 *   project path.</li>
 * <li><b>ProjectItemDirectory:</b> Copies the directory and all
 *   the files under this directory to the correct project path.</li>
 * <li><b>ProjectItemLinkedItems:</b> Copies all the project items
 *   in the liked items to the correct project path.</li>
 * <li><b>ProjectItemSubProject:</b> Copies the whole project and
 *   all it's project items to the correct project path.</li>
 * </ul>
 * \param project The project where to move this item.
 * \return True on success, false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (false);
}


//----------------------------------------------------------------
bool ProjectItem::CopyTo(ProjectItem* projectitem)
/**
 * \brief Copies this project item into the given project item.
 * The item can only be moved to a directory, linkeditems or
 * subproject.
 * There is a slightly different behaivoir on the various
 * ProjectItem types.<br>
 * <ul>
 * <li><b>ProjectItemDirectory:</b> Copies the directory and all
 *   the files under this directory to the correct project item path.</li>
 * <li><b>ProjectItemLinkedItems:</b> Copies all the project items
 *   in the liked items to the correct project item path.</li>
 * <li><b>ProjectItemSubProject:</b> Copies the whole project and
 *   all it's project items to the correct project item path.</li>
 * </ul>
 * \param projectitem The project item where to move this item.
 * \return True on success, false otherwise.
 **/
{
    NOT_IMPLEMENTED_YET();
    return (false);
}



} // namespace penv
