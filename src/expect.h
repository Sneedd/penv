/*
 * expect.h - Definition of the expect macros likley and unlikely
 *
 * Author: Steffen Ott
 * Date: 09/23/2007
 *
 */

#ifndef _PENV_EXPECT_H_
#define _PENV_EXPECT_H_


/**
 * \def likely(x) Uses the buildin expect for prediction that the 'if'
 * statement will be normally 'true'.
 * \def unlikely(x) Uses the buildin expect for prediction that the 'if'
 * statement will be normally 'false'.
 **/

#ifndef likely
    #ifdef __GNUC__ // Check if GNUC is used
        #define likely(x)   __builtin_expect((x), 1)
    #else
        #warning No __buildin_expect() macro defined by the compiler
        #define likely(x)   (x)
    #endif
#endif

#ifndef unlikely
    #ifdef __GNUC__ // Check if GNUC is used
        #define unlikely(x) __builtin_expect((x), 0)
    #else
        #warning No __buildin_expect() macro defined by the compiler
        #define unlikely(x) (x)
    #endif
#endif

#endif // _PENV_EXPECT_H_


