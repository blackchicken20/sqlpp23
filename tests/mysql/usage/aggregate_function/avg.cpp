/*
 * Copyright (c) 2025, Roland Bock
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

#ifdef _MSC_VER
#define NOMINMAX
#endif

#include "sqlpp23/core/aggregate_function/avg.h"
#include <cassert>

#include <sqlpp23/mysql/mysql.h>
#include <sqlpp23/sqlpp23.h>
#include <sqlpp23/tests/mysql/make_test_connection.h>
#include <sqlpp23/tests/mysql/tables.h>

auto require_close(int line, double l, double r) -> void
{
  if (std::abs(l - r) > 0.001) {
    std::cerr << line << ": abs(";
    std::cerr << sqlpp::to_sql_string(std::cerr, l);
    std::cerr << " - ";
    std::cerr << sqlpp::to_sql_string(std::cerr, r);
    std::cerr << ") > 0.001\n";
    throw std::runtime_error("Unexpected result");
  }
}

namespace sql = sqlpp::mysql;
int main(int, char*[]) {
  sql::global_library_init();
  try {
    const auto tab = test::TabFoo{};
    auto db = sql::make_test_connection();

    test::createTabFoo(db);

    // clear the table
    db(truncate(tab));

    // insert
    db(insert_into(tab).set(tab.intN = 7));
    db(insert_into(tab).set(tab.intN = 7));
    db(insert_into(tab).set(tab.intN = 9));

    // select avg
    for (const auto& row : db(select(
            avg(tab.intN).as(sqlpp::alias::avg_),
            avg(sqlpp::distinct, tab.intN).as(sqlpp::alias::distinct_avg_)
            ).from(tab))) {
      assert(row.avg_.has_value());
      assert(row.distinct_avg_.has_value());
      require_close(__LINE__, row.avg_.value(), 7.666666);
      require_close(__LINE__, row.distinct_avg_.value(), 8);
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
