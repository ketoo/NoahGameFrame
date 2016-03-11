// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Assert.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Debug/BuildDescriptor.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4996)  // function or variable may be unsafe
#pragma warning (disable:4127)  // conditional expression is constant.
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


// This function must not be inlined because it should be generated in the Theron library.
// NOTE: This must match the similar code in GenerateInline!
THERON_NOINLINE void BuildDescriptor::GenerateInLibrary(char *const identifier)
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
