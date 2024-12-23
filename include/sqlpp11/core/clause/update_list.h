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

#include <sqlpp11/core/query/dynamic.h>
#include <sqlpp11/core/detail/type_set.h>
#include <sqlpp11/core/tuple_to_sql_string.h>
#include <sqlpp11/core/type_traits.h>

namespace sqlpp
{
  template <typename... Assignments>
  struct update_list_t
  {
    update_list_t(std::tuple<Assignments...> assignments) : _assignments(assignments)
    {
    }

    update_list_t(const update_list_t&) = default;
    update_list_t(update_list_t&&) = default;
    update_list_t& operator=(const update_list_t&) = default;
    update_list_t& operator=(update_list_t&&) = default;
    ~update_list_t() = default;

    std::tuple<Assignments...> _assignments;
  };

  SQLPP_PORTABLE_STATIC_ASSERT(
      assert_no_unknown_tables_in_update_assignments_t,
      "at least one update assignment requires a table which is otherwise not known in the statement");

  template <typename... Assignments>
  struct is_clause<update_list_t<Assignments...>> : public std::true_type
  {
  };

  template <typename Statement, typename... Assignments>
  struct consistency_check<Statement, update_list_t<Assignments...>>
  {
    using type = typename std::conditional<Statement::template _no_unknown_tables<update_list_t<Assignments...>>,
                                                           consistent_t,
                                                           assert_no_unknown_tables_in_update_assignments_t>::type;
  };

  template<typename... Assignments>
    struct nodes_of<update_list_t<Assignments...>>
    {
      using type = detail::type_vector<Assignments...>;
    };

  SQLPP_PORTABLE_STATIC_ASSERT(assert_update_assignments_t, "update assignments required, i.e. set(...)");

  SQLPP_PORTABLE_STATIC_ASSERT(assert_update_set_assignments_t, "at least one argument is not an assignment in set()");
  SQLPP_PORTABLE_STATIC_ASSERT(assert_update_set_no_duplicates_t, "at least one duplicate column detected in set()");
  SQLPP_PORTABLE_STATIC_ASSERT(assert_update_set_allowed_t,
                               "at least one assignment is prohibited by its column definition in set()");
  SQLPP_PORTABLE_STATIC_ASSERT(assert_update_set_single_table_t,
                               "set() contains assignments for columns from more than one table");
  SQLPP_PORTABLE_STATIC_ASSERT(assert_update_set_count_args_t, "at least one assignment expression required in set()");

  template <typename... Assignments>
  using check_update_set_t = static_combined_check_t<
      static_check_t<logic::all<is_assignment<Assignments>::value...>::value,
                     assert_update_set_assignments_t>,
      static_check_t<not detail::has_duplicates<typename lhs<Assignments>::type...>::value,
                     assert_update_set_no_duplicates_t>,
      static_check_t<sizeof...(Assignments) == 0 or detail::make_joined_set_t<required_tables_of_t<
                                                        typename lhs<Assignments>::type>...>::size() == 1,
                     assert_update_set_single_table_t>>;

  template <typename... Assignments>
  struct check_update_static_set
  {
    using type = static_combined_check_t<check_update_set_t<remove_dynamic_t<Assignments>...>,
                                         static_check_t<sizeof...(Assignments) != 0, assert_update_set_count_args_t>>;
  };

  template <typename... Assignments>
  using check_update_static_set_t = typename check_update_static_set<Assignments...>::type;

  struct no_update_list_t
  {
  };

  template <typename Statement>
  struct clause_base<no_update_list_t, Statement> : public clause_data<no_update_list_t, Statement>
  {
    using clause_data<no_update_list_t, Statement>::clause_data;

#warning : reactivate check_update_static_set_t
    template <typename... Assignments>
    auto set(Assignments... assignments) const
        -> decltype(new_statement(*this, update_list_t<Assignments...>{std::make_tuple(assignments...)}))
    {
      return new_statement(*this, update_list_t<Assignments...>{std::make_tuple(assignments...)});
    }
  };

  template <typename Statement>
  struct consistency_check<Statement, no_update_list_t>
  {
    using type = assert_update_assignments_t;

  };

  // Interpreters
  template <typename Context>
  auto to_sql_string(Context& context, const no_update_list_t& t) -> std::string
  {
    return "";
  }

  template <typename Context, typename... Assignments>
  auto to_sql_string(Context& context, const update_list_t<Assignments...>& t) -> std::string
  {
    return " SET " + tuple_to_sql_string(context, t._assignments, tuple_operand_no_dynamic{", "});
  }

  template <typename... T>
  auto update_set(T&&... t) -> decltype(statement_t<no_update_list_t>().set(std::forward<T>(t)...))
  {
    return statement_t<no_update_list_t>().set(std::forward<T>(t)...);
  }

}  // namespace sqlpp
