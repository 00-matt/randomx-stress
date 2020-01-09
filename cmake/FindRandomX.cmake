# This file is a part of randomx-stress.

#[=======================================================================[.rst:
FindRandomX
-------

Finds the RandomX library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``RandomX::RandomX``
  The RandomX library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``RandomX_FOUND``
  True if the system has the RandomX library.
``RandomX_INCLUDE_DIRS``
  Include directories needed to use RandomX.
``RandomX_LIBRARIES``
  Libraries needed to link to RandomX.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``RandomX_INCLUDE_DIR``
  The directory containing ``randomx.h``.
``RandomX_LIBRARY``
  The path to the RandomX library.

#]=======================================================================]

find_path(RandomX_INCLUDE_DIR NAMES randomx.h)
find_library(RandomX_LIBRARY NAMES randomx)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RandomX
  FOUND_VAR RandomX_FOUND
  REQUIRED_VARS
    RandomX_LIBRARY
    RandomX_INCLUDE_DIR
)

if(RandomX_FOUND)
  set(RandomX_LIBRARIES ${RandomX_LIBRARY})
  set(RandomX_INCLUDE_DIRS ${RandomX_INCLUDE_DIR})
  if(NOT TARGET RandomX::RandomX)
    add_library(RandomX::RandomX UNKNOWN IMPORTED)
    set_target_properties(RandomX::RandomX PROPERTIES
      IMPORTED_LOCATION "${RandomX_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${RandomX_INCLUDE_DIR}"
    )
  endif()
endif()

mark_as_advanced(RandomX_INCLUDE_DIR RandomX_LIBRARY)
