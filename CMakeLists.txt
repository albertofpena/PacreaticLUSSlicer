cmake_minimum_required(VERSION 2.8.9)

project(PancreaticLUS)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "http://www.example.com/Slicer/Extensions/PancreaticLUS")
set(EXTENSION_CATEGORY "NAVISurg")
set(EXTENSION_CONTRIBUTORS "Alberto Fernandez Pena")
set(EXTENSION_DESCRIPTION "This is an example of a simple extension")
set(EXTENSION_ICONURL "http://www.example.com/Slicer/Extensions/PancreaticLUS.png")
set(EXTENSION_SCREENSHOTURLS "http://www.example.com/Slicer/Extensions/PancreaticLUS/Screenshots/1.png")

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(PancreaticLUS)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
