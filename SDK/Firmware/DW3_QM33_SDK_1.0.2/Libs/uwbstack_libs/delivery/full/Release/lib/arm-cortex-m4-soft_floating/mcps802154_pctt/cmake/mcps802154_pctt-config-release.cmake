#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mcps802154_pctt" for configuration "Release"
set_property(TARGET mcps802154_pctt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mcps802154_pctt PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/arm-cortex-m4-soft_floating/mcps802154_pctt/libmcps802154_pctt_full_arm-cortex-m4-soft_floating_rtos_R12.7.0-00288-gb8203d558863.a"
  )

list(APPEND _cmake_import_check_targets mcps802154_pctt )
list(APPEND _cmake_import_check_files_for_mcps802154_pctt "${_IMPORT_PREFIX}/lib/arm-cortex-m4-soft_floating/mcps802154_pctt/libmcps802154_pctt_full_arm-cortex-m4-soft_floating_rtos_R12.7.0-00288-gb8203d558863.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
