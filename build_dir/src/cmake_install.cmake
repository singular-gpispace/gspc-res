# Install script for directory: /scratch/gnawali/Try/gspc-res/template/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/scratch/gnawali/Try/gspc-res/install_dir")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so"
         RPATH "/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/libexec/iml:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/lib:/usr/lib/gcc/x86_64-linux-gnu/12:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/scratch/gnawali/Try/gspc-res/build_dir/src/libSINGULAR-template-installation.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so"
         OLD_RPATH "/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/libexec/iml:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/lib:/usr/lib/gcc/x86_64-linux-gnu/12:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib:"
         NEW_RPATH "/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/libexec/iml:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/lib:/usr/lib/gcc/x86_64-linux-gnu/12:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSINGULAR-template-installation.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so"
         RPATH "\$ORIGIN/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/libexec/iml:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-f4kucmkjbh6zho27okeaxhwhwcickrfx/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gmp-6.3.0-n5466d2epee7y3eyr3drmdwpzotucz4b/lib:/usr/lib/gcc/x86_64-linux-gnu/12:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/scratch/gnawali/Try/gspc-res/install_dir" TYPE SHARED_LIBRARY FILES "/scratch/gnawali/Try/gspc-res/build_dir/src/libSINGULAR-template-module.so")
  if(EXISTS "$ENV{DESTDIR}/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so"
         OLD_RPATH "/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/libexec/iml:/scratch/gnawali/Try/gspc-res/build_dir/src:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-f4kucmkjbh6zho27okeaxhwhwcickrfx/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gmp-6.3.0-n5466d2epee7y3eyr3drmdwpzotucz4b/lib:/usr/lib/gcc/x86_64-linux-gnu/12:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib:"
         NEW_RPATH "\$ORIGIN/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/libexec/iml:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/boost-1.63.0-suqj3khkv55qa6efb36qxknh7gadjn2x/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gpi-space-24.12-n522f32zjepvd2hhefibm4liumbmbuyu/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/singular-4.4.0p2-f4kucmkjbh6zho27okeaxhwhwcickrfx/lib:/scratch/gnawali/singular-gpispace/spack/opt/spack/linux-debian12-icelake/gcc-12.2.0/gmp-6.3.0-n5466d2epee7y3eyr3drmdwpzotucz4b/lib:/usr/lib/gcc/x86_64-linux-gnu/12:/usr/lib/x86_64-linux-gnu:/usr/lib:/lib/x86_64-linux-gnu:/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/scratch/gnawali/Try/gspc-res/install_dir/libSINGULAR-template-module.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

