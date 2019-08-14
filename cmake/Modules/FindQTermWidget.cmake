# Look for the header file.
find_path(QTermWidget_INCLUDE NAMES qtermwidget5/qtermwidget.h
  PATHS $ENV{QTERMWIDGET_ROOT}/include /opt/local/include /usr/local/include /usr/include
  DOC "Path in which the file qtermwidget5/qtermwidget.h is located." )

# Look for the library.
find_library(QTermWidget_LIBRARY NAMES qtermwidget5
  PATHS /usr/lib $ENV{QTERMWIDGET_ROOT}/lib
  DOC "Path to qtermwidget5 library." )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QTermWidget DEFAULT_MSG QTermWidget_INCLUDE QTermWidget_LIBRARY)

if(QTERMWIDGET_FOUND)
  message(STATUS "Found QTermWidget (include: ${QTermWidget_INCLUDE}, library: ${QTermWidget_LIBRARY})")
  set(QTermWidget_INCLUDES ${QTermWidget_INCLUDE})
  set(QTermWidget_LIBRARIES ${QTermWidget_LIBRARY})
  mark_as_advanced(QTermWidget_INCLUDE QTermWidget_LIBRARY)
Endif()
