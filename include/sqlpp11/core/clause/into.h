#pragma once

/*
 * Copyright (c) 2013-2016, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sqlpp11/core/detail/type_set.h>
#include <sqlpp11/core/no_data.h>
#include <sqlpp11/core/static_assert.h>
#include <sqlpp11/core/database/prepared_insert.h>
#include <sqlpp11/core/query/statement_fwd.h>
#include <sqlpp11/core/concepts.h>
#include <sqlpp11/core/type_traits.h>

namespace sqlpp
{
  // A SINGLE TABLE DATA
  template <typename _Table>
  struct into_t
  {
    into_t(_Table table) : _table(table)
    {
    }

    into_t(const into_t&) = default;
    into_t(into_t&&) = default;
    into_t& operator=(const into_t&) = default;
    into_t& operator=(into_t&&) = default;
    ~into_t() = default;

    _Table _table;
  };

  template <typename _Table>
  struct is_clause<into_t<_Table>> : public std::true_type
  {
  };

  template <typename Statement, typename _Table>
  struct consistency_check<Statement, into_t<_Table>>
  {
    using type = consistent_t;
  };

  template<typename _Table>
    struct nodes_of<into_t<_Table>>
    {
      using type = detail::type_vector<_Table>;
    };

  template<typename _Table>
    struct required_insert_columns_of<into_t<_Table>>: public required_insert_columns_of<_Table>
    {
    };

  template<typename _Table>
  struct provided_tables_of<into_t<_Table>> : public provided_tables_of<_Table>
  {
  };

  SQLPP_WRAPPED_STATIC_ASSERT(assert_into_t, "into() required");

  // NO INTO YET
  struct no_into_t
  {
  };

  template <typename Statement>
  struct clause_base<no_into_t, Statement> : public clause_data<no_into_t, Statement>
  {
    using clause_data<no_into_t, Statement>::clause_data;


    template <StaticRawTable _Table>
    auto into(_Table table) const
    {
      return new_statement(*this, into_t<_Table>{table});
    }
  };

  template <typename Statement>
  struct consistency_check<Statement, no_into_t>
  {
    using type = assert_into_t;
  };

  // Interpreters
  template <typename Context>
  auto to_sql_string(Context& , const no_into_t&) -> std::string
  {
    return "";
  }

  template <typename Context, typename _Table>
  auto to_sql_string(Context& context, const into_t<_Table>& t) -> std::string
  {
    return " INTO " + to_sql_string(context, t._table);
  }

  template <StaticRawTable T>
  auto into(T t)
  {
    return statement_t<no_into_t>().into(std::move(t));
  }
}  // namespace sqlpp
