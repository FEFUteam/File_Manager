# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\File_manager_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\File_manager_autogen.dir\\ParseCache.txt"
  "File_manager_autogen"
  )
endif()
