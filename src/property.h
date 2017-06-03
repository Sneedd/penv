/*
 * property.h - Declaration of the Property class
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */


#ifndef _PENV_PROPERTY_H_
#define _PENV_PROPERTY_H_

#include "wxincludes.h"
#include "propertytype.h"

namespace penv {

// Forward Definition
class PropertyList;

/**
 * \brief The Property class is a item in a PropertyList. This class
 * should not be used directly, instead use the methods in the
 * PropertyList class.<br>
 * A Property can contain 4 differnt types of values: Integer,
 * Boolean, Strings and Double.
 **/
class Property
{
    public:
        Property();
        Property(const wxString& name, int value);
        Property(const wxString& name, bool value);
        Property(const wxString& name, const wxString& value);
        Property(const wxString& name, PropertyList* value);
        ~Property();

        PropertyType GetType() const;
        void SetType(PropertyType type);

        const wxString& GetName() const;
        void SetName(const wxString& name);

        bool GetBoolean() const;
        void SetBoolean(bool value);

        int GetInteger() const;
        void SetInteger(int value);

        double GetDouble() const;
        void SetDouble(double value);

        const wxString& GetString() const;
        void SetString(const wxString& value);

        PropertyList* GetPropertyList() const;
        void SetPropertyList(PropertyList* value);

        bool ReadNode(wxXmlNode* node);
        wxXmlNode* WriteNode();

    private:
        union {
            bool          Boolean;
            int           Integer;
            double        Double;
            wxString*     String;
            PropertyList* Props;
        } m_value;
        PropertyType m_type;
        wxString m_name;
};

} // namespace penv


#endif // _PENV_PROPERTY_H_


