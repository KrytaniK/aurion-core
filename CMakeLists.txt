cmake_minimum_required(VERSION 4.2)

project(aurion-core VERSION 1.1.1)

add_subdirectory(AurionCore)

set(CPACK_PACKAGE_NAME "AurionCore")
set(CPACK_GENERATOR "ZIP")

include(CPack)