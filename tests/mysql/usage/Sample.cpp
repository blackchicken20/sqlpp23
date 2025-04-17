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
#include <vector>

#include <sqlpp23/mysql/mysql.h>
#include <sqlpp23/sqlpp23.h>
#include <sqlpp23/tests/core/result_helpers.h>
#include <sqlpp23/tests/mysql/make_test_connection.h>
#include <sqlpp23/tests/mysql/tables.h>

namespace sql = sqlpp::mysql;
int Sample(int, char*[]) {
  sql::global_library_init();
  try {
    auto db = sql::make_test_connection();

    test::createTabBar(db);
    test::createTabFoo(db);

    assert(not db(select(sqlpp::value(false).as(sqlpp::alias::a))).front().a);

    const auto tab = test::TabBar{};
    // clear the table
    db(delete_from(tab));

    // Several ways of ensuring that tab is empty
    assert(
        not db(select(exists(select(tab.intN).from(tab)).as(::sqlpp::alias::a)))
                .front()
                .a);  // this is probably the fastest
    assert(not db(select(count(tab.intN).as(sqlpp::alias::a)).from(tab))
                   .front()
                   .a);
    assert(db(select(tab.intN).from(tab)).empty());

    // explicit all_of(tab)
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
    select(all_of(tab)).from(tab);
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
    db(select(all_of(tab)).from(tab));
    std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
    for (const auto& row : db(select(all_of(tab)).from(tab))) {
      std::cerr << __FILE__ << ": " << __LINE__ << std::endl;
      std::cerr << "row.intN: " << row.intN << ", row.textN: " << row.textN
                << ", row.boolNn: " << row.boolNn << std::endl;
    };
    // insert
    db(insert_into(tab).default_values());
    const auto x = select(all_of(tab)).from(tab);
    const auto y = db.prepare(x);
    for (const auto& row : db(db.prepare(select(all_of(tab)).from(tab)))) {
      std::cerr << "intN: " << row.intN << std::endl;
      std::cerr << "textN: " << row.textN << std::endl;
      std::cerr << "boolNn: " << row.boolNn << std::endl;
    }
    db(insert_into(tab).set(tab.textN = "kaesekuchen", tab.boolNn = true));
    db(insert_into(tab).set(tab.textN = "kaesekuchen",
                            dynamic(true, tab.boolNn = true)));
    db(insert_into(tab).default_values());
    db(insert_into(tab).set(tab.textN = "", tab.boolNn = true));

    // update
    db(update(tab)
           .set(tab.boolNn = false)
           .where(tab.intN.in(std::vector<int>{1, 2, 3, 4})));
    db(update(tab).set(tab.boolNn = true).where(tab.intN.in(1)));

    // remove
    {
      db(delete_from(tab).where(tab.intN == tab.intN + 3));

      std::cerr << "+++++++++++++++++++++++++++" << std::endl;
      for (const auto& row : db(select(all_of(tab)).from(tab))) {
        std::cerr << __LINE__ << " row.textN: " << row.textN << std::endl;
      }
      std::cerr << "+++++++++++++++++++++++++++" << std::endl;
      decltype(db(select(all_of(tab)).from(tab))) result;
      result = db(select(all_of(tab)).from(tab));
      std::cerr << "Accessing a field directly from the result (using the "
                   "current row): "
                << result.begin()->intN << std::endl;
      std::cerr << "Can do that again, no problem: " << result.begin()->intN
                << std::endl;
    }

    // transaction
    {
      auto tx = start_transaction(db);
      auto result =
          db(select(all_of(tab),
                    value(select(max(tab.intN).as(sqlpp::alias::a)).from(tab))
                        .as(sqlpp::alias::a))
                 .from(tab));
      if (const auto& row = *result.begin()) {
        const int64_t a = row.intN.value_or(0);
        const std::optional<long long> m = row.a;
        std::cerr << __LINE__ << " row.intN: " << a << ", row.max: " << m
                  << std::endl;
      }
      tx.commit();
    }

    test::TabFoo foo;
    for (const auto& row :
         db(select(tab.intN).from(tab.join(foo).on(tab.intN == foo.intN)))) {
      std::cerr << row.intN << std::endl;
    }

    for (const auto& row : db(select(tab.intN).from(
             tab.left_outer_join(foo).on(tab.intN == foo.intN)))) {
      std::cerr << row.intN << std::endl;
    }

    auto ps = db.prepare(select(all_of(tab))
                             .from(tab)
                             .where(tab.intN != parameter(tab.intN) and
                                    tab.textN != parameter(tab.textN) and
                                    tab.boolNn != parameter(tab.boolNn)));
    ps.params.intN = 7;
    ps.params.textN = "wurzelbrunft";
    ps.params.boolNn = true;
    for (const auto& row : db(ps)) {
      std::cerr << "bound result: intN: " << row.intN << std::endl;
      std::cerr << "bound result: textN: " << row.textN << std::endl;
      std::cerr << "bound result: boolNn: " << row.boolNn << std::endl;
    }

    std::cerr << "--------" << std::endl;
    ps.params.boolNn = false;
    for (const auto& row : db(ps)) {
      std::cerr << "bound result: intN: " << row.intN << std::endl;
      std::cerr << "bound result: textN: " << row.textN << std::endl;
      std::cerr << "bound result: boolNn: " << row.boolNn << std::endl;
    }

    std::cerr << "--------" << std::endl;
    ps.params.textN = "kaesekuchen";
    for (const auto& row : db(ps)) {
      std::cerr << "bound result: intN: " << row.intN << std::endl;
      std::cerr << "bound result: textN: " << row.textN << std::endl;
      std::cerr << "bound result: boolNn: " << row.boolNn << std::endl;
    }

    auto pi = db.prepare(insert_into(tab).set(tab.textN = parameter(tab.textN),
                                              tab.boolNn = true));
    pi.params.textN = "prepared cake";
    std::cerr << "Inserted: " << db(pi) << std::endl;

    auto pu = db.prepare(update(tab)
                             .set(tab.boolNn = parameter(tab.boolNn))
                             .where(tab.textN == "prepared cake"));
    pu.params.boolNn = false;
    std::cerr << "Updated: " << db(pu) << std::endl;

    auto pr =
        db.prepare(delete_from(tab).where(tab.textN != parameter(tab.textN)));
    pr.params.textN = "prepared cake";
    std::cerr << "Deleted lines: " << db(pr) << std::endl;

    for (const auto& row :
         db(select(case_when(tab.boolNn).then(tab.intN).else_(foo.intN).as(
                       tab.intN))
                .from(tab.cross_join(foo)))) {
      std::cerr << row.intN << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
