//
// Created by Vali on 4/13/2021.
//

#ifndef VDGE_MACROS_HPP
#define VDGE_MACROS_HPP

#ifdef VDGE_DEBUG
#define TERMINATE() abort()
#else
#define TERMINATE() ;
#endif

#define CORE_ASSERT(x, msg) { \
        if (!(x)) { \
            printf("Assertion failed: (%s)\n\tFunction: %s\n\t\tFile: %s\n\t\tLine: %d\nMessage: %s\n", \
                #x, __PRETTY_FUNCTION__, __FILE__, __LINE__, msg); \
                TERMINATE(); \
        } \
    }

#endif //VDGE_MACROS_HPP