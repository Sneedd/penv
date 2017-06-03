/*
 * property.cpp - Implementation of the Property class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */

#include "property.h"

#include "propertylist.h"
#include "expect.h"
#include "penvhelper.h"


namespace penv {

//----------------------------------------------------------------
Property::Property()
/**
 * \brief Constructor, initialize the property object.
 **/
{
    m_type = penvPT_None;
    m_name = wxEmptyString;
    m_value.Integer = 0;
}


//----------------------------------------------------------------
Property::Property(const wxString& name, int value)
/**
 * \brief Constructor, initialize the property object as integer.
 **/
{
    m_type = penvPT_Integer;
    m_name = name;
    m_value.Integer = value;
}


//----------------------------------------------------------------
Property::Property(const wxString& name, bool value)
/**
 * \brief Constructor, initialize the property object as boolean.
 **/
{
    m_type = penvPT_Boolean;
    m_name = name;
    m_value.Boolean = value;
}


//----------------------------------------------------------------
Property::Property(const wxString& name, const wxString& value)
/**
 * \brief Constructor, initialize the property object as string.
 **/
{
    m_type = penvPT_String;
    m_name = name;
    m_value.String = new wxString(value);
}


//----------------------------------------------------------------
Property::Property(const wxString& name, PropertyList* value)
/**
 * \brief Constructor, initialize the property object as propertylist.
 **/
{
    m_type = penvPT_Properties;
    m_name = name;
    m_value.Props = value;
}


//----------------------------------------------------------------
Property::~Property()
/**
 * \brief Destructor.
 **/
{
    if (m_type == penvPT_Properties)
    {
        if (m_value.Props != NULL) delete (m_value.Props);
    }
    else if (m_type == penvPT_String)
    {
        if (m_value.String != NULL) delete (m_value.String);
    }
}


//----------------------------------------------------------------
PropertyType Property::GetType() const
/**
 * \brief Returns the type of the property.
 * \return Type of the property.
 **/
{
    return (m_type);
}


//----------------------------------------------------------------
void Property::SetType(PropertyType type)
/**
 * \brief Sets the type of the property.
 * \param type Type of the property.
 **/
{
    m_type = type;
}


//----------------------------------------------------------------
const wxString& Property::GetName() const
/**
 * \brief Returns the name of the property.
 * \return Name of the property.
 **/
{
    return (m_name);
}


//----------------------------------------------------------------
void Property::SetName(const wxString& name)
/**
 * \brief Sets the name of the property.
 * \param name Name of the property.
 **/
{
    m_name = name;
}


//----------------------------------------------------------------
bool Property::GetBoolean() const
/**
 * \brief Returns the property as a boolean. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \return Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Boolean)) {
        wxLogWarning(_T("[penv::Property::GetBoolean] Reading Property %s who is not Type Boolean"), m_name.c_str());
    }
    return (m_value.Boolean);
}


//----------------------------------------------------------------
void Property::SetBoolean(bool value)
/**
 * \brief Sets the value of the proprerty as boolean. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \param value Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Boolean)) {
        wxLogWarning(_T("[penv::Property::SetBoolean] Writing Property %s who is not Type Boolean"), m_name.c_str());
        m_type = penvPT_Boolean;
    }
    m_value.Boolean = value;
}


//----------------------------------------------------------------
int Property::GetInteger() const
/**
 * \brief Returns the property as a integer. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \return Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Integer)) {
        wxLogWarning(_T("[penv::Property::GetInteger] Reading Property %s who is not Type Integer"), m_name.c_str());
    }
    return (m_value.Integer);
}



//----------------------------------------------------------------
void Property::SetInteger(int value)
/**
 * \brief Sets the value of the proprerty as integer. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \param value Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Integer)) {
        wxLogWarning(_T("[penv::Property::SetInteger] Writing Property %s who is not type integer."), m_name.c_str());
        m_type = penvPT_Integer;
    }
    m_value.Integer = value;
}


//----------------------------------------------------------------
double Property::GetDouble() const
/**
 * \brief Returns the property as a double. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \return Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Double)) {
        wxLogWarning(_T("[penv::Property::GetDouble] Reading Property %s who is not type double."), m_name.c_str());
    }
    return (m_value.Double);
}


//----------------------------------------------------------------
void Property::SetDouble(double value)
/**
 * \brief Sets the value of the proprerty as double. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \param value Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Double)) {
        wxLogWarning(_T("[penv::Property::SetDouble] Writing Property %s who is not type double."), m_name.c_str());
        m_type = penvPT_Double;
    }
    m_value.Double = value;
}


//----------------------------------------------------------------
const wxString& Property::GetString() const
/**
 * \brief Returns the property as a string. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \return Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_String)) {
        wxLogWarning(_T("[penv::Property::GetString] Reading Property %s who is not type string"), m_name.c_str());
        return (wxEmptyString);
    }
    return (*(m_value.String));
}


//----------------------------------------------------------------
void Property::SetString(const wxString& value)
/**
 * \brief Sets the value of the proprerty as string. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \param value Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_String)) {
        wxLogWarning(_T("[penv::Property::SetString] Writing Property %s who is not Type String"), m_name.c_str());
        m_type = penvPT_String;

    }
    else
    {
        if (m_value.String == NULL) {
            m_value.String = new wxString(value);
        } else {
            (*(m_value.String)) = value;
        }
    }
}


//----------------------------------------------------------------
PropertyList* Property::GetPropertyList() const
/**
 * \brief Returns the property as a propertieslist. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \return Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Properties)) {
        wxLogWarning(_T("[penv::Property::GetPropertyList] Reading Property %s who is not Type PropertyList"), m_name.c_str());
    }
    return (m_value.Props);
}


//----------------------------------------------------------------
void Property::SetPropertyList(PropertyList* value)
/**
 * \brief Sets the value of the proprerty as propertylist. This method
 * will generate a warning, when the property as a incorrect
 * type.
 * \param value Value of this property.
 **/
{
    if (unlikely(m_type != penvPT_Properties)) {
        wxLogWarning(_T("[penv::Property::SetPropertyList] Writing Property %s who is not Type PropertyList."), m_name.c_str());
    }
    m_value.Props = value;
}



//----------------------------------------------------------------
bool Property::ReadNode(wxXmlNode* node)
/**
 * \brief Read out a 'propery' xml element node.
 * See PropertyList class description for structure of such an xml node.
 * \param node The 'property' xml element node.
 * \return True on success; false otherwise.
 **/
{
    if (node == NULL)
    {
        wxLogError(_T("[penv::Property::ReadNode] Argument 'node' is NULL."));
        return (false);
    }
    if (node->GetType() != wxXML_ELEMENT_NODE || node->GetName() != _T("property"))
    {
        wxLogError(_T("[penv::Property::ReadNode] Xml node must be an element node with name 'property'."));
        return (false);
    }
    // Read out name and type
    m_name = node->GetPropVal(_T("name"), wxEmptyString);
    if (m_name.IsEmpty())
    {
        wxLogError(_T("[penv::Property::ReadNode] Property node does not have the 'name' attribute."));
        return (false);
    }
    wxString typestring = node->GetPropVal(_T("type"), wxEmptyString);
    if (typestring.IsEmpty())
    {
        wxLogError(_T("[penv::Property::ReadNode] Property node does not have the 'type' attribute."));
        return (false);
    }
    // Resolve type
    if (typestring == _T("boolean")) m_type = penvPT_Boolean;
    else if (typestring == _T("integer")) m_type = penvPT_Integer;
    else if (typestring == _T("double")) m_type = penvPT_Double;
    else if (typestring == _T("string")) m_type = penvPT_String;
    else if (typestring == _T("properties")) m_type = penvPT_Properties;
    else if (typestring == _T("arrayboolean")) m_type = penvPT_ArrayBoolean;
    else if (typestring == _T("arrayinteger")) m_type = penvPT_ArrayInteger;
    else if (typestring == _T("arraydouble")) m_type = penvPT_ArrayDouble;
    else if (typestring == _T("arraystring")) m_type = penvPT_ArrayString;
    else
    {
        wxLogError(_T("[penv::Property::ReadNode] Propery XmlNode %s Type Attribute is unknown."), m_name.c_str());
        return (false);
    }
    // Read out the value
    if (m_type == penvPT_Boolean) {
        wxString boolstring = node->GetNodeContent();
        if (boolstring == _T("true")) m_value.Boolean = true;
        else m_value.Boolean = false;
    }
    else if (m_type == penvPT_Integer) {
        wxString intstring = node->GetNodeContent();
        long value = 0;
        if (!intstring.ToLong(&value)) {
            wxLogWarning(_T("[penv::Property::ReadNode] On Property XmlNode %s convert to integer failed, default value 0 is set."), m_name.c_str());
            value = 0;
        }
        m_value.Integer = (int)value;
    }
    else if (m_type == penvPT_Double) {
        wxString doublestring = node->GetNodeContent();
        double value = 0;
        if (!doublestring.ToDouble(&value)) {
            wxLogWarning(_T("[penv::Property::ReadNode] On Property XmlNode %s convert to double failed, default value 0 is set."), m_name.c_str());
            value = 0;
        }
        m_value.Double = value;
    }
    else if (m_type == penvPT_String) {
        m_value.String = new wxString(node->GetNodeContent());
    }
    else if (m_type == penvPT_Properties) {
        PropertyList* props = new PropertyList(this);
        props->ReadNode(node);
        m_value.Props = props;
    }
    else if (m_type == penvPT_ArrayBoolean) {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvPT_ArrayInteger) {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvPT_ArrayDouble) {
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvPT_ArrayString) {
        NOT_IMPLEMENTED_YET();
    }
    return (true);
}


//----------------------------------------------------------------
wxXmlNode* Property::WriteNode()
/**
 * \brief Writes a 'property' xml element node.
 * See class description for structure of such an xml node.
 * \return The 'property' xml element node; or NULL on error.
 **/
{
    // Predefine content
    wxString content = wxEmptyString;
    if (m_type == penvPT_Boolean) {
        content = PenvHelper::CreateBoolean(m_value.Boolean);
    }
    else if (m_type == penvPT_Integer) {
        content = wxString::Format(_T("%i"), m_value.Integer);
    }
    else if (m_type == penvPT_Double) {
        content = wxString::Format(_T("%f"), m_value.Double);
    }
    else if (m_type == penvPT_String) {
        content = *(m_value.String);
    }
    // Now create node, with predefinied content
    wxXmlNode* node = PenvHelper::CreateXmlNode(_T("property"), content);
    node->AddProperty(_T("name"), m_name);
    // Set type attribute and additional stuff, if needed
    if (m_type == penvPT_Boolean) {
        node->AddProperty(_T("type"), _T("boolean"));
    }
    else if (m_type == penvPT_Integer) {
        node->AddProperty(_T("type"), _T("integer"));
    }
    else if (m_type == penvPT_Double) {
        node->AddProperty(_T("type"), _T("double"));
    }
    else if (m_type == penvPT_String) {
        node->AddProperty(_T("type"), _T("string"));
    }
    else if (m_type == penvPT_Properties) {
        node->AddProperty(_T("type"), _T("properties"));
        wxArrayString* array = m_value.Props->GetProperties(false);
        for (size_t i=0; i<array->Count(); ++i)
        {
            Property* prop = m_value.Props->GetProperty((*array)[i]);
            wxXmlNode* child = prop->WriteNode();
            if (child == NULL)
            {
                wxLogError(_T("[penv::Property::WriteNode] Failed to write property '%s', skipping."), prop->GetName().c_str());
                continue;
            }
            PenvHelper::AddXmlChildNode(node, child);
        }
        delete array;
    }
    else if (m_type == penvPT_ArrayBoolean) {
        node->AddProperty(_T("type"), _T("arrayboolean"));
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvPT_ArrayInteger) {
        node->AddProperty(_T("type"), _T("arrayinteger"));
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvPT_ArrayDouble) {
        node->AddProperty(_T("type"), _T("arraydouble"));
        NOT_IMPLEMENTED_YET();
    }
    else if (m_type == penvPT_ArrayString) {
        node->AddProperty(_T("type"), _T("arraystring"));
        NOT_IMPLEMENTED_YET();
    }
    return (node);
}



} // namespace penv



