INSTALL(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DESTINATION ${DOC_DIR}
  PATTERN CMakeLists.txt EXCLUDE
  PATTERN Makefile.am EXCLUDE
  PATTERN doxygen/* EXCLUDE
)
