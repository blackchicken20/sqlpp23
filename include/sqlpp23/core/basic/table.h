#pragma once

/*
 * Copyright (c) 2013-2015, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
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

#include <sqlpp23/core/basic/all_of.h>
#include <sqlpp23/core/basic/column.h>
#include <sqlpp23/core/basic/enable_join.h>
#include <sqlpp23/core/basic/join.h>
#include <sqlpp23/core/basic/table_as.h>
#include <sqlpp23/core/detail/type_set.h>
#include <sqlpp23/core/type_traits.h>

namespace sqlpp {
template <typename TableSpec>
struct table_t : public TableSpec::template _table_columns<table_t<TableSpec>>,
                 public enable_join<table_t<TableSpec>> {
  template <typename NameTagProvider>
  constexpr auto as(const NameTagProvider& /*unused*/) const
      -> table_as_t<TableSpec, name_tag_of_t<NameTagProvider>> {
    return {};
  }
};

template <typename TableSpec>
struct is_raw_table<table_t<TableSpec>> : public std::true_type {};

template <typename TableSpec>
struct is_table<table_t<TableSpec>> : public std::true_type {};

template <typename TableSpec>
struct name_tag_of<table_t<TableSpec>> : public name_tag_of<TableSpec> {};

template <typename TableSpec>
struct provided_tables_of<table_t<TableSpec>> {
  using type = sqlpp::detail::type_set<table_t<TableSpec>>;
};

template <typename TableSpec>
struct required_insert_columns_of<table_t<TableSpec>> {
  using type = typename TableSpec::_required_insert_columns;
};

template <typename Context, typename TableSpec>
auto to_sql_string(Context& context, const table_t<TableSpec>& /*unused*/)
    -> std::string {
  return name_to_sql_string(context, name_tag_of_t<TableSpec>{});
}
}  // namespace sqlpp
