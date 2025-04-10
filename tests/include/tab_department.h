#pragma once

// generated by ./scripts/ddl2cpp -auto-id tests/include/tab_department.sql
// tests/include/tab_department model

#include <optional>

#include <sqlpp23/core/basic/table.h>
#include <sqlpp23/core/basic/table_columns.h>
#include <sqlpp23/core/name/create_name_tag.h>
#include <sqlpp23/core/type_traits.h>

namespace model {
struct TabDepartment_ : public ::sqlpp::name_tag_base {
  struct Id : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
    using data_type = ::sqlpp::integral;
    using has_default = std::true_type;
  };
  struct Name : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(name, name);
    using data_type = std::optional<::sqlpp::text>;
    using has_default = std::true_type;
  };
  struct Division : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(division, division);
    using data_type = ::sqlpp::text;
    using has_default = std::true_type;
  };
  SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_department, tabDepartment);
  template <typename T>
  using _table_columns = sqlpp::table_columns<T, Id, Name, Division>;
  using _required_insert_columns = sqlpp::detail::type_set<>;
};
using TabDepartment = ::sqlpp::table_t<TabDepartment_>;

}  // namespace model
