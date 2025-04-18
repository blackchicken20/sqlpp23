/*
 * Copyright (c) 2021 - 2021, Roland Bock, ZerQAQ
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>

#include <sqlpp23/mysql/mysql.h>
#include <sqlpp23/sqlpp23.h>
#include <sqlpp23/tests/mysql/make_test_connection.h>
#include <sqlpp23/tests/mysql/tables.h>

const auto tab = test::TabFoo{};

namespace sql = sqlpp::mysql;

int DeleteFrom(int, char*[]) {
  sql::global_library_init();
  try {
    auto db = sql::make_test_connection();
    test::createTabFoo(db);

    db(insert_into(tab).set(tab.textNnD = "1", tab.boolN = false));
    db(insert_into(tab).set(tab.textNnD = "2", tab.boolN = false));
    db(insert_into(tab).set(tab.textNnD = "3", tab.boolN = false));

    db(sql::delete_from(tab).order_by(tab.intN.desc()).limit(1u));
    for (const auto& row : db(sqlpp::select(tab.textNnD)
                                  .from(tab)
                                  .order_by(tab.intN.desc())
                                  .limit(1u))) {
      if (row.textNnD != "2")
        throw std::runtime_error("unexpected value for row.textNnD: " +
                                 std::string(row.textNnD));
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
