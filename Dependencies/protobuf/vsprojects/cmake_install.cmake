# Install script for directory: D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/cmake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/protobuf")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "libprotobuf-lite" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Debug/libprotobuf-lited.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Release/libprotobuf-lite.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/MinSizeRel/libprotobuf-lite.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/RelWithDebInfo/libprotobuf-lite.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "libprotobuf" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Debug/libprotobufd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Release/libprotobuf.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/MinSizeRel/libprotobuf.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/RelWithDebInfo/libprotobuf.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "libprotoc" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Debug/libprotocd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Release/libprotoc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/MinSizeRel/libprotoc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/RelWithDebInfo/libprotoc.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protoc" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Debug/protoc.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/Release/protoc.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/MinSizeRel/protoc.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/RelWithDebInfo/protoc.exe")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES
    "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/protobuf.pc"
    "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/protobuf-lite.pc"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "any.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/any.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "any.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/any.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "api.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/api.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "arena.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/arena.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "arena_impl.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/arena_impl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "arenastring.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/arenastring.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "code_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/code_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "command_line_interface.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/command_line_interface.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/cpp" TYPE FILE RENAME "cpp_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/cpp/cpp_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/csharp" TYPE FILE RENAME "csharp_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/csharp/csharp_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/csharp" TYPE FILE RENAME "csharp_names.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/csharp/csharp_names.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "importer.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/importer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/java" TYPE FILE RENAME "java_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/java/java_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/java" TYPE FILE RENAME "java_names.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/java/java_names.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/js" TYPE FILE RENAME "js_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/js/js_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/js" TYPE FILE RENAME "well_known_types_embed.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/js/well_known_types_embed.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/objectivec" TYPE FILE RENAME "objectivec_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/objectivec/objectivec_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/objectivec" TYPE FILE RENAME "objectivec_helpers.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/objectivec/objectivec_helpers.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "parser.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/parser.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/php" TYPE FILE RENAME "php_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/php/php_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "plugin.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/plugin.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "plugin.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/plugin.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/python" TYPE FILE RENAME "python_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/python/python_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/ruby" TYPE FILE RENAME "ruby_generator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/ruby/ruby_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/descriptor.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/descriptor.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor_database.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/descriptor_database.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "duration.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/duration.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "dynamic_message.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/dynamic_message.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "empty.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/empty.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "extension_set.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/extension_set.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "field_mask.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/field_mask.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_enum_reflection.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/generated_enum_reflection.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_enum_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/generated_enum_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_message_reflection.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/generated_message_reflection.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_message_table_driven.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/generated_message_table_driven.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_message_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/generated_message_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "has_bits.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/has_bits.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "implicit_weak_message.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/implicit_weak_message.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "inlined_string_field.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/inlined_string_field.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "coded_stream.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/coded_stream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "gzip_stream.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/gzip_stream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "printer.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/printer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "strtod.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/strtod.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "tokenizer.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/tokenizer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "zero_copy_stream.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/zero_copy_stream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "zero_copy_stream_impl.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/zero_copy_stream_impl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "zero_copy_stream_impl_lite.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/io/zero_copy_stream_impl_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/map.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_entry.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/map_entry.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_entry_lite.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/map_entry_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_field.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/map_field.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_field_inl.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/map_field_inl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_field_lite.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/map_field_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_type_handler.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/map_type_handler.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "message.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/message.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "message_lite.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/message_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "metadata.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/metadata.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "metadata_lite.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/metadata_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "reflection.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/reflection.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "reflection_ops.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/reflection_ops.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "repeated_field.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/repeated_field.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "service.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/service.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "source_context.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/source_context.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "struct.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/struct.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "bytestream.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/bytestream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "callback.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/callback.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "casts.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/casts.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "common.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/common.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "fastmem.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/fastmem.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "hash.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/hash.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "logging.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/logging.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "macros.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/macros.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "mutex.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/mutex.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "once.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/once.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "platform_macros.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/platform_macros.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "port.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/port.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "singleton.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/singleton.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "status.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/status.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "stl_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/stl_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "stringpiece.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/stringpiece.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "template_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/stubs/template_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "text_format.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/text_format.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "timestamp.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/timestamp.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "type.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/type.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "unknown_field_set.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/unknown_field_set.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "delimited_message_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/delimited_message_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "field_comparator.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/field_comparator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "field_mask_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/field_mask_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "json_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/json_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "message_differencer.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/message_differencer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "time_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/time_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "type_resolver.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/type_resolver.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "type_resolver_util.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/util/type_resolver_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wire_format.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/wire_format.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wire_format_lite.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/wire_format_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wire_format_lite_inl.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/wire_format_lite_inl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wrappers.pb.h" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/wrappers.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "any.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/any.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "api.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/api.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "plugin.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/plugin.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/descriptor.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "duration.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/duration.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "empty.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/empty.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "field_mask.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/field_mask.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "source_context.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/source_context.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "struct.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/struct.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "timestamp.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/timestamp.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "type.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/type.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wrappers.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/wrappers.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/descriptor.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "any.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/any.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "api.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/api.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "duration.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/duration.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "empty.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/empty.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "field_mask.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/field_mask.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "source_context.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/source_context.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "struct.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/struct.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "timestamp.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/timestamp.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "type.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/type.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wrappers.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/wrappers.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "plugin.proto" FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/src/google/protobuf/compiler/plugin.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-export" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/protobuf-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/protobuf-targets.cmake"
         "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/CMakeFiles/Export/cmake/protobuf-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/protobuf-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/protobuf-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/CMakeFiles/Export/cmake/protobuf-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/CMakeFiles/Export/cmake/protobuf-targets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/CMakeFiles/Export/cmake/protobuf-targets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/CMakeFiles/Export/cmake/protobuf-targets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/CMakeFiles/Export/cmake/protobuf-targets-release.cmake")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-export" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE DIRECTORY FILES "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/cmake/" REGEX "/protobuf\\-targets\\.cmake$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/Documents/Downloads/protobuf-cpp-3.6.1/protobuf-3.6.1/vsprojects/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
