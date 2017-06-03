/*
 * projectreeitemdataobject.cpp - Implementation of the ProjectTreeItemDataObject class
 *
 * Author: Steffen Ott, Michael Schwierz
 * Date: 05/29/2008
 *
 */



#include "projecttreeitemdataobject.h"


namespace penv {


const wxChar* penvProjectTreeItemDataFormat = _T("penvProjectTreeItemDataFormat");



//----------------------------------------------------------------
ProjectTreeItemDataObject::ProjectTreeItemDataObject()
    : wxDataObjectSimple(wxDataFormat(penvProjectTreeItemDataFormat))
/**
 * \brief Constructor, initializes a empty ProjectTreeItemData object.
 **/
{
    m_treeitemformat.SetId(penvProjectTreeItemDataFormat);
}



//----------------------------------------------------------------
ProjectTreeItemDataObject::ProjectTreeItemDataObject(Project* project)
    : ProjectTreeItemDataBase(project)
    , wxDataObjectSimple(wxDataFormat(penvProjectTreeItemDataFormat))
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a project.
 * \param project Project attached to this object.
 **/
{
    m_treeitemformat.SetId(penvProjectTreeItemDataFormat);
}



//----------------------------------------------------------------
ProjectTreeItemDataObject::ProjectTreeItemDataObject(Workspace* workspace)
    : ProjectTreeItemDataBase(workspace)
    , wxDataObjectSimple(wxDataFormat(penvProjectTreeItemDataFormat))
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a workspace.
 * \param project Workspace attached to this object.
 **/
{
    m_treeitemformat.SetId(penvProjectTreeItemDataFormat);
}



//----------------------------------------------------------------
ProjectTreeItemDataObject::ProjectTreeItemDataObject(ProjectItem* item)
    : ProjectTreeItemDataBase(item)
    , wxDataObjectSimple(wxDataFormat(penvProjectTreeItemDataFormat))
/**
 * \brief Constructor, initializes a ProjectTreeItemData object which
 * carries a project item.
 * \param project Project item attached to this object.
 **/
{
    m_treeitemformat.SetId(penvProjectTreeItemDataFormat);
}



//----------------------------------------------------------------
ProjectTreeItemDataObject::~ProjectTreeItemDataObject()
/**
 * \brief Destructor.
 **/
{
    // Nothing to do.
}


//----------------------------------------------------------------
ProjectTreeItemDataBase* ProjectTreeItemDataObject::Clone() const
/**
 * \brief Clones this object into the returning object.
 * Only the pointer for the spezial type will be cloned,
 * all others are initialized with NULL.
 * \return Cloned object of this class.
 **/
{
    if (m_type == penvTI_Project) {
        return (new ProjectTreeItemDataObject(m_project));
    }
    else if (m_type == penvTI_ProjectItem) {
        return (new ProjectTreeItemDataObject(m_item));
    }
    else if (m_type == penvTI_Workspace) {
        return (new ProjectTreeItemDataObject(m_workspace));
    }
    return (NULL);
}




//----------------------------------------------------------------
size_t ProjectTreeItemDataObject::GetDataSize() const
/**
 * \brief Returns the data size for the serialized data
 * in this class.
 * \return Data size of this class.
 **/
{
    int size = 0;
    size += (sizeof(m_project)*3);
    size += sizeof(m_type);
    return (size);
}



//----------------------------------------------------------------
bool ProjectTreeItemDataObject::GetDataHere(void* buffer) const
/**
 * \brief Copy the data to the buffer, return true on success.
 * \param buffer The buffer to write to.
 * \return True on success; false otherwise.
 **/
{
    int ptrsize = sizeof(m_project);
    unsigned char* bytebuffer = (unsigned char*)buffer;
    memcpy((void*)(bytebuffer),             (void*)(&m_project),     ptrsize);
    memcpy((void*)(bytebuffer + ptrsize),   (void*)(&m_workspace),   ptrsize);
    memcpy((void*)(bytebuffer + ptrsize*2), (void*)(&m_item),        ptrsize);
    memcpy((void*)(bytebuffer + ptrsize*3), (void*)(&m_type),        sizeof(m_type));
    return (true);
}



//----------------------------------------------------------------
bool ProjectTreeItemDataObject::SetData(size_t len, const void *buf)
/**
 * \brief Copy the data from the buffer, return true on success.
 * \param len Length of the buffer.
 * \param buffer The buffer to read from.
 * \return True on success; false otherwise.
 **/
{
    int ptrsize = sizeof(m_project);
    if (GetDataSize() != len) return (false);
    unsigned char* bytebuffer = (unsigned char*)buf;
    memcpy((void*)(&m_project),  (void*)(bytebuffer),               ptrsize);
    memcpy((void*)(&m_workspace),(void*)(bytebuffer + ptrsize),     ptrsize);
    memcpy((void*)(&m_item),     (void*)(bytebuffer + ptrsize*2),   ptrsize);
    memcpy((void*)(&m_type),     (void*)(bytebuffer + ptrsize*3),   sizeof(m_type));
    return (true);
}






//----------------------------------------------------------------
bool ProjectTreeItemDataObject::GetDataHere(const wxDataFormat& format, void *buf ) const
/**
 * \brief The method will write the data of the format format in
 * the buffer buf and return true on success, false on failure.
 * \param format The format.
 * \param buf Buffer.
 * \return True on success; false otherwise.
 **/
{
    return (this->GetDataHere(buf));
}


//----------------------------------------------------------------
bool ProjectTreeItemDataObject::SetData(const wxDataFormat& format, size_t len, const void *buf)
/**
 * \brief Set the data in the format format of the length len
 * provided in the buffer buf.
 * \param format The format.
 * \param len Size of the buffer.
 * \param buf Buffer.
 * \return True on success; false otherwise.
 **/
{
    return (this->SetData(len, buf));
}

//----------------------------------------------------------------
size_t ProjectTreeItemDataObject::GetDataSize(const wxDataFormat& format ) const
/**
 * \brief Returns the data size of the given format.
 * \param format The format.
 * \return Always ProjectTreeItemData::GetDataSize(void)
 **/
{
    return (this->GetDataSize());
}

//----------------------------------------------------------------
void ProjectTreeItemDataObject::GetAllFormats(wxDataFormat *formats, Direction dir) const
/**
 * \brief Copy all supported formats in the given direction to the array
 * pointed to by formats. There is enough space for GetFormatCount(dir)
 * formats in it.
 * \param formats The supported formats.
 * \param dir Direction of ...
 **/
{
    formats[0] = m_treeitemformat;
}



//----------------------------------------------------------------
size_t ProjectTreeItemDataObject::GetFormatCount(Direction dir) const
/**
 * \brief Returns the number of available formats for rendering
 * or setting the data.
 * \param dir Direction of ...
 * \return Always return 1.
 **/
{
    return (1);
}




//----------------------------------------------------------------
wxDataFormat ProjectTreeItemDataObject::GetPreferredFormat(Direction dir) const
/**
 * \brief Returns the preferred format for either rendering the data
 * (if dir is Get, its default value) or for setting it. Usually this
 * will be the native format of the wxDataObject.
 * \param dir Direction of ...
 * \return
 **/
{
    return (m_treeitemformat);
}




} // namespace penv
