/*
 * Copyright (c) 2013 - 2016, Roland Bock
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

#include <cassert>
#include <iostream>

#include <sqlpp23/mysql/database/connection.h>
#include <sqlpp23/sqlpp23.h>
#include <sqlpp23/tests/mysql/make_test_connection.h>
#include <sqlpp23/tests/mysql/tables.h>

namespace {
const auto library_raii =
    sqlpp::mysql::scoped_library_initializer_t{0, nullptr, nullptr};

namespace sql = sqlpp::mysql;
const auto tab = test::TabBar{};

SQLPP_CREATE_NAME_TAG(something);
}  // namespace

void testPreparedStatementResult(sql::connection& db) {
  auto preparedInsert =
      db.prepare(insert_into(tab).set(tab.textN = parameter(tab.textN)));
  preparedInsert.params.textN = std::nullopt;
  db(preparedInsert);
  preparedInsert.params.textN = "17";
  db(preparedInsert);
  preparedInsert.params.textN = std::nullopt;
  db(preparedInsert);
  preparedInsert.params.textN = "17";
  db(preparedInsert);

  auto preparedSelectAll =
      db.prepare(sqlpp::select(count(tab.intN).as(something)).from(tab));
  auto preparedUpdateAll =
      db.prepare(sqlpp::update(tab).set(tab.boolNn = false));

  {
    // explicit result scope
    // if results are released update should execute without exception
    auto result = db(preparedSelectAll);
    std::ignore = result.front().something;
  }

  db(preparedUpdateAll);
}

int Prepared(int, char*[]) {
  sql::global_library_init();
  try {
    auto db = sql::make_test_connection();
    test::createTabBar(db);

    testPreparedStatementResult(db);
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
