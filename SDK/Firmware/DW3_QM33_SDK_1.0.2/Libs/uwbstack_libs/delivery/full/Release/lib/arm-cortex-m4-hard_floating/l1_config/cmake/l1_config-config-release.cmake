#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "l1_config" for configuration "Release"
set_property(TARGET l1_config APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(l1_config PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/arm-cortex-m4-hard_floating/l1_config/libl1_config_full_arm-cortex-m4-hard_floating_rtos_R12.7.0-00288-gb8203d558863.a"
  )

list(APPEND _cmake_import_check_targets l1_config )
list(APPEND _cmake_import_check_files_for_l1_config "${_IMPORT_PREFIX}/lib/arm-cortex-m4-hard_floating/l1_config/libl1_config_full_arm-cortex-m4-hard_floating_rtos_R12.7.0-00288-gb8203d558863.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
