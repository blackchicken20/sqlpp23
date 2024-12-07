#pragma once

// generated by ./scripts/ddl2cpp -auto-id -with-table-creation-helper tests/mysql/usage/Tables.sql tests/mysql/usage/Tables test

#include <sqlpp11/core/basic/table.h>
#include <sqlpp11/core/basic/table_columns.h>
#include <sqlpp11/core/type_traits.h>
#include <sqlpp11/core/name/create_name_tag.h>
#include <sqlpp11/core/compat/string_view.h>

namespace test
{
  template<typename Db>
  void createTabSample(Db& db)
  {
    db.execute(R"+++(DROP TABLE IF EXISTS tab_sample)+++");
    db.execute(R"+++(CREATE TABLE tab_sample (
  id bigint(20) AUTO_INCREMENT PRIMARY KEY,
  int_n bigint(20) DEFAULT NULL,
  text_n varchar(255) DEFAULT NULL,
  bool_n bool DEFAULT NULL
))+++");
  }

  struct TabSample_ : public ::sqlpp::name_tag_base
  {
    struct Id : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
      using value_type = ::sqlpp::integral;
      using has_default = std::true_type;
    };
    struct IntN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(int_n, intN);
      using value_type = ::sqlpp::optional<::sqlpp::integral>;
      using has_default = std::true_type;
    };
    struct TextN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(text_n, textN);
      using value_type = ::sqlpp::optional<::sqlpp::text>;
      using has_default = std::true_type;
    };
    struct BoolN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(bool_n, boolN);
      using value_type = ::sqlpp::optional<::sqlpp::boolean>;
      using has_default = std::true_type;
    };
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_sample, tabSample);
    template<typename T>
    using _table_columns = sqlpp::table_columns<T,
               Id,
               IntN,
               TextN,
               BoolN>;
    using _required_insert_columns = sqlpp::detail::type_set<>;
  };
  using TabSample = ::sqlpp::table_t<TabSample_>;

  template<typename Db>
  void createTabDateTime(Db& db)
  {
    db.execute(R"+++(DROP TABLE IF EXISTS tab_date_time)+++");
    db.execute(R"+++(CREATE TABLE tab_date_time (
  id bigint(20) AUTO_INCREMENT PRIMARY KEY,
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
      using value_type = ::sqlpp::integral;
      using has_default = std::true_type;
    };
    struct DayPointN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(day_point_n, dayPointN);
      using value_type = ::sqlpp::optional<::sqlpp::day_point>;
      using has_default = std::true_type;
    };
    struct TimePointN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(time_point_n, timePointN);
      using value_type = ::sqlpp::optional<::sqlpp::time_point>;
      using has_default = std::true_type;
    };
    struct DateTimePointND : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(date_time_point_n_d, dateTimePointND);
      using value_type = ::sqlpp::optional<::sqlpp::time_point>;
      using has_default = std::true_type;
    };
    struct TimeOfDayN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(time_of_day_n, timeOfDayN);
      using value_type = ::sqlpp::optional<::sqlpp::time_of_day>;
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

  template<typename Db>
  void createTabFoo(Db& db)
  {
    db.execute(R"+++(DROP TABLE IF EXISTS tab_foo)+++");
    db.execute(R"+++(CREATE TABLE tab_foo (
  id bigint(20) AUTO_INCREMENT PRIMARY KEY,
  int_n bigint(20) DEFAULT NULL
))+++");
  }

  struct TabFoo_ : public ::sqlpp::name_tag_base
  {
    struct Id : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
      using value_type = ::sqlpp::integral;
      using has_default = std::true_type;
    };
    struct IntN : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(int_n, intN);
      using value_type = ::sqlpp::optional<::sqlpp::integral>;
      using has_default = std::true_type;
    };
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_foo, tabFoo);
    template<typename T>
    using _table_columns = sqlpp::table_columns<T,
               Id,
               IntN>;
    using _required_insert_columns = sqlpp::detail::type_set<>;
  };
  using TabFoo = ::sqlpp::table_t<TabFoo_>;

  template<typename Db>
  void createTabJson(Db& db)
  {
    db.execute(R"+++(DROP TABLE IF EXISTS tab_json)+++");
    db.execute(R"+++(CREATE TABLE tab_json (
  id bigint(20) AUTO_INCREMENT PRIMARY KEY,
  data JSON NOT NULL
))+++");
  }

  struct TabJson_ : public ::sqlpp::name_tag_base
  {
    struct Id : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
      using value_type = ::sqlpp::integral;
      using has_default = std::true_type;
    };
    struct Data : public ::sqlpp::name_tag_base
    {
      SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(data, data);
      using value_type = ::sqlpp::text;
      using has_default = std::false_type;
    };
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_json, tabJson);
    template<typename T>
    using _table_columns = sqlpp::table_columns<T,
               Id,
               Data>;
    using _required_insert_columns = sqlpp::detail::type_set<
               sqlpp::column_t<sqlpp::table_t<TabJson_>, Data>>;
  };
  using TabJson = ::sqlpp::table_t<TabJson_>;

} // namespace test
