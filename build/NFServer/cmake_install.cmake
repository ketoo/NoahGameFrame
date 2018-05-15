# Install script for directory: /mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/NFServer

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFDataAgent_NosqlPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFGameLogicPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFGameServerNet_ClientPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFGameServerNet_ServerPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFGameServerPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFLoginLogicPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFLoginNet_ClientPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFLoginNet_ServerPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFMasterNet_ServerPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFMasterNet_HttpServerPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFMasterServerPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFProxyServerNet_ClientPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFProxyServerNet_ServerPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFProxyLogicPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFWorldLogicPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFWorldNet_ClientPlugin/cmake_install.cmake")
  INCLUDE("/mnt/hgfs/OSP/NoahGameFrame-Develop/NoahGameFrame/build/NFServer/NFWorldNet_ServerPlugin/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

