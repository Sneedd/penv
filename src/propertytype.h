/*
 * propertytype.h - Declaration of the PropertyType enumerator
 *
 * Author: Steffen Ott
 * Date: 09/18/2007
 *
 */

#ifndef _PENV_PROPERTYTYPE_H_
#define _PENV_PROPERTYTYPE_H_

namespace penv {


/**
 * \brief The ProjectItemType enumerator spezifies the type of
 * a Property.
 **/
enum PropertyType
{
    /**
     * \brief Spezifies that the Property is not definied.
     **/
    penvPT_None,
    /**
     * \brief Spezifies that the Property is a PropertyList.
     **/
    penvPT_Properties,
    /**
     * \brief Spezifies that the Property is a Boolean.
     **/
    penvPT_Boolean,
    /**
     * \brief Spezifies that the Property is a Integer.
     **/
    penvPT_Integer,
    /**
     * \brief Spezifies that the Property is a Double.
     **/
    penvPT_Double,
    /**
     * \brief Spezifies that the Property is a String.
     **/
    penvPT_String,
    /**
     * \brief Spezifies that the Property is a Array of Booleans.
     * \todo This feature is not implementet yet.
     **/
    penvPT_ArrayBoolean,
    /**
     * \brief Spezifies that the Property is a Array of Integers.
     * \todo This feature is not implementet yet.
     **/
    penvPT_ArrayInteger,
    /**
     * \brief Spezifies that the Property is a Array of Doubles.
     * \todo This feature is not implementet yet.
     **/
    penvPT_ArrayDouble,
    /**
     * \brief Spezifies that the Property is a Array of Strings.
     * \todo This feature is not implementet yet.
     **/
    penvPT_ArrayString
};

} // namespace penv

#endif // _PENV_PROPERTYTYPE_H_
