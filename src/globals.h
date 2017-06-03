/*
 * globals.h - Defines some global informations about the penv framework.
 *
 * Author: Steffen Ott
 * Date: 04/02/2008
 *
 */

#ifndef _PENV_GLOBALS_H_
#define _PENV_GLOBALS_H_

#include <wx/version.h>

/**
 * \brief Definition of the top level window caption.
 **/
#define penvCOMMON_TOPLEVELWINDOWCAPTION _T("penv")

/**
 * \brief Definition if the framework should use the
 * home directories. 'true' to use them; 'false' otherwise.
 **/
#define penvCOMMON_CONFIGURATIONUSEHOMEDIRECTORIES false
/**
 * \brief Definition if the framework should use one
 * configuration for all users. 'true' for all users;
 * 'false' otherwise.
 **/
#define penvCOMMON_CONFIGURATIONFORALLUSERS false
/**
 * \brief Definition of the standard configuration
 * directory. Should be a single directory or a relative
 * path to the application path.
 **/
#define penvCOMMON_CONFIGURATIONDIRECTORY _T("config")
/**
 * \brief Definition of the log file path. This
 * string must be a ANSI string and not wrapped into
 * TEXT() macro.
 **/
#define penvCOMMON_LOGFILEPATH "penv.log"


/**
 * \brief Definition for the standard toolbar style.
 **/
#define penvCOMMON_TOOLBARSTYLE wxTB_HORIZONTAL|wxNO_BORDER|wxTB_FLAT|wxTB_NODIVIDER
/**
 * \brief Definition for the toolbar icon sizes.
 * Use wxSize(width, height) to define it.
 **/
#define penvCOMMON_TOOLBARICONSIZE wxSize(16,16)


/**
 * \brief Defines the selection mode of the project tree ctrl.
 * This can be wxTR_SINGLE or wxTR_MULTIPLE.
 * \todo Implement wxTR_MULTIPLE.
 **/
#define penvPROJECTTREECTRL_SELECTIONMODE wxTR_SINGLE


/**
 * \brief Definition of the frameworks version.
 * DO NOT CHANGE THIS!!!!!!
 **/
#define penvVERSION Version(0,8,0506)

/**
 * \brief Definition of the wxWidgets version used
 * to build this framework.
 **/
#define penvVERSION_WXWIDGETS Version(wxMAJOR_VERSION,wxMINOR_VERSION,wxRELEASE_NUMBER)


#endif // _PENV_GLOBALS_H_
