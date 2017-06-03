/*
 * projectitem.h - Declaration of the ProjectItem class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 10/02/2007
 *
 */


#ifndef _PENV_PROJECTITEM_H_
#define _PENV_PROJECTITEM_H_

#include "wxincludes.h"
#include "propertylist.h"
#include "projectitemtype.h"
#include "penvitem.h"

namespace penv {

// Forward Declarations
class Project;
class ProjectItemDirectory;
enum ProjectItemType;

/**
 * \brief The ProjectItem class represents a project item in the framework.
 * The class is abstract and must be inherited to use it properly. The
 * following classes are predefined for the framework:<br>
 * <ul>
 *  <li><b>ProjectItemFile:</b> The ProjectItemFile is for handling simple
 *  files in a project.</li>
 *  <li><b>ProjectItemDirectory:</b> The ProjectItemDirectory is for handling simple
 *  directories in a project.</li>
 *  <li><b>ProjectItemLinkedList:</b> The ProjectItemLinkedList is for handling
 *  more than one file, which belong together. In the tree ctrl the linked item appears
 *  as one file, but has a 'plus' to expand it and reveal the other items which belongs to
 *  the main file.</li>
 *  <li><b>ProjectItemSubProject:</b> The ProjectItemSubProject is for handling
 *  sub projects in a project.</li>
 * </ul><br>
 * A project item can be virtual, this means that the project item does not exist
 * physically. See Virtual() and IsVirtual().<br>
 * A project item can be connected to a window type and a window id. The window type
 * is used to open the appropriate window document or window pane to display the
 * project item. The window id is used when the window is already opened.<br>
 * Every project item has a property list connected to it where properties for the
 * item can be saved. This information will always be saved into the project xml file.
 * \todo MoveTo() and CopyTo() Methods should be abstract and implemented in the
 * inherited classes.
 **/
class ProjectItem : public PenvItem
{
    public:
        friend class ProjectItemList;
        ProjectItem();
        ProjectItem(ProjectItemList* parent);
        virtual ~ProjectItem();

        virtual const wxString& GetName() const;
        virtual void SetName(const wxString& name);

        ProjectItemType GetType() const;

        const wxString& GetWindowId() const;
        void SetWindowId(const wxString& windowid);

        const wxString& GetWindowClassId() const;
        void SetWindowClassId(const wxString& windowtype);

        bool IsVirtual() const;
        void Virtual(bool fvirtual=true);

        virtual wxString GetPathString() const;
        virtual wxFileName GetPath() const;

        ProjectItemList* GetParent();
        void SetParent(ProjectItemList* parent);

        const wxTreeItemId& GetTreeItemId() const;
        void SetTreeItemId(const wxTreeItemId& treeitemid);

        PropertyList* GetPropertyList();

        /**
         * \brief Returns the item list of this project item. This method
         * is abstract and only implemented in the inherited class.
         * See description of this method in the inherited class.
         * \return Item list of this project item or NULL.
         **/
        virtual ProjectItemList* GetItemList() = 0;

        virtual bool MoveTo(Project* project);
        virtual bool MoveTo(ProjectItem* projectitem);
        virtual bool CopyTo(Project* project);
        virtual bool CopyTo(ProjectItem* projectitem);

        /**
         * \brief Saves this project item.  This method
         * is abstract and only implemented in the inherited class.
         * See description of this method in the inherited class.
         * \return True on success; false otherwise.
         **/
        virtual bool Save() = 0;
        /**
         * \brief Loads this project item.  This method
         * is abstract and only implemented in the inherited class.
         * See description of this method in the inherited class.
         * \return True on success; false otherwise.
         **/
        virtual bool Load() = 0;
        /**
         * \brief Clones this project item.  This method
         * is abstract and only implemented in the inherited class.
         * See description of this method in the inherited class.
         * \return Cloned project item.
         **/
        virtual ProjectItem* Clone() = 0;
        /**
         * \brief Reads a 'projectitem' xml element. This method
         * is abstract and only implemented in the inherited class.
         * See description of this method in the inherited class.
         * \return Cloned project item.
         **/
        virtual bool ReadNode(wxXmlNode* node) = 0;
        /**
         * \brief Writes a 'projectitem' xml element. This method
         * is abstract and only implemented in the inherited class.
         * See description of this method in the inherited class.
         * \return Cloned project item.
         **/
        virtual wxXmlNode* WriteNode() = 0;

    protected:
        wxString m_name;
        ProjectItemType m_type;
        wxString m_windowid;
        wxString m_windowtype;
        bool m_virtual;
        PropertyList* m_properties;
        ProjectItemList* m_parent;
        wxTreeItemId m_treeitem;
};

} // namespace penv


#endif // _PENV_PROJECTITEM_H_
