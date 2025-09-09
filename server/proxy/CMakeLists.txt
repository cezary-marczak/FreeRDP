# FreeRDP: A Remote Desktop Protocol Implementation
# FreeRDP Proxy Server
#
# Copyright 2019 Mati Shabtay <matishabtay@gmail.com>
# Copyright 2019 Kobi Mizrachi <kmizrachi18@gmail.com>
# Copyright 2019 Idan Freiberg <speidy@gmail.com>

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(CMakeDependentOption)
include(FindPkgConfig)
set(MODULE_NAME "freerdp-proxy")
set(MODULE_PREFIX "FREERDP_SERVER_PROXY")

find_package(PkgConfig REQUIRED)
pkg_search_module(CJSON REQUIRED libcjson)

set(${MODULE_PREFIX}_SRCS
  pf_context.c
  pf_channels.c
  pf_channels.h
  pf_client.c
  pf_client.h
  pf_input.c
  pf_input.h
  pf_update.c
  pf_update.h
  pf_rail.c
  pf_rail.h
  pf_rdpgfx.c
  pf_rdpgfx.h
  pf_disp.c
  pf_disp.h
  pf_server.c
  pf_server.h
  pf_gdi.c
  pf_gdi.h
  pf_config.c
  pf_graphics.c
  pf_graphics.h
  pf_modules.c
  pf_modules.h
  pf_cliprdr.c
  pf_cliprdr.h
  pf_rdpsnd.c
  pf_rdpsnd.h
  pf_capture.c
  pf_capture.h
  pf_log.h
  )

# On windows create dll version information.
# Vendor, product and year are already set in top level CMakeLists.txt
if (WIN32)
  set (RC_VERSION_MAJOR ${FREERDP_VERSION_MAJOR})
  set (RC_VERSION_MINOR ${FREERDP_VERSION_MINOR})
  set (RC_VERSION_BUILD ${FREERDP_VERSION_REVISION})
  set (RC_VERSION_FILE "${MODULE_NAME}${CMAKE_EXECUTABLE_SUFFIX}" )

  configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/WindowsDLLVersion.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY)

  set ( ${MODULE_PREFIX}_SRCS ${${MODULE_PREFIX}_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif()

add_library(${MODULE_NAME} ${${MODULE_PREFIX}_SRCS})

target_include_directories(${MODULE_NAME} PRIVATE ${CJSON_INCLUDE_DIRS})
target_compile_options(${MODULE_NAME} PRIVATE ${CJSON_CFLAGS_OTHER})
#target_link_libraries(${MODULE_NAME} PRIVATE ${CJSON_LIBRARIES})
target_link_options(${MODULE_NAME} PRIVATE ${CJSON_LDFLAGS})

set_target_properties(${MODULE_NAME} PROPERTIES OUTPUT_NAME ${MODULE_NAME}${FREERDP_API_VERSION})

#set(${MODULE_PREFIX}_SRCS ${${MODULE_PREFIX}_SRCS} freerdp_proxy.c)
#add_executable(${MODULE_NAME} ${${MODULE_PREFIX}_SRCS})

set(${MODULE_PREFIX}_LIBS ${${MODULE_PREFIX}_LIBS} freerdp-server)
set(${MODULE_PREFIX}_LIBS ${${MODULE_PREFIX}_LIBS} freerdp-client)
set(${MODULE_PREFIX}_LIBS ${${MODULE_PREFIX}_LIBS} rdpgfx-client)
set(${MODULE_PREFIX}_LIBS ${${MODULE_PREFIX}_LIBS} winpr freerdp)

target_link_libraries(${MODULE_NAME} PRIVATE ${CJSON_LIBRARIES} ${${MODULE_PREFIX}_LIBS})

set_target_properties(${MODULE_NAME} PROPERTIES OUTPUT_NAME ${MODULE_NAME}${FREERDP_API_VERSION})
if (WITH_LIBRARY_VERSIONING)
  set_target_properties(${MODULE_NAME} PROPERTIES VERSION ${FREERDP_VERSION} SOVERSION ${FREERDP_API_VERSION})
endif()

install(TARGETS ${MODULE_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT server EXPORT FreeRDP-ProxyTargets)

if (WITH_DEBUG_SYMBOLS AND MSVC)
  get_target_property(OUTPUT_FILENAME ${MODULE_NAME} OUTPUT_NAME)
  install(FILES ${CMAKE_BINARY_DIR}/${OUTPUT_FILENAME}.pdb DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT symbols)
endif()

set_property(TARGET ${MODULE_NAME} PROPERTY FOLDER "Server/proxy")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/freerdp-proxy.pc.in ${CMAKE_CURRENT_BINARY_DIR}/freerdp-proxy${FREERDP_VERSION_MAJOR}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/freerdp-proxy${FREERDP_VERSION_MAJOR}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

CMAKE_DEPENDENT_OPTION(WITH_PROXY_MODULES "Compile proxy modules" OFF	"WITH_PROXY" OFF)
if (WITH_PROXY_MODULES)
  add_subdirectory("modules")
endif()
