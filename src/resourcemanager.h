/*
 * resourcemanager.h - Declaration of the ResourceManager class
 *
 * Author: Steffen Ott
 * Date: 05/22/2008
 *
 */


#ifndef _PENV_RESOURCEMANAGER_H_
#define _PENV_RESOURCEMANAGER_H_


namespace penv {

/**
 * \brief The ResourceManager class manages the resources for the
 * framework. You can use it for your own resources, but you must use
 * it, when you want to change the strings, images, ... for the framework.<br>
 * There is a spezial type of Resource File which is used XRS, this means
 * that the resources are compressed in a ZIP Archive.
 * \todo Not implemented yet.
 **/
class ResourceManager
{
    public:
        ResourceManager();
        virtual ~ResourceManager();
    protected:
    private:
};

} // namespace penv

#endif // _PENV_RESOURCEMANAGER_H_


