/*
 * propertylist.cpp - Implementation of the PropertyList class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */



#include "propertylist.h"
#include "expect.h"
#include "penvhelper.h"

namespace penv {


//----------------------------------------------------------------
PropertyList::PropertyList(Property* parent)
/**
 * \brief Constructor, initialize the PropertyList object.
 * The parent parameter spezifies if this PropertyList has
 * a parent list.
 * \param parent Parent PropertyList.
 **/
{
    m_parent = parent;
    m_hashmap = new PropertiesHashMap();
    m_empty = wxEmptyString;
}


//----------------------------------------------------------------
PropertyList::PropertyList(const PropertyList& props)
/**
 * \brief Copyconstructor, creates a new copy of the whole
 * propertylist from the given PropertyList.
 * \param props PropertyList which should be copied.
 **/
{
    // TODO SO:
    NOT_IMPLEMENTED_YET();
}



//----------------------------------------------------------------
PropertyList::~PropertyList()
/**
 * \brief Destructor.
 **/
{
    // TODO SO: Kill the hashmap
    NOT_IMPLEMENTED_YET();
}


//----------------------------------------------------------------
size_t PropertyList::Count()
/**
 * \brief Returns the number of properies on this level. That means
 * not all properties in the hierarchical structure will be counted.
 * \return Number of properties on this level.
 **/
{
    return (m_hashmap->size());
}



//----------------------------------------------------------------
wxArrayString* PropertyList::GetProperties(bool recursive)
/**
 * \brief Returns all property names from this PropertyList.
 * If the parameter 'recursive' is true all properties will be
 * recursively inserted into the returning string array, with
 * a slash who separates every property from one level to another.
 * If 'recursive' is false only the properties from this level
 * are returned.
 * \param recursive True if all properties should be returned.
 * \return String array with property names.
 **/
{
    // Array erzeugen
    wxArrayString* array = new wxArrayString();
    array->Alloc(m_hashmap->size());
    // Durch Hashmap durchschleifen
    PropertiesHashMap::iterator itr;
    //if (m_hashmap->size() <= 0) return (array);
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); itr++)
    {
        // Properties in Array speichern
        array->Add(itr->first);
        // Überprüfen ob rekursiv die Properties geholt werden müssen
        Property* prop = itr->second;
        if (recursive && prop->GetType() == penvPT_Properties)
        {
            // Rekursiv aufrufen
            wxArrayString* recarray = prop->GetPropertyList()->GetProperties(recursive);
            for (size_t i=0; i<recarray->Count(); ++i)
            {
                wxString strg = itr->first;
                strg.Append('/').Append(recarray->Item(i).c_str());
                array->Add(strg);
            }
            delete recarray;
        }
    }
    return (array);
}



//----------------------------------------------------------------
bool PropertyList::ExistsProperty(const wxString& propname)
/**
 * \brief Checks if a property exists in the property hierarchy.
 * Use slash to separate one property from another.
 * \param propname The property name.
 * \return True if property exists; false otherwise.
 **/
{
    return (GetProperty(propname, false) == NULL);
}



//----------------------------------------------------------------
PropertyType PropertyList::GetType(const wxString& propname)
/**
 * \brief Returns the type of the property. This method will
 * return penvPT_None if the property does not exists and an
 * error will be generated.
 * Use slash to separate one property from another.
 * \param propname The property name.
 * \return Type of the property (see PropertyType enumerator).
 **/
{
    Property* prop = GetProperty(propname, false);
    if (unlikely(prop == NULL)) return (penvPT_None);
    return (prop->GetType());
}


//----------------------------------------------------------------
Property* PropertyList::GetParent()
/**
 * \brief Returns the parent property of this PropertyList. If this method
 * returns NULL, then this propertylist is the root PropertyList.
 * \return Parent property or NULL if PropertyList is the root list.
 **/
{
    return (m_parent);
}


//----------------------------------------------------------------
Property* PropertyList::GetProperty(const wxString& propname, bool create)
/**
 * \brief Returns a property with the given name. If the property does not
 * exists and create is false then NULL will be returned. If the property
 * does not exists and create is true, then the property will be created.
 * To differ from created and not created property, a created property has
 * the penvPT_None type.
 * \param propname The property name.
 * \param create True if not existed property should be created.
 * \return The found property or NULL.
 **/
{
    // Propertyname aufsplitten (nach '/' parsen)
    wxArrayString names;
    PenvHelper::SplitString(propname, &names);

    // String Array enthält nur ein Element wenn das Property aus
    //   dieser Liste benötigt wird
    if (names.Count() == 1) {
        // Testen ob das Property existiert
        if (m_hashmap->find(propname) == m_hashmap->end())
        {
            // Property existiert nicht
            if (create) {
                Property* newprop = new Property();
                newprop->SetName(propname);
                (*m_hashmap)[propname] = newprop;
                return (newprop);
            } else {
                return (NULL);
            }
        }
        else
        {
            // Property existiert
            return ((*m_hashmap)[propname]);
        }
    }

    // Suchen des Properties (rekursiv)
    // NOTE: Da Tiefe bekannt rekursion nicht nötig!
    // Prüfen ob das erste Property vorhanden ist
    if (m_hashmap->find(names[0]) == m_hashmap->end())
    {
        // Property existiert nicht
        if (create)
        {
            // First property which will be created, is always
            //  of type propertylist
            Property* firstprop = new Property();
            firstprop->SetType(penvPT_Properties);
            firstprop->SetName(names[0]);
            PropertyList* firstproplist = new PropertyList(firstprop);
            firstprop->SetPropertyList(firstproplist);
            (*m_hashmap)[names[0]] = firstprop;
            PropertyList* temp = firstproplist;
            for (size_t i=1; i<names.Count(); ++i)
            {
                // Only last property is not type propertylist
                if (i < names.Count()-1)
                {   // Not the last property
                    Property* prop = new Property();
                    prop->SetType(penvPT_Properties);
                    prop->SetName(names[i]);
                    PropertyList* proplist = new PropertyList(prop);
                    prop->SetPropertyList(proplist);
                    (*(temp->m_hashmap))[names[i]] = prop;
                    temp = proplist;
                }
                else
                {   // Here comes the last property
                    Property* prop = new Property();
                    prop->SetType(penvPT_None);
                    prop->SetName(names[i]);
                    (*(temp->m_hashmap))[names[i]] = prop;
                    return (prop);
                }
            }
        } else {
            return (NULL);
        }
    }
    else
    {
        // Property existiert
        // Neuer Name zusammensetzten und rekursiv aufrufen
        wxString name = names[1];
        for (size_t i=2; i<names.Count(); ++i)
        {
            name += '/';
            name += names[i];
        }
        return ((*m_hashmap)[names[0]]->GetPropertyList()->GetProperty(name, create));
    }

    // Wenn er hier ankommt muss ein Fehler aufgetreten sein
    wxLogError(_T("[penv::PropertyList::GetProperty] Unhandled error occured."));
    return (NULL);
}

//----------------------------------------------------------------
void PropertyList::SetProperty(const wxString& propname, const Property& prop)
/**
 * \brief Sets the property with the given name. If the property does
 * not exists the property will be created. If the property already exists
 * the property will be overridden.
 * \param propname The property name.
 * \param prop The property.
 **/
{
    Property* setprop = GetProperty(propname, true);
    if (setprop->GetType() != prop.GetType())
    {
        setprop->SetType(prop.GetType());
        setprop->SetName(prop.GetName());
    }
    switch (setprop->GetType())
    {
        case penvPT_Boolean:
            setprop->SetBoolean(prop.GetBoolean());
        break;
        case penvPT_Integer:
            setprop->SetInteger(prop.GetInteger());
        break;
        case penvPT_Double:
            setprop->SetDouble(prop.GetDouble());
        break;
        case penvPT_String:
            setprop->SetString(prop.GetString());
        break;
        case penvPT_Properties:
            setprop->SetPropertyList(prop.GetPropertyList());
        break;
        case penvPT_ArrayBoolean:
            NOT_IMPLEMENTED_YET();
        break;
        case penvPT_ArrayInteger:
            NOT_IMPLEMENTED_YET();
        break;
        case penvPT_ArrayDouble:
            NOT_IMPLEMENTED_YET();
        break;
        case penvPT_ArrayString:
            NOT_IMPLEMENTED_YET();
        break;
        default:
            wxString msg = wxString::Format(_T("Unknown type for property %s."), setprop->GetName().c_str());
        break;
    }
}


//----------------------------------------------------------------
int PropertyList::GetInteger(const wxString& propname)
/**
 * \brief Returns the integer value from a known property.
 * The method returns -1 if the property does not exists and
 * a error will be generated.
 * \param propname The property name.
 * \return Value of that property.
 **/
{
    Property* prop = GetProperty(propname, false);
    if (prop == NULL) {
        wxLogError(_T("[penv::PropertyList::GetInteger] The property '%s' does not exists."), propname.c_str());
        return (-1);
    }
    return (prop->GetInteger());
}



//----------------------------------------------------------------
double PropertyList::GetDouble(const wxString& propname)
/**
 * \brief Returns the double value from a known property.
 * The method returns -1 if the property does not exists and
 * a error will be generated.
 * \param propname The property name.
 * \return Value of that property.
 **/
{
    Property* prop = GetProperty(propname, false);
    if (prop == NULL) {
        wxLogError(_T("[penv::PropertyList::GetDouble] The property '%s' does not exists."), propname.c_str());
        return (-1);
    }
    return (prop->GetDouble());
}




//----------------------------------------------------------------
bool PropertyList::GetBoolean(const wxString& propname)
/**
 * \brief Returns the double value from a known property.
 * The method returns false if the property does not exists and
 * a error will be generated.
 * \param propname The property name.
 * \return Value of that property.
 **/
{
    Property* prop = GetProperty(propname, false);
    if (prop == NULL) {
        wxLogError(_T("[penv::PropertyList::GetBoolean] The property '%s' does not exists."), propname.c_str());
        return (false);
    }
    return (prop->GetBoolean());
}


//----------------------------------------------------------------
const wxString& PropertyList::GetString(const wxString& propname)
/**
 * \brief Returns the string value from a known property.
 * The method returns an empty string if the property does not exists and
 * a error will be generated.
 * \param propname The property name.
 * \return Value of that property.
 **/
{
    Property* prop = GetProperty(propname, false);
    if (prop == NULL) {
        wxLogError(_T("[penv::PropertyList::GetString] The property '%s' does not exists."), propname.c_str());
        return (m_empty);
    }
    return (prop->GetString());
}


//----------------------------------------------------------------
PropertyList* PropertyList::GetPropertyList(const wxString& propname)
/**
 * \brief Returns the propertylist from a known property.
 * The method returns NULL if the property does not exists and
 * a error will be generated.
 * \param propname The property name.
 * \return Value of that property.
 **/
{
    Property* prop = GetProperty(propname, false);
    if (prop == NULL) {
        wxLogError(_T("[penv::PropertyList::GetPropertyList] The property '%s' does not exists."), propname.c_str());
        return (NULL);
    }
    return (prop->GetPropertyList());
}

//----------------------------------------------------------------
void PropertyList::Set(const wxString& propname, int value)
/**
 * \brief Sets the integer value of a known property. If the
 * property does not exists it will be created. If the property
 * exists and has the worng type, it will be overridden and a warning
 * will be created.
 * \param propname The property name.
 * \param value Value to set for the property.
 **/
{
    Property* prop = GetProperty(propname, true);
    if (prop->GetType() == penvPT_None) prop->SetType(penvPT_Integer);
    prop->SetInteger(value);
}


//----------------------------------------------------------------
void PropertyList::Set(const wxString& propname, double value)
/**
 * \brief Sets the double value of a known property. If the
 * property does not exists it will be created. If the property
 * exists and has the worng type, it will be overridden and a warning
 * will be created.
 * \param propname The property name.
 * \param value Value to set for the property.
 **/
{
    Property* prop = GetProperty(propname, true);
    if (prop->GetType() == penvPT_None) prop->SetType(penvPT_Double);
    prop->SetDouble(value);
}



//----------------------------------------------------------------
void PropertyList::Set(const wxString& propname, bool value)
/**
 * \brief Sets the boolean value of a known property. If the
 * property does not exists it will be created. If the property
 * exists and has the worng type, it will be overridden and a warning
 * will be created.
 * \param propname The property name.
 * \param value Value to set for the property.
 **/
{
    Property* prop = GetProperty(propname, true);
    if (prop->GetType() == penvPT_None) prop->SetType(penvPT_Boolean);
    prop->SetBoolean(value);
}


//----------------------------------------------------------------
void PropertyList::Set(const wxString& propname, const wxString& value)
/**
 * \brief Sets the string value of a known property. If the
 * property does not exists it will be created. If the property
 * exists and has the worng type, it will be overridden and a warning
 * will be created.
 * \param propname The property name.
 * \param value Value to set for the property.
 **/
{
    Property* prop = GetProperty(propname, true);
    if (prop->GetType() == penvPT_None) prop->SetType(penvPT_String);
    prop->SetString(value);
}



//----------------------------------------------------------------
void PropertyList::Set(const wxString& propname, const wxChar* value)
/**
 * \brief Sets the string value of a known property. If the
 * property does not exists it will be created. If the property
 * exists and has the worng type, it will be overridden and a warning
 * will be created.
 * \param propname The property name.
 * \param value Value to set for the property.
 **/
{
    Set(propname, wxString(value));
}


//----------------------------------------------------------------
void PropertyList::Set(const wxString& propname, const PropertyList& value)
/**
 * \brief Sets the propertylist value of a known property. If the
 * property does not exists it will be created. If the property
 * exists and has the worng type, it will be overridden and a warning
 * will be created.
 * \param propname The property name.
 * \param value Value to set for the property.
 **/
{
    Property* prop = GetProperty(propname, true);
    if (prop->GetType() == penvPT_None) prop->SetType(penvPT_Properties);
    PropertyList* list = new PropertyList(value);
    prop->SetPropertyList(list);
}




//----------------------------------------------------------------
bool PropertyList::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'properties' xml element node or a 'property'
 * xml element node where type attribute is 'properties'.
 * See class description for structure of such an xml node.
 * \param node The 'properties' xml element node or 'property' node
 * with spezific a spezific type.
 * \return True on success; false otherwise.
 **/
{
    // Fehler abfangen
    if (unlikely(node == NULL)) {
        wxLogError(_T("[penv::PropertyList::ReadNode] Properties node is NULL."));
        return (false);
    }
    if (unlikely(node->GetType() != wxXML_ELEMENT_NODE)) {
        wxLogError(_T("[penv::PropertyList::ReadNode] Given properties node '%s' is not an element."), node->GetName().c_str());
        return (false);
    }
    if (node->GetName() != _T("properties") && (node->GetName() == _T("property") &&
        node->GetPropVal(_T("type"), wxEmptyString) != _T("properties")))
    {
        wxLogError(_T("[penv::PropertyList::ReadNode] Node must have the name 'properties' or 'property' with type set to 'properties'."));
        return (false);
    }
    // Alle Elemente in dem Node einlesen
    wxXmlNode* child = node->GetChildren();
    while (child != NULL)
    {
        // Nicht Element Knoten überspringen
        if (child->GetType() != wxXML_ELEMENT_NODE || child->GetName() != _T("property")) {
            child = child->GetNext();
            continue;
        }
        // Property erstellen und auslesen
        Property* prop = new Property();
        if (unlikely(!prop->ReadNode(child))) {
            // Wenn ein Fehler aufgetreten ist, dann eine Warning ausgeben
            // und nicht hinzufügen
            wxLogWarning(_T("[penv::PropertyList::ReadNode] Could not read property \"%s\"."), child->GetName().c_str());
        } else {
            // Überprüfen, ob das Property mit dem Namen schon in der
            // Liste existiert
            if (m_hashmap->find(prop->GetName()) != m_hashmap->end()) {
                wxLogWarning(_T("[penv::PropertyList::ReadNode] Property with the name \"%s\" already exists."), prop->GetName().c_str());
            } else {
                (*m_hashmap)[prop->GetName()] = prop;
            }
        }
        child = child->GetNext();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* PropertyList::WriteNode()
/**
* \brief Writes a 'properties' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'properties' xml element node; or NULL on error.
 **/
{
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("properties"));
    PropertiesHashMap::iterator itr;
    for (itr = m_hashmap->begin(); itr != m_hashmap->end(); itr++)
    {
        Property* prop = itr->second;
        wxXmlNode* child = prop->WriteNode();
        if (child == NULL)
        {
            wxLogError(_T("[penv::PropertyList::WriteNode] Failed to write property '%s', skipping."), prop->GetName().c_str());
            continue;
        }
        PenvHelper::AddXmlChildNode(node, child);
    }
    return (node);
}

} // namespace penv


