// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_DEBUG_BUILDDESCRIPTOR_H
#define THERON_DETAIL_DEBUG_BUILDDESCRIPTOR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Theron/Assert.h>
#include <Theron/Defines.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4996)  // function or variable may be unsafe.
#pragma warning (disable:4127)  // conditional expression is constant.
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
Generates strings that characterize the settings used to build Theron.

By comparing strings generated in the library with strings built in user code,
we can detect potential errors caused by building user code with settings different
from those used to build the Theron library. Some of the defines in Theron/Defines.h
affect code generation and behavior, so must be consistent across the build.

That means, for example, that a release build of the Theron library can't be mixed
with a debug build of client code that uses it.

Note that it's not generally safe to override the default definitions by simply
defining them in user code, even if they are defined before the inclusion of any
Theron headers. Instead they should be defined globally, for the whole build, ideally
by defining them via compiler options.
*/
class BuildDescriptor
{
public:

    /**
    Checks that build descriptors generated in the library and in user code match.
    */
    inline static void Check();

private:

    /**
    Gets a string descriptor characterizing the build settings used to build the Theron library.
    */
    static void GenerateInLibrary(char *const identifier);

    /**
    Fills a provided buffer with a string descriptor characterizing build settings.
    */
    inline static void GenerateInline(char *const identifier);
};


inline void BuildDescriptor::Check()
{
#if THERON_ENABLE_BUILD_CHECKS

    char libraryDescriptor[256];
    GenerateInLibrary(libraryDescriptor);

    char inlineDescriptor[256];
    GenerateInline(inlineDescriptor);

    if (strcmp(inlineDescriptor, libraryDescriptor) != 0)
    {
        fprintf(stderr, "Detected build differences between Theron library and user code\n");
        fprintf(stderr, "Client:  %s\n", inlineDescriptor);
        fprintf(stderr, "Library: %s\n", libraryDescriptor);

        exit(1);
    }

#endif // THERON_ENABLE_BUILD_CHECKS
}


// NOTE: This must match the similar code in GenerateInLibrary!
inline void BuildDescriptor::GenerateInline(char *const identifier)
{
    char buffer[16];

    THERON_ASSERT(identifier);

    // Build up a string identifier that characterizes the client build settings.
    strcpy(identifier, THERON_VERSION);

    if (THERON_64BIT)
    {
        strcat(identifier, ".64bit");
    }
    else
    {
        strcat(identifier, ".32bit");
    }

    sprintf(buffer, ".%dbytes", THERON_CACHELINE_ALIGNMENT);
    strcat(identifier, buffer);

    if (THERON_MSVC)
    {
        strcat(identifier, ".msvc");
    }

    if (THERON_GCC)
    {
        strcat(identifier, ".gcc");
    }

    if (THERON_DEBUG)
    {
        strcat(identifier, ".debug");
    }

    if (THERON_BOOST)
    {
        strcat(identifier, ".boost");
    }

    if (THERON_CPP11)
    {
        strcat(identifier, ".cpp11");
    }

    if (THERON_POSIX)
    {
        strcat(identifier, ".posix");
    }

    if (THERON_NUMA)
    {
        strcat(identifier, ".numa");
    }

    if (THERON_XS)
    {
        strcat(identifier, ".xs");
    }

    if (THERON_ENABLE_DEFAULTALLOCATOR_CHECKS)
    {
        strcat(identifier, ".alloc");
    }

    if (THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS)
    {
        strcat(identifier, ".register");
    }

    if (THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS)
    {
        strcat(identifier, ".unhandled");
    }

    if (THERON_ENABLE_BUILD_CHECKS)
    {
        strcat(identifier, ".build");
    }
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_DEBUG_BUILDDESCRIPTOR_H
