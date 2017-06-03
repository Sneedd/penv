/*
 * propertylist.h - Declaration of the PropertyList class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#ifndef _PENV_PROPERTYLIST_H_
#define _PENV_PROPERTYLIST_H_

#include "wxincludes.h"
#include "property.h"


namespace penv {

/**
 * \brief The PropertyList class is a container for properties. The main use
 * of this class is the chance to save configurations for various objects in
 * the framework. See following list of classes which uses the PropertyList.
 * The advantage of this class is, that the properties can be saved in a xml
 * file. So no external configuration will be needed for these classes.<br>
 * A Property can contain also a PropertyList, therefore the configuration
 * can be build up as a hierarchy.<br>
 * Because of the hierarchical structure the properties of a deepert PropertyList
 * can be accessed from the first PropertyList if the property names are
 * separated with a slash '/'. Example:<br>
 * <code>
 * props->Set(_T("secondproplist/thirdproplist/boolprop"), true);<br>
 * wxString strg = props->GetString(_T("anothersecondproplist/stringprop"));<br>
 * </code><br>
 * List of classes where PropertyList is used:<br>
 * <ul>
 *  <li><b>Environment:</b> For saving configurations of the main application.</li>
 *  <li><b>Package:</b> For saving package configurations.</li>
 *  <li><b>Workspace:</b> For saving workspace spezific configurations.</li>
 *  <li><b>Project:</b> For saving project spezific configurations.</li>
 *  <li><b>ProjectItem:</b> For saving project item spezific configurations.
 *  This includes the following inherited classes ProjectItemFile,
 *  ProjectItemDirectory, ProjectItemLinkedItems and ProjectItemSubProject.</li>
 * </ul>
 * Here the definition of a 'properties' xml element:<br>
 * <code>
 *  &lt;properties&gt;<br>
 *  &nbsp;&nbsp;&lt;property name="boolprop" type="boolean"&gt;true&lt;/property&gt;<br>
 *  &nbsp;&nbsp;&lt;property name="intprop" type="integer"&gt;1000&lt;/property&gt;<br>
 *  &nbsp;&nbsp;&lt;property name="doubleprop" type="double"&gt;0.64372&lt;/property&gt;<br>
 *  &nbsp;&nbsp;&lt;property name="secondproplist" type="properties"&gt;<br>
 *  &nbsp;&nbsp;&nbsp;&nbsp;&lt;property name="boolprop" type="boolean"&gt;true&lt;/property&gt;<br>
 *  &nbsp;&nbsp;&nbsp;&nbsp;&lt;property name="intprop" type="integer"&gt;1000&lt;/property&gt;<br>
 *  &nbsp;&nbsp;&lt;/property&gt;<br>
 *  &nbsp;&nbsp;&lt;!-- other property elements should be here --&gt;<br>
 *  &lt;/properties&gt;<br>
 * </code>
 * \todo Implement the clone functionality.
 **/
class PropertyList
{
    public:
        PropertyList(Property* parent = NULL);
        PropertyList(const PropertyList& props);
        ~PropertyList();

        size_t Count();
        wxArrayString* GetProperties(bool recursive = false);
        bool ExistsProperty(const wxString& propname);
        PropertyType GetType(const wxString& propname);
        Property* GetParent();
        Property* GetProperty(const wxString& propname, bool create = false);
        void SetProperty(const wxString& propname, const Property& prop);

        int GetInteger(const wxString& propname);
        double GetDouble(const wxString& propname);
        bool GetBoolean(const wxString& propname);
        const wxString& GetString(const wxString& propname);
        PropertyList* GetPropertyList(const wxString& propname);


        void Set(const wxString& propname, int value);
        void Set(const wxString& propname, double value);
        void Set(const wxString& propname, bool value);
        void Set(const wxString& propname, const wxString& value);
        void Set(const wxString& propname, const wxChar* value);
        void Set(const wxString& propname, const PropertyList& value);

        PropertyList* Clone(){return NULL;}
        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

    private:
        WX_DECLARE_STRING_HASH_MAP(Property*, PropertiesHashMap);
        PropertiesHashMap* m_hashmap;
        wxString m_empty;
        Property* m_parent;
};

} // namespace penv

#endif // _PENV_PROPERTYLIST_H_
