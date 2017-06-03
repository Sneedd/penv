/*
 * toolbarlist.cpp - Implementation of the ToolBarList class
 *
 * Author: Steffen Ott
 * Date: 10/01/2007
 *
 */

#include "toolbarlist.h"
#include "expect.h"
#include "penvhelper.h"
#include "environment.h"


namespace penv {


//----------------------------------------------------------------
ToolBarList::ToolBarList()
/**
 * \brief Constructor. Unused.
 **/
{
    wxASSERT_MSG(false, _T("Not allowed to be called."));
}


//----------------------------------------------------------------
ToolBarList::ToolBarList(ApplicationFrame* frame, wxAuiManager* manager)
/**
 * \brief Constructor, initializes the toolbarlist.
 * \param frame The ApplicationFrame.
 * \param manager The wxAuiManager for the Frame.
 **/
{
    m_frame = frame;
    m_manager = manager;
    m_hashmap = new ToolBarHashMap();
}



//----------------------------------------------------------------
ToolBarList::~ToolBarList()
/**
 * \brief Destructor.
 **/
{
    Clear();
    if (m_hashmap != NULL) delete m_hashmap;
}



//----------------------------------------------------------------
wxArrayString* ToolBarList::GetToolBars()
/**
 * \brief Returns all toolbar IDs in a string array.
 * You must destroy this array after use.
 * \return All toolbar ids in a string array.
 **/
{
    wxArrayString* array = new wxArrayString();
    ToolBarHashMap::iterator itr;
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        array->Add(itr->first);
    }
    return (array);
}


//----------------------------------------------------------------
ToolBar* ToolBarList::GetToolBar(const wxString& id)
/**
 * \brief Returns a toolbar over their ID. If the toolbar
 * does not exists this methof will return NULL.
 * \param id The ID of the wanted toolbar.
 * \return Returns the toolbar if it exists; NULL otherwise.
 **/
{
    ToolBarHashMap::iterator itr;
    if (unlikely((itr = m_hashmap->find(id)) == m_hashmap->end())) {
        return (NULL);
    }
    return (itr->second);
}


//----------------------------------------------------------------
bool ToolBarList::Add(ToolBar* toolbar)
/**
 * \brief Adds a toolbar to the container. Because the toolbar must
 * have a unique ID, this ID will be generated if the ID in the toolbar
 * is empty. To generate a new toolbar ID the NewID() method will be used.
 * To commit changes and show newly added toolbars call Update().
 * \param toolbar The toolbar which will be added.
 * \return True on success; false otherwise.
 **/
{
    // Fehler abfangen
    if (unlikely(toolbar == NULL)) {
        wxLogError(_T("[penv::ToolBarList::Add] Cannot add toolbar, because parameter argument is NULL."));
        return (false);
    }
    // Generate a unique ID if ID is empty.
    if (unlikely(toolbar->GetId().IsEmpty())) {
        toolbar->SetId(NewId());
    }
    // Check if ID already exists
    if (unlikely(m_hashmap->find(toolbar->GetId()) != m_hashmap->end())) {
        wxLogError(_T("[penv::ToolBarList::Add] Cannot add toolbar, because the id \"%s\" already exists."), toolbar->GetId().c_str());
        return (false);
    }
    // Add toolbar
    (*m_hashmap)[toolbar->GetId()] = toolbar;
    return (true);
}


//----------------------------------------------------------------
bool ToolBarList::Remove(const wxString& id)
/**
 * \brief Removes a toolbar from the container.
 * To commit changes and remove toolbars call Update().
 * \param id The ID of the toolbar which should be removed.
 * \return True on success; false otherwise.
 **/
{
    ToolBarHashMap::iterator itr;
    if (unlikely((itr = m_hashmap->find(id)) == m_hashmap->end())) {
        wxLogError(_T("[penv::ToolbarList::Remove] cannot remove. id '%s' does not exist."), id.c_str());
        return (false);
    }
    ToolBar* tb = itr->second;

    if (tb->GetToolBar() != NULL) {
        if (m_manager->GetPane(tb->GetToolBar()).IsOk())
        {
            m_manager->DetachPane(tb->GetToolBar());
        }
        tb->GetToolBar()->Hide();
        tb->ClearToolEvents();
        tb->GetToolBar()->Destroy();
        tb->SetToolBar(NULL);
    }
    m_hashmap->erase(id);
    delete tb;
    return (true);
}


//----------------------------------------------------------------
void ToolBarList::Clear()
/**
 * \brief Deletes all toolbars from this container. The
 * allocated memory will be freed and events will be deregisterd.
 * To commit changes and remove toolbars call Update().
 **/
{
    if (m_hashmap != NULL)
    {
        ToolBarHashMap::iterator itr;
        for (itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
        {
            if (itr->second != NULL)
            {
                ToolBar* toolbar = itr->second;
                if (toolbar == NULL) continue;
                // TEST: Deregistrieren der Events oder wird das beim
                //       zerstören des wxToolBar objects gemacht?
                delete itr->second;
            }
        }
        m_hashmap->clear();
    }
}


//----------------------------------------------------------------
wxString ToolBarList::NewId()
/**
 * \brief Creates a new unique ID for the toolbar.
 * \return Unique ID for the toolbar.
 **/
{
    wxString uniqueid = PenvHelper::GenerateRandomHexValues(4);
    while (m_hashmap->find(uniqueid) != m_hashmap->end())
    {
        uniqueid = PenvHelper::GenerateRandomHexValues(4);
    }
    return (uniqueid);
}


//----------------------------------------------------------------
Array<ToolBarItem>* ToolBarList::FindToolBarItems(const wxString& commandid)
/**
 * \brief Searches for toolbar items who are connected with the
 * given commandid. The returned array must be destroyed by the caller
 * of this method.
 * \param commandid Command Id to search for.
 * \return Array with toolbar items.
 **/
{
    Array<ToolBarItem>* array = new Array<ToolBarItem>(false);
    ToolBarHashMap::iterator itr;
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        ToolBar* toolbar = itr->second;
        for (size_t i=0; i<toolbar->Count(); ++i)
        {
            if (toolbar->GetItem(i)->GetId() == commandid)
            {
                array->Add(toolbar->GetItem(i));
            }
        }
    }
    return (array);
}




//----------------------------------------------------------------
bool ToolBarList::Update()
/**
 * \brief Updates all toolbars in this container. That means
 * that all toolbars in the frame will be removed and inserted
 * like they are in this container.
 * \return True on success; false otherwise.
 **/
{
    bool error = false;
    ToolBarHashMap::iterator itr;
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        ToolBar* toolbar = itr->second;
        if (unlikely(toolbar == NULL)) continue;
        if (toolbar->GetToolBar() == NULL)
        {
            // If toolbar does not exists, create one
            wxToolBar* wxtoolbar = NULL;
            wxtoolbar = new wxToolBar(m_frame, wxID_ANY, wxDefaultPosition,
                wxDefaultSize, penvCOMMON_TOOLBARSTYLE);
            wxtoolbar->SetToolBitmapSize(penvCOMMON_TOOLBARICONSIZE);
            toolbar->SetToolBar(wxtoolbar);
        }
        else
        {
            // If toolbar exists, check if it is in manager
            if (m_manager->GetPane(toolbar->GetToolBar()).IsOk())
            {
                // Toolbar exists in manager, so it must be removed
                m_manager->DetachPane(toolbar->GetToolBar());
                toolbar->GetToolBar()->Hide();
            }
        }
        if (unlikely(!toolbar->Update())) {
            wxLogError(_T("[penv::ToolBarList::Update] Cannot update toolbar \"%s\"."), toolbar->GetName().c_str());
            error = true;
        }
    }
    m_manager->Update();
    return (!error);
}



//----------------------------------------------------------------
void ToolBarList::RefreshPaneInfos()
/**
 * \brief Refreshes the wxAuiPaneInfo objects in all toolbars.
 **/
{
    wxAuiManager* mgr = Environment::Get()->GetFrame()->GetManager();
    ToolBarHashMap::iterator itr;
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        ToolBar* tbar = itr->second;
        tbar->SetPaneInfo(mgr->GetPane(tbar->GetToolBar()));
    }
}



//----------------------------------------------------------------
void ToolBarList::RealizeAllToolBars()
/**
 * \brief Realizes all toolbars in the toolbar list.
 **/
{
    ToolBarHashMap::iterator itr;
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        ToolBar* tbar = itr->second;
        tbar->GetToolBar()->Realize();
    }
    Environment::Get()->GetFrame()->GetManager()->Update();
}





//----------------------------------------------------------------
bool ToolBarList::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'toolbars' xml element node.
 * See class description for structure of such an xml node.
 * \param node The 'toolbars' xml element node.
 * \return True on success; false otherwise.
 **/
{
    // Catch errors
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::ToolBarList::ReadNode] Cannot load xml node, because parameter argument is NULL."));
        return (false);
    }
    if (unlikely(node->GetType() != wxXML_ELEMENT_NODE)) {
        wxLogError(_T("[penv::ToolBarList::ReadNode] Cannot load, xml node must be an element node."));
        return (false);
    }
    if (unlikely(node->GetName() != _T("toolbars"))) {
        wxLogError(_T("[penv::ToolBarList::ReadNode] Cannot load, xml element node must have the name \"toolbars\". This element node has the name \"%s\"."), node->GetName().c_str());
        return (false);
    }
    // Begin reading
    wxXmlNode* child = node->GetChildren();
    while (child != NULL)
    {
        // Skip unknown elements and not element nodes
        if (unlikely(child->GetType() != wxXML_ELEMENT_NODE ||
            child->GetName() != _T("toolbar"))) {
            child = child->GetNext();
            continue;
        }
        ToolBar* toolbar = new ToolBar();
        if (unlikely(!toolbar->ReadNode(child))) {
            wxLogWarning(_T("[penv::ToolBarList::ReadNode] Cannot load toolbar. Skipping..."));
            delete toolbar;
            child = child->GetNext();
            continue;
        }
        Add(toolbar);
        child = child->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* ToolBarList::WriteNode()
/**
 * \brief Writes a 'toolbars' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'toolbars' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("toolbars"));
    ToolBarHashMap::iterator itr;
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); ++itr)
    {
        ToolBar* tbar = itr->second;
        wxXmlNode* tbarnode = tbar->WriteNode();
        PenvHelper::AddXmlChildNode(node, tbarnode);
    }
    return (node);
}





} // namespace penv


