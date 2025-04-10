#pragma once

// generated by ./scripts/ddl2cpp -auto-id -with-table-creation-helper
// tests/sqlite3/usage/Tables.sql tests/sqlite3/usage/Tables test

#include <optional>

#include <sqlpp23/core/basic/table.h>
#include <sqlpp23/core/basic/table_columns.h>
#include <sqlpp23/core/name/create_name_tag.h>
#include <sqlpp23/core/type_traits.h>

namespace test {
template <typename Db>
void createTabSample(Db& db) {
  db(R"+++(DROP TABLE IF EXISTS tab_sample)+++");
  db(R"+++(CREATE TABLE tab_sample (
  id INTEGER PRIMARY KEY,
  alpha bigint(20) DEFAULT NULL,
  beta varchar(255) DEFAULT NULL,
  gamma boolean
))+++");
}

struct TabSample_ : public ::sqlpp::name_tag_base {
  struct Id : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
    using data_type = ::sqlpp::integral;
    using has_default = std::true_type;
  };
  struct Alpha : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(alpha, alpha);
    using data_type = std::optional<::sqlpp::integral>;
    using has_default = std::true_type;
  };
  struct Beta : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(beta, beta);
    using data_type = std::optional<::sqlpp::text>;
    using has_default = std::true_type;
  };
  struct Gamma : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(gamma, gamma);
    using data_type = std::optional<::sqlpp::boolean>;
    using has_default = std::true_type;
  };
  SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_sample, tabSample);
  template <typename T>
  using _table_columns = sqlpp::table_columns<T, Id, Alpha, Beta, Gamma>;
  using _required_insert_columns = sqlpp::detail::type_set<>;
};
using TabSample = ::sqlpp::table_t<TabSample_>;

template <typename Db>
void createTabFoo(Db& db) {
  db(R"+++(DROP TABLE IF EXISTS tab_foo)+++");
  db(R"+++(CREATE TABLE tab_foo (
  omega bigint(20) DEFAULT NULL
))+++");
}

struct TabFoo_ : public ::sqlpp::name_tag_base {
  struct Omega : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(omega, omega);
    using data_type = std::optional<::sqlpp::integral>;
    using has_default = std::true_type;
  };
  SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_foo, tabFoo);
  template <typename T>
  using _table_columns = sqlpp::table_columns<T, Omega>;
  using _required_insert_columns = sqlpp::detail::type_set<>;
};
using TabFoo = ::sqlpp::table_t<TabFoo_>;

template <typename Db>
void createIntegralSample(Db& db) {
  db(R"+++(DROP TABLE IF EXISTS integral_sample)+++");
  db(R"+++(CREATE TABLE integral_sample (
  signed_value INTEGER,
  unsigned_value INTEGER UNSIGNED
))+++");
}

struct IntegralSample_ : public ::sqlpp::name_tag_base {
  struct SignedValue : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(signed_value, signedValue);
    using data_type = std::optional<::sqlpp::integral>;
    using has_default = std::true_type;
  };
  struct UnsignedValue : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(unsigned_value, unsignedValue);
    using data_type = std::optional<::sqlpp::unsigned_integral>;
    using has_default = std::true_type;
  };
  SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(integral_sample, integralSample);
  template <typename T>
  using _table_columns = sqlpp::table_columns<T, SignedValue, UnsignedValue>;
  using _required_insert_columns = sqlpp::detail::type_set<>;
};
using IntegralSample = ::sqlpp::table_t<IntegralSample_>;

template <typename Db>
void createBlobSample(Db& db) {
  db(R"+++(DROP TABLE IF EXISTS blob_sample)+++");
  db(R"+++(CREATE TABLE blob_sample (
  id INTEGER PRIMARY KEY,
  data blob
))+++");
}

struct BlobSample_ : public ::sqlpp::name_tag_base {
  struct Id : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
    using data_type = ::sqlpp::integral;
    using has_default = std::true_type;
  };
  struct Data : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(data, data);
    using data_type = std::optional<::sqlpp::blob>;
    using has_default = std::true_type;
  };
  SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(blob_sample, blobSample);
  template <typename T>
  using _table_columns = sqlpp::table_columns<T, Id, Data>;
  using _required_insert_columns = sqlpp::detail::type_set<>;
};
using BlobSample = ::sqlpp::table_t<BlobSample_>;

template <typename Db>
void createFpSample(Db& db) {
  db(R"+++(DROP TABLE IF EXISTS fp_sample)+++");
  db(R"+++(CREATE TABLE fp_sample (
  id INTEGER PRIMARY KEY,
  fp DOUBLE
))+++");
}

struct FpSample_ : public ::sqlpp::name_tag_base {
  struct Id : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
    using data_type = ::sqlpp::integral;
    using has_default = std::true_type;
  };
  struct Fp : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(fp, fp);
    using data_type = std::optional<::sqlpp::floating_point>;
    using has_default = std::true_type;
  };
  SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(fp_sample, fpSample);
  template <typename T>
  using _table_columns = sqlpp::table_columns<T, Id, Fp>;
  using _required_insert_columns = sqlpp::detail::type_set<>;
};
using FpSample = ::sqlpp::table_t<FpSample_>;

template <typename Db>
void createTabDateTime(Db& db) {
  db(R"+++(DROP TABLE IF EXISTS tab_date_time)+++");
  db(R"+++(CREATE TABLE tab_date_time
(
  id INTEGER PRIMARY KEY,
  day_point_n date,
  time_point_n datetime,
  time_of_day_n time
))+++");
}

struct TabDateTime_ : public ::sqlpp::name_tag_base {
  struct Id : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(id, id);
    using data_type = ::sqlpp::integral;
    using has_default = std::true_type;
  };
  struct DayPointN : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(day_point_n, dayPointN);
    using data_type = std::optional<::sqlpp::day_point>;
    using has_default = std::true_type;
  };
  struct TimePointN : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(time_point_n, timePointN);
    using data_type = std::optional<::sqlpp::time_point>;
    using has_default = std::true_type;
  };
  struct TimeOfDayN : public ::sqlpp::name_tag_base {
    SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(time_of_day_n, timeOfDayN);
    using data_type = std::optional<::sqlpp::time_of_day>;
    using has_default = std::true_type;
  };
  SQLPP_CREATE_NAME_TAG_FOR_SQL_AND_CPP(tab_date_time, tabDateTime);
  template <typename T>
  using _table_columns =
      sqlpp::table_columns<T, Id, DayPointN, TimePointN, TimeOfDayN>;
  using _required_insert_columns = sqlpp::detail::type_set<>;
};
using TabDateTime = ::sqlpp::table_t<TabDateTime_>;

}  // namespace test
