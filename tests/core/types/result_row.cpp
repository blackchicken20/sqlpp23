/*
 * Copyright (c) 2016-2016, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MockDb.h"
#include "Sample.h"
#include <sqlpp11/sqlpp11.h>

#include "../../include/test_helpers.h"

namespace
{
  constexpr auto bar = test::TabBar{};
  constexpr auto foo = test::TabFoo{};

  static_assert(not sqlpp::can_be_null_t<decltype(bar.id)>::value, "");
  static_assert(sqlpp::can_be_null_t<decltype(foo.doubleN)>::value, "");
  static_assert(not sqlpp::can_be_null_t<decltype(foo.textNnD)>::value, "");
  static_assert(not sqlpp::can_be_null_t<decltype(bar.boolNn)>::value, "");
  const auto seven = sqlpp::value(7).as(sqlpp::alias::s);
  static_assert(not sqlpp::can_be_null_t<decltype(seven)>::value, "");

  auto db = MockDb{};

  void single_table()
  {
    {
      // result fields are as nullable as the expressions they represent
      const auto rows = db(select(bar.id, bar.boolNn, seven).from(bar).unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "");
      static_assert(not is_optional<decltype(x.s)>::value, "");
    }
  }

  void optional_columns()
  {
    {
      // result fields are as nullable as the expressions they represent
#warning: add `if_` to other expressions, too
#warning: test with nullable columns, too.
#warning: test with all kinds of functions as well.
#warning: We should actually test for the exact type!
      const auto rows = db(select(bar.id.if_(true), bar.boolNn.if_(true)/*, seven*/).from(bar).unconditionally());
      auto& x = rows.front();
      static_assert(is_optional<decltype(x.id)>::value, "");
      static_assert(is_optional<decltype(x.boolNn)>::value, "");
    }
  }

  void join()
  {
    // Join
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(foo.join(bar).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "nullable value can always be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value, "left side of (inner) join cannot be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "right side of (inner) join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
    {
	  const auto& rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(bar.join(foo).on(foo.doubleN > bar.id))
                             .unconditionally());
          auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "nullable value can always be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "left side of (inner) join cannot be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value, "right side of (inner) join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
    {
      MockSizeDb db2;
      auto&& result = db2(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(bar.join(foo).on(foo.doubleN > bar.id))
                             .unconditionally());
      result.size();
      static_assert(std::is_same<size_t, decltype(result.size())>::value, "MockSizeDb size() isn't size_t");
    }

    // Inner join
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(foo.inner_join(bar).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "nullable value can always be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value, "left side of inner join cannot be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "right side of inner join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(bar.inner_join(foo).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "primary key cannot be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "left side of inner join cannot be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value, "right side of inner join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }

    // Left outer join
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(foo.left_outer_join(bar).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(is_optional<decltype(x.id)>::value, "outer primary key can be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value, "left side of left outer join cannot be null");
      static_assert(is_optional<decltype(x.boolNn)>::value, "right side of left outer join can be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(bar.left_outer_join(foo).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "primary key cannot be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "left side of left outer join cannot be null");
      static_assert(is_optional<decltype(x.textNnD)>::value, "right side of left outer join can be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }

    // Right outer join
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(foo.right_outer_join(bar).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "primary key cannot be null");
      static_assert(is_optional<decltype(x.textNnD)>::value, "left side of right outer join can be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value,
                    "right side of right outer join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(bar.right_outer_join(foo).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(is_optional<decltype(x.id)>::value, "left side of right outer join can be null");
      static_assert(is_optional<decltype(x.boolNn)>::value, "left side of right outer join can be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value,
                    "right side of right outer join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }

    // Outer join
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(foo.outer_join(bar).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(is_optional<decltype(x.id)>::value, "left side of outer join can be null");
      static_assert(is_optional<decltype(x.textNnD)>::value, "left side of outer join can be null");
      static_assert(is_optional<decltype(x.boolNn)>::value, "right side of outer join can be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
    {
      const auto rows = db(select(bar.id, foo.textNnD, bar.boolNn, seven)
                             .from(bar.outer_join(foo).on(foo.doubleN > bar.id))
                             .unconditionally());
      auto& x = rows.front();
      static_assert(is_optional<decltype(x.id)>::value, "left side of outer join can be null");
      static_assert(is_optional<decltype(x.boolNn)>::value, "left side of outer join can be null");
      static_assert(is_optional<decltype(x.textNnD)>::value, "right side of outer join can be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }

    // Cross join
    {
      const auto rows =
          db(select(bar.id, foo.textNnD, bar.boolNn, seven).from(foo.cross_join(bar)).unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "primary key cannot be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value, "left side of cross join cannot be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "right side of cross join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
    {
      const auto rows =
          db(select(bar.id, foo.textNnD, bar.boolNn, seven).from(bar.cross_join(foo)).unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.id)>::value, "primary key cannot be null");
      static_assert(not is_optional<decltype(x.boolNn)>::value, "left side of cross join cannot be null");
      static_assert(not is_optional<decltype(x.textNnD)>::value, "right side of cross join cannot be null");
      static_assert(not is_optional<decltype(x.s)>::value, "constant non-null value can not be null");
    }
  }

  void aggregates()
  {
    {
      // aggregates of nullable values
#warning use a nullable value
      const auto a = bar.id;
      static_assert(not sqlpp::can_be_null_t<decltype(bar.id)>::value, "");
      static_assert(not sqlpp::can_be_null_t<decltype(a)>::value, "");
      const auto rows = db(select(count(a), avg(a), max(a), min(a), sum(a)).from(bar).unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.count)>::value, "");
      static_assert(is_optional<decltype(x.avg)>::value, "");
      static_assert(is_optional<decltype(x.sum)>::value, "");
      static_assert(is_optional<decltype(x.max)>::value, "");
      static_assert(is_optional<decltype(x.min)>::value, "");
    }
    {
      // aggregates of nullable values
      const auto o = foo.doubleN;
      static_assert(sqlpp::can_be_null_t<decltype(foo.doubleN)>::value, "");
      static_assert(sqlpp::can_be_null_t<decltype(o)>::value, "");
      const auto rows = db(select(count(o), avg(o), max(o), min(o), sum(o)).from(foo).unconditionally());
      auto& x = rows.front();
      static_assert(not is_optional<decltype(x.count)>::value, "");
      static_assert(is_optional<decltype(x.avg)>::value, "");
      static_assert(is_optional<decltype(x.sum)>::value, "");
      static_assert(is_optional<decltype(x.max)>::value, "");
      static_assert(is_optional<decltype(x.min)>::value, "");
    }
  }
}

int main(int, char* [])
{
  single_table();
  optional_columns();
  join();
  aggregates();
}
