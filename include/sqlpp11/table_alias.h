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

#include <sqlpp11/enable_join.h>
#include <sqlpp11/table_columns.h>
#include <sqlpp11/detail/type_set.h>
#include <sqlpp11/serialize.h>
#include <sqlpp11/join.h>
#include <sqlpp11/type_traits.h>

namespace sqlpp
{
  template <typename AliasProvider, typename TableSpec>
  struct table_alias_t : public TableSpec::_table_columns<AliasProvider>,
                         public enable_join<table_alias_t<AliasProvider, TableSpec>>
  {
    using _nodes = detail::type_vector<>;
    using _required_ctes = required_ctes_of<TableSpec>;
    using _provided_tables = detail::type_set<AliasProvider>;

    static_assert(required_tables_of_t<TableSpec>::size::value == 0, "table aliases must not depend on external tables");

#warning: need to inherit?
    //using _column_tuple_t = std::tuple<column_t<AliasProvider, ColumnSpec>...>;
  };

  template<typename AliasProvider, typename TableSpec>
    struct is_table<table_alias_t<AliasProvider, TableSpec>> : public std::true_type{};

  template<typename AliasProvider, typename TableSpec>
    struct name_tag_of<table_alias_t<AliasProvider, TableSpec>> : public name_tag_of<AliasProvider>{};

  template <typename Context, typename AliasProvider, typename TableSpec>
  Context& serialize(Context& context, const table_alias_t<AliasProvider, TableSpec>&)
  {
    context << name_tag_of_t<TableSpec>::_name_t::template char_ptr<Context>();
    context << " AS " << name_tag_of_t<AliasProvider>::_name_t::template char_ptr<Context>();
    return context;
  }
}  // namespace sqlpp
