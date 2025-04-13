#pragma once

// generated by ./scripts/ddl2cpp -auto-id -with-table-creation-helper tests/include/sqlpp23/tests/sqlite3/tables.sql tests/include/sqlpp23/tests/sqlite3/tables test

#include <optional>

#include <sqlpp23/core/basic/table.h>
#include <sqlpp23/core/basic/table_columns.h>
#include <sqlpp23/core/type_traits.h>
#include <sqlpp23/core/name/create_name_tag.h>

namespace test
{
  template<typename Db>
  void createTabFoo(Db& db)
  {
    db(R"+++(DROP TABLE IF EXISTS tab_foo)+++");
    db(R"+++(CREATE TABLE tab_foo
(
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  text_nn_d varchar(255) NOT NULL DEFAULT '',
  int_n bigint,
  double_n double precision,
  u_int_n bigint UNSIGNED,
  bool_n bool,
  blob_n blob
))+++");
  }

  struct TabFoo_ : public ::sqlpp::name_tag_base
  {
    struct Id : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
      using data_type = ::sqlpp::integral;
      using has_default = std::true_type;
    };
    struct TextNnD : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(text_nn_d, textNnD);
      using data_type = ::sqlpp::text;
      using has_default = std::true_type;
    };
    struct IntN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(int_n, intN);
      using data_type = std::optional<::sqlpp::integral>;
      using has_default = std::true_type;
    };
    struct DoubleN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(double_n, doubleN);
      using data_type = std::optional<::sqlpp::floating_point>;
      using has_default = std::true_type;
    };
    struct UIntN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(u_int_n, uIntN);
      using data_type = std::optional<::sqlpp::unsigned_integral>;
      using has_default = std::true_type;
    };
    struct BoolN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(bool_n, boolN);
      using data_type = std::optional<::sqlpp::boolean>;
      using has_default = std::true_type;
    };
    struct BlobN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(blob_n, blobN);
      using data_type = std::optional<::sqlpp::blob>;
      using has_default = std::true_type;
    };
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_foo, tabFoo);
    template<typename T>
    using _table_columns = sqlpp::table_columns<T,
               Id,
               TextNnD,
               IntN,
               DoubleN,
               UIntN,
               BoolN,
               BlobN>;
    using _required_insert_columns = sqlpp::detail::type_set<>;
  };
  using TabFoo = ::sqlpp::table_t<TabFoo_>;

  template<typename Db>
  void createTabBar(Db& db)
  {
    db(R"+++(DROP TABLE IF EXISTS tab_bar)+++");
    db(R"+++(CREATE TABLE tab_bar
(
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  text_n varchar(255) NULL,
  bool_nn bool NOT NULL DEFAULT false,
  int_n int
))+++");
  }

  struct TabBar_ : public ::sqlpp::name_tag_base
  {
    struct Id : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
      using data_type = ::sqlpp::integral;
      using has_default = std::true_type;
    };
    struct TextN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(text_n, textN);
      using data_type = std::optional<::sqlpp::text>;
      using has_default = std::true_type;
    };
    struct BoolNn : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(bool_nn, boolNn);
      using data_type = ::sqlpp::boolean;
      using has_default = std::true_type;
    };
    struct IntN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(int_n, intN);
      using data_type = std::optional<::sqlpp::integral>;
      using has_default = std::true_type;
    };
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_bar, tabBar);
    template<typename T>
    using _table_columns = sqlpp::table_columns<T,
               Id,
               TextN,
               BoolNn,
               IntN>;
    using _required_insert_columns = sqlpp::detail::type_set<>;
  };
  using TabBar = ::sqlpp::table_t<TabBar_>;

  template<typename Db>
  void createTabDateTime(Db& db)
  {
    db(R"+++(DROP TABLE IF EXISTS tab_date_time)+++");
    db(R"+++(CREATE TABLE tab_date_time (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  day_point_n date,
  time_point_n datetime(3),
  date_time_point_n_d datetime DEFAULT CURRENT_TIMESTAMP,
  time_of_day_n time(3)
))+++");
  }

  struct TabDateTime_ : public ::sqlpp::name_tag_base
  {
    struct Id : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
      using data_type = ::sqlpp::integral;
      using has_default = std::true_type;
    };
    struct DayPointN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(day_point_n, dayPointN);
      using data_type = std::optional<::sqlpp::day_point>;
      using has_default = std::true_type;
    };
    struct TimePointN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(time_point_n, timePointN);
      using data_type = std::optional<::sqlpp::time_point>;
      using has_default = std::true_type;
    };
    struct DateTimePointND : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(date_time_point_n_d, dateTimePointND);
      using data_type = std::optional<::sqlpp::time_point>;
      using has_default = std::true_type;
    };
    struct TimeOfDayN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(time_of_day_n, timeOfDayN);
      using data_type = std::optional<::sqlpp::time_of_day>;
      using has_default = std::true_type;
    };
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_date_time, tabDateTime);
    template<typename T>
    using _table_columns = sqlpp::table_columns<T,
               Id,
               DayPointN,
               TimePointN,
               DateTimePointND,
               TimeOfDayN>;
    using _required_insert_columns = sqlpp::detail::type_set<>;
  };
  using TabDateTime = ::sqlpp::table_t<TabDateTime_>;

} // namespace test
