# Copyright (c) 2021-2021, Roland Bock
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this
#    list of conditions and the following disclaimer in the documentation and/or
#    other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
add_subdirectory(core/)

add_library(sqlpp23_testing INTERFACE)
target_include_directories(sqlpp23_testing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (NOT MSVC)
    target_compile_options(sqlpp23_testing INTERFACE -Wall -Wextra -pedantic)
endif ()

if(BUILD_MYSQL_CONNECTOR)
    add_subdirectory(mysql)
endif()

if(BUILD_POSTGRESQL_CONNECTOR)
    add_subdirectory(postgresql)
endif()

if(BUILD_SQLITE3_CONNECTOR OR BUILD_SQLCIPHER_CONNECTOR)
    add_subdirectory(sqlite3)
endif()

add_subdirectory(scripts)
