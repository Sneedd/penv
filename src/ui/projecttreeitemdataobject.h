/*
 * projecttreeitemdataobject.h - Declaration of the ProjectTreeItemDataObject class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/29/2008
 *
 */


#ifndef _PENV_UI_PROJECTTREEITEMDATAOBJECT_H_
#define _PENV_UI_PROJECTTREEITEMDATAOBJECT_H_

#include "../wxincludes.h"
#include "../project.h"
#include "../workspace.h"
#include "../projectitem.h"
#include "projecttreeitemtype.h"
#include "projecttreeitemdatabase.h"


namespace penv {



/**
 * \brief Defines the Data Format for a Tree Item.
 **/
extern const wxChar* penvProjectTreeItemDataFormat;



/**
 * \brief The ProjectTreeItemDataObject class holds information for a treeitem
 * and implements this information for a data object. The data object will
 * be used to do drag and drop operations on the tree ctrl. The information
 * for the tree item is the appropriate item connected to the tree item.
 **/
class ProjectTreeItemDataObject : public ProjectTreeItemDataBase, public wxDataObjectSimple
{
    public:
        ProjectTreeItemDataObject();
        ProjectTreeItemDataObject(Project* project);
        ProjectTreeItemDataObject(Workspace* workspace);
        ProjectTreeItemDataObject(ProjectItem* item);
        virtual ~ProjectTreeItemDataObject();

        virtual ProjectTreeItemDataBase* Clone() const;

        virtual size_t GetDataSize() const;
        virtual bool GetDataHere(void *buf) const;
        virtual bool SetData(size_t len, const void *buf);

        virtual bool GetDataHere(const wxDataFormat& format, void *buf ) const;
        virtual bool SetData(const wxDataFormat& format, size_t len, const void *buf);
        virtual size_t GetDataSize(const wxDataFormat& format ) const;
        virtual void GetAllFormats(wxDataFormat *formats, Direction dir = Get) const;
        virtual size_t GetFormatCount(Direction dir = Get) const;
        virtual wxDataFormat GetPreferredFormat(Direction dir = Get) const;

    protected:
        wxDataFormat m_treeitemformat;
};

} // namespace penv

#endif // _PENV_UI_PROJECTTREEITEMDATAOBJECT_H_
