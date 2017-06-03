/*
 * toolbar.cpp - Implementation of the ToolBar class
 *
 * Author: Steffen ott
 * Date: 11/05/2007
 *
 */



#include "toolbar.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"

namespace penv {

//----------------------------------------------------------------
ToolBar::ToolBar()
/**
 * \brief Constructor, initialize the ToolBar object.
 **/
{
    m_array = new Array<ToolBarItem>();
    m_toolbar = NULL;
    m_counttoolbarids = 0;
    m_toolbarids = NULL;
    m_visible = true;
}


//----------------------------------------------------------------
ToolBar::~ToolBar()
/**
 * \brief Destructor.
 **/
{
    // TEST: Müssen events gelöscht werden?
    // TEST: Muss die wxToolBar gelöscht werden oder darf nur in Update stattfinden?
    if (m_array != NULL)
    {
        m_array->Clear(true);
        delete m_array;
    }
}



//----------------------------------------------------------------
void ToolBar::SetName(const wxString& name)
/**
 * \brief Sets the name of the toolbar. The name of the toolbar
 * is only visible, when the toolbar is floating and can be used
 * for example in the menu to toggle the toolbars visibility.
 * \param name Name of the toolbar.
 **/
{
    m_name = name;
}


//----------------------------------------------------------------
const wxString& ToolBar::GetName() const
/**
 * \brief Returns the name of the toolbar.
 * See also SetName().
 * \return Name of the toolbar.
 **/
{
    return (m_name);
}



//----------------------------------------------------------------
void ToolBar::SetId(const wxString& id)
/**
 * \brief Sets the ID of the toolbar. The toolbar ID must
 * be unique. This method also changes the ID in the ToolBarList
 * when the ToolBarList contains this ToolBar.
 * \param id Unique ID for this toolbar.
 **/
{
    ToolBarList* list = Environment::Get()->GetCommandList()->GetToolBarList();
    wxString correctedid;
    if (!id.StartsWith(_T("TOOLBAR_"))) {
        correctedid.Append(_T("TOOLBAR_"));
        correctedid.Append(id);
    } else {
        correctedid.Append(id);
    }

    if (correctedid == m_id) return;
    if (list->GetToolBar(correctedid) != NULL) {
        // If a toolbar with the new id already exists
        wxLogError(_T("[penv::ToolBar::SetId] A toolbar with the id '%s' already exists in ToolBarList."), id.c_str());
        return;
    }
    else
    {
        // If a toolbar with the new id not in list
        if (list->GetToolBar(m_id) != NULL)
        {
            // if a toolbar with old id does exists in toolbarlist, change it
            // TEST this
            list->Remove(m_id);
            m_id = correctedid;
            list->Add(this);
        }
    }
    m_id = correctedid;
    m_paneinfo.Name(m_id);
}


//----------------------------------------------------------------
const wxString& ToolBar::GetId() const
/**
 * \brief Return the ID of this toolbar. The toolbar id is
 * unique.
 * \return Toolbar id.
 **/
{
    return (m_id);
}


//----------------------------------------------------------------
void ToolBar::Visible(bool visible)
/**
 * \brief Changes visibility of the toolbar.
 * \param visible True to show toolbar; false to hide.
 **/
{
    if (m_toolbar == NULL) {
        wxLogError(_T("[penv::ToolBar::Visible] Toolbar is not initalized, call Update() before changeing the visibility."));
        return;
    }
    if (m_visible == visible) return;
    wxAuiManager* manager = Environment::Get()->GetFrame()->GetManager();
    if (visible)
    {
        // Show toolbar
        manager->AddPane(m_toolbar, m_paneinfo);
        m_toolbar->Show();
        manager->Update();
    }
    else
    {
        // Hide toolbar
        m_paneinfo = manager->GetPane(m_toolbar);
        manager->DetachPane(m_toolbar);
        m_toolbar->Hide();
        manager->Update();
    }
    m_visible = visible;
}


//----------------------------------------------------------------
bool ToolBar::IsVisible() const
/**
 * \brief Returns if the toolbar is visible.
 * \return True if toolbar is visible; false otherwise.
 **/
{
    return (m_visible);
}



//----------------------------------------------------------------
ToolBarItem* ToolBar::GetItem(size_t index)
/**
 * \brief Returns a toolbar item with the given index.
 * If the index is out of range this method will return
 * null.
 * \param index Index of the toolbar item.
 * \return Toolbar item if found; NULL on error.
 **/
{
    if (unlikely(index < 0 || index >= m_array->Count())) {
        wxLogError(_T("[penv::ToolBar::GetItem] Given index \"%i\" is out of range."), index);
        return (NULL);
    }
    return ((*m_array)[index]);
}


//----------------------------------------------------------------
bool ToolBar::Add(ToolBarItem* item)
/**
 * \brief Adds an toolbar item to the end of the container.
 * To update the changed toolbar simply call Update().
 * \param item Toolbar item which should be added.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(item == NULL)) {
        wxLogError(_T("[penv::ToolBar::Add] Could not add toolbar item, because parameter argument is NULL."));
        return (false);
    }
    m_array->Add(item);
    return (true);
}


//----------------------------------------------------------------
bool ToolBar::Insert(size_t index, ToolBarItem* item)
/**
 * \brief Inserst a toolbar item at the spezified index.
 * To update the changed toolbar simply call Update().
 * \param index Index at which to insert toolbar item.
 * \param item Toolbar item to insert.
 * \return True on success; false otherwise.
 **/
{
    if (index < 0 || index > m_array->Count())
    {
        wxLogError(_T("[penv::ToolBar::Insert] Index is out of range for toolbar '%s'."), m_name.c_str());
        return (false);
    }
    return (m_array->Insert(index, item));
}


//----------------------------------------------------------------
bool ToolBar::Remove(ToolBarItem* item)
/**
 * \brief Removes a toolbar item from the toolbar.
 * To update the changed toolbar simply call Update().
 * \param item The toolbar item which should be removed.
 * \return True on success; false otherwise.
 **/
{
    bool result = m_array->Remove(item);
    if (!result)
    {
        wxLogError(_T("[penv::ToolBar::Remove] Cannot remove the toolbar item, because it does not exist in toolbar '%s'."), m_name.c_str());
    }
    return (result);
}


//----------------------------------------------------------------
bool ToolBar::Remove(size_t index)
/**
 * \brief Removes a toolbar item from the toolbar at the spezified
 * index. To update the changed toolbar simply call Update().
 * \param index The index of the toolbar item to remove.
 * \return True on success; false otherwise.
 **/
{
    if (index < 0 || index >= m_array->Count())
    {
        wxLogError(_T("[penv::ToolBar::Remove] Index is out of range for toolbar '%s'."), m_name.c_str());
        return (false);
    }
    return (m_array->Remove(index));
}


//----------------------------------------------------------------
void ToolBar::Clear()
/**
 * \brief Removes all toolbar items from the toolbar.
 * To update the changed toolbar simply call Update().
 **/
{
    m_array->Clear();
}


//----------------------------------------------------------------
size_t ToolBar::Count()
/**
 * \brief Returns the number of the toolbar items in the toolbar.
 * \return Number of toobar items.
 **/
{
    return (m_array->Count());
}


//----------------------------------------------------------------
bool ToolBar::Update()
/**
 * \brief Updates the toolbar and all its item. In some cases it is
 * necessary to update the wxAuiManager after calling this method.
 * \return True on success; false otherwise.
 **/
{
    Environment* env = Environment::Get();
    if (unlikely(m_toolbar == NULL)) {
        wxToolBar* m_toolbar = new wxToolBar(env->GetFrame(), wxID_ANY,
            wxDefaultPosition, wxDefaultSize, penvCOMMON_TOOLBARSTYLE);
        m_toolbar->SetToolBitmapSize(penvCOMMON_TOOLBARICONSIZE);
    }
    else
    {
        // Because ClearTools does not remove events
        //  from event handler, remove them manually
        ClearToolEvents();
        // Clear all tool items
        m_toolbar->ClearTools();
    }
    if (m_toolbarids != NULL) delete [] m_toolbarids;
    m_toolbarids = new int[m_array->Count()];
    m_counttoolbarids = m_array->Count();
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        ToolBarItem* item = m_array->ItemPtr(i);
        // Überprüfen ob ID schon aufgelöst wurde
        if (item->GetCommand() == NULL && !item->GetId().IsEmpty()) {
            item->SetCommand(env->GetCommandList()->GetCommand(item->GetId()));
            if (item->GetCommand() == NULL) {
                wxLogWarning(_T("[penv::ToolBar::Update] Could not resolve command id '%s' for item. Removing item!"), item->GetId().c_str());
                m_array->Del(i, true);
                continue;
            }
        }
        if (item->GetCommand() != NULL) {
            if (item->GetCommand()->IsEvent()) {
                wxLogWarning(_T("[penv::ToolBar::Update] Could not add command id '%s' to toolbar, because this command is a event. Removing item!"), item->GetId().c_str());
                m_array->Del(i, true);
                continue;
            }
            if (!item->GetIcon().IsOk()) {
                wxLogWarning(_T("[penv::ToolBar::Update] Could not add command id '%s' to toolbar, because this command does not have a bitmap."), item->GetId().c_str());
                continue;
            }
        }
        wxToolBarToolBase* tool = NULL;
        if (unlikely(item->IsSeparator())) {
            tool = m_toolbar->AddSeparator();
            item->SetTool(tool);
        } else {
            tool = m_toolbar->AddTool(wxNewId(),
                item->GetCommand()->GetName(),
                item->GetCommand()->GetIcon(),
                wxNullBitmap,
                wxITEM_NORMAL,
                item->GetCommand()->GetName(),
                item->GetCommand()->GetHelp());
            item->SetTool(tool);
            tool->Enable(item->GetCommand()->IsEnabled());
            if (unlikely(!item->TryConnect())) {
                wxLogWarning(_T("[penv::ToolBar::Update] Could not connect tool item, with wxWidgets Event."));
                m_toolbarids[i] = -1;
            } else {
                m_toolbarids[i] = tool->GetId();
            }
        }
    }
    for (int i=m_array->Count()-1; i>=0; --i)
    {
        if (m_array->ItemPtr(i) == NULL) m_array->Remove((size_t)i, false);
    }
    m_toolbar->Realize();
    // Toolbar always set caption new
    m_paneinfo.Caption(m_name).Name(m_id).ToolbarPane();
    // Toolbar is always on top!
    if (m_paneinfo.dock_direction != wxAUI_DOCK_TOP) {
        wxLogWarning(_T("[penv::ToolBar::Update] Toolbar '%s' is not on top of the frame, changing it to display toolbar on top."), m_name.c_str());
        m_paneinfo.Top();
    }
    // Always set the correct size
    m_paneinfo.BestSize(m_toolbar->GetBestSize());
    m_paneinfo.MinSize(-1,-1).MaxSize(-1,-1).FloatingSize(-1,-1);
    // Add toolbar to aui manager, if toolbar visible and there are min. 1 item
    if (m_visible && m_array->Count() > 0) {
        wxAuiManager* manager = Environment::Get()->GetFrame()->GetManager();
        if (unlikely(!manager->AddPane(m_toolbar, m_paneinfo))) {
            wxLogError(_T("[penv::ToolBar::Update] Cannot add toolbar '%s' to aui manager."), m_name.c_str());
            return (false);
        }
        m_toolbar->Show();
    } else {
        m_toolbar->Hide();
    }
    return (true);
}



//----------------------------------------------------------------
bool ToolBar::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'toolbar' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'toolbar' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Fehler überprüfen
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::ToolBar::ReadNode] Cannot load xml node, because parameter argument is NULL."));
        return (false);
    }
    if (unlikely(node->GetType() != wxXML_ELEMENT_NODE)) {
        wxLogError(_T("[penv::ToolBar::ReadNode] Cannot load, xml node must be an element node."));
        return (false);
    }
    if (unlikely(node->GetName() != _T("toolbar"))) {
        wxLogError(_T("[penv::ToolBar::ReadNode] Cannot load, xml element node must have the name \"toolbar\". This element node has the name \"%s\"."), node->GetName().c_str());
        return (false);
    }

    // Einlesen der Attribute der Toolbar
    if (unlikely(!node->HasProp(_T("id")))) {
        wxLogError(_T("[penv::ToolBar::ReadNode] Cannot load, xml element node must have the attribute \"id\". This element node has the name \"%s\"."), node->GetName().c_str());
        return (false);
    }
    m_id = node->GetPropVal(_T("id"), wxEmptyString);
    CorrectToolbarId();
    m_name = node->GetPropVal(_T("name"), wxEmptyString);
    m_visible = PenvHelper::ParseBoolean(node->GetPropVal(_T("visible"), _T("true")));

    // Einlesen der Position und der ToolBar Items
    Environment* env = Environment::Get();
    wxXmlNode* child = node->GetChildren();
    while (child != NULL)
    {
        // Nicht Elemente überspringen
        if (unlikely(child->GetType() != wxXML_ELEMENT_NODE)) {
            child = child->GetNext();
            continue;
        }
        if (child->GetName() == _T("position")) {
            wxString strg = child->GetNodeContent();
            env->GetFrame()->GetManager()->LoadPaneInfo(strg, m_paneinfo);
        }
        if (child->GetName() == _T("toolitem")) {
            ToolBarItem* item = new ToolBarItem();
            if (!item->ReadNode(child)) {
                wxLogWarning(_T("[penv::ToolBar::ReadNode] Toolbar item in toolbar \"%s\" could not be readed. Skipping..."), m_name.c_str());
                delete item;
            } else {
                Add(item);
            }
        }
        child = child->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* ToolBar::WriteNode()
/**
 * \brief Writes a 'toolbar' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'toolbar' xml element node; or NULL on error.
 **/
{
    // ToolBar Node erstellen
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("toolbar"));
    node->AddProperty(_T("id"), m_id);
    node->AddProperty(_T("name"), m_name);
    node->AddProperty(_T("visible"), PenvHelper::CreateBoolean(m_visible));
    // Positions Node erstellen (node ist parent!)
    wxAuiManager* mgr = Environment::Get()->GetFrame()->GetManager();
    wxString posinfo = mgr->SavePaneInfo(m_paneinfo);
    wxXmlNode* posnode = PenvHelper::CreateXmlNode(_T("position"), posinfo);
    PenvHelper::AddXmlChildNode(node, posnode);
    // ToolBar Itemes erstellen und hinzufügen
    for (size_t i=0; i<m_array->Count(); ++i)
    {
        ToolBarItem* item = (*m_array)[i];
        wxXmlNode* toolitemnode = item->WriteNode();
        PenvHelper::AddXmlChildNode(node, toolitemnode);
    }
    return (node);
}


//----------------------------------------------------------------
wxToolBar* ToolBar::GetToolBar()
/**
 * \brief Returns the wxToolBar for this toolbar. This method
 * is only used internally.
 * \return The wxToolBar
 **/
{
    return (m_toolbar);
}


//----------------------------------------------------------------
void ToolBar::SetToolBar(wxToolBar* toolbar)
/**
 * \brief Sets the wxToolBar for this toolbar. This method
 * is only used internally.
 * \param toolbar The wxToolBar.
 **/
{
    m_toolbar = toolbar;
}



//----------------------------------------------------------------
wxAuiPaneInfo& ToolBar::GetPaneInfo()
/**
 * \brief Returns the wxAuiPaneInfo object for the toolbar.
 * The wxAuiPaneInfo spezifies the position of the toolbar.
 * \return The wxAuiPaneInfo object.
 **/
{
    return (m_paneinfo);
}


//----------------------------------------------------------------
void ToolBar::SetPaneInfo(const wxAuiPaneInfo& paneinfo)
/**
 * \brief Sets the wxAuiPaneInfo object for this toolbar.
 * \param paneinfo The paneinfo object for this toolbar.
 **/
{
    m_paneinfo = paneinfo;
}


//----------------------------------------------------------------
void ToolBar::CorrectToolbarId()
/**
 * \brief Corrects the toolbar id, because the toolbar id must always
 * look like 'TOOLBAR_xxx'. Where 'xxx' is the unique id.
 **/
{
    if (!m_id.StartsWith(_T("TOOLBAR_"))) {
        wxString id;
        id.Append(_T("TOOLBAR_"));
        id.Append(m_id);
        m_id = id;
    }
}


//----------------------------------------------------------------
void ToolBar::ClearToolEvents()
/**
 * \brief Removes all events registered to the ApplicationFrame
 * event handler. And removes the internally stored toolbar item
 * ids afterwards.
 **/
{
    if (m_toolbarids == NULL) return;
    ApplicationFrame* frame = Environment::Get()->GetFrame();
    // Use the internal Id array, because the penv ToolBar structure
    //   could have changed.
    for (size_t i=0; i<m_counttoolbarids; ++i)
    {
        if (m_toolbarids[i] != -1)
            frame->Disconnect(m_toolbarids[i]);
    }
    delete [] m_toolbarids;
    m_toolbarids = NULL;
    m_counttoolbarids = 0;
}


} // namespace penv


