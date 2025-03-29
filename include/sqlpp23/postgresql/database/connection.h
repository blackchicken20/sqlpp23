#pragma once

/**
 * Copyright © 2014-2015, Matthijs Möhlmann
 * Copyright (c) 2023, Vesselin Atanasov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
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

#include <iostream>
#include <memory>

#include <sqlpp23/core/database/connection.h>
#include <sqlpp23/core/database/transaction.h>
#include <sqlpp23/core/query/statement_constructor_arg.h>
#include <sqlpp23/core/to_sql_string.h>
#include <sqlpp23/postgresql/bind_result.h>
#include <sqlpp23/postgresql/database/connection_config.h>
#include <sqlpp23/postgresql/database/connection_handle.h>
#include <sqlpp23/postgresql/database/exception.h>
#include <sqlpp23/postgresql/database/serializer_context.h>
#include <sqlpp23/postgresql/detail/prepared_statement_handle.h>
#include <sqlpp23/postgresql/prepared_statement.h>
#include <sqlpp23/postgresql/result.h>
#include <sqlpp23/postgresql/to_sql_string.h>

#ifdef SQLPP_DYNAMIC_LOADING
#include <sqlpp23/postgresql/dynamic_libpq.h>
#endif

struct pg_conn;
typedef struct pg_conn PGconn;

namespace sqlpp::postgresql {
#ifdef SQLPP_DYNAMIC_LOADING
using namespace dynamic;
#endif

namespace detail {
inline std::unique_ptr<detail::prepared_statement_handle_t> prepare_statement(
    std::unique_ptr<connection_handle>& handle,
    const std::string& stmt,
    const size_t& param_count) {
  if (handle->config->debug) {
    std::cerr << "PostgreSQL debug: preparing: " << stmt << std::endl;
  }

  return std::make_unique<detail::prepared_statement_handle_t>(*handle, stmt,
                                                               param_count);
}

inline void execute_prepared_statement(
    std::unique_ptr<connection_handle>& handle,
    std::shared_ptr<detail::prepared_statement_handle_t>& prepared) {
  if (handle->config->debug) {
    std::cerr << "PostgreSQL debug: executing: " << prepared->name()
              << std::endl;
  }
  prepared->execute();
}
}  // namespace detail

// Base connection class
class connection_base : public sqlpp::connection {
 private:
  bool _transaction_active{false};

  void validate_connection_handle() const {
    if (!_handle) {
      throw std::logic_error{"connection handle used, but not initialized"};
    }
  }

  // direct execution
  bind_result_t select_impl(const std::string& stmt) { return execute(stmt); }

  size_t insert_impl(const std::string& stmt) {
    return static_cast<size_t>(execute(stmt)->result.affected_rows());
  }

  size_t update_impl(const std::string& stmt) {
    return static_cast<size_t>(execute(stmt)->result.affected_rows());
  }

  size_t remove_impl(const std::string& stmt) {
    return static_cast<size_t>(execute(stmt)->result.affected_rows());
  }

  // prepared execution
  prepared_statement_t prepare_impl(const std::string& stmt,
                                    const size_t& param_count) {
    validate_connection_handle();
    return {prepare_statement(_handle, stmt, param_count)};
  }

  bind_result_t run_prepared_select_impl(prepared_statement_t& prep) {
    validate_connection_handle();
    execute_prepared_statement(_handle, prep._handle);
    return {prep._handle};
  }

  size_t run_prepared_execute_impl(prepared_statement_t& prep) {
    validate_connection_handle();
    execute_prepared_statement(_handle, prep._handle);
    return static_cast<size_t>(prep._handle->result.affected_rows());
  }

  size_t run_prepared_insert_impl(prepared_statement_t& prep) {
    validate_connection_handle();
    execute_prepared_statement(_handle, prep._handle);
    return static_cast<size_t>(prep._handle->result.affected_rows());
  }

  size_t run_prepared_update_impl(prepared_statement_t& prep) {
    validate_connection_handle();
    execute_prepared_statement(_handle, prep._handle);
    return static_cast<size_t>(prep._handle->result.affected_rows());
  }

  size_t run_prepared_remove_impl(prepared_statement_t& prep) {
    validate_connection_handle();
    execute_prepared_statement(_handle, prep._handle);
    return static_cast<size_t>(prep._handle->result.affected_rows());
  }

 public:
  using _connection_base_t = connection_base;
  using _config_t = connection_config;
  using _config_ptr_t = std::shared_ptr<const _config_t>;
  using _handle_t = detail::connection_handle;
  using _handle_ptr_t = std::unique_ptr<_handle_t>;

  using _prepared_statement_t = prepared_statement_t;

  struct _tags {
    using _null_result_is_trivial_value = std::true_type;
  };

  // Select stmt (returns a result)
  template <typename Select>
  bind_result_t select(const Select& s) {
    context_t context(this);
    return select_impl(to_sql_string(context, s));
  }

  // Prepared select
  template <typename Select>
  _prepared_statement_t prepare_select(Select& s) {
    context_t context(this);
    return prepare_impl(to_sql_string(context, s), context._count);
  }

  template <typename PreparedSelect>
  bind_result_t run_prepared_select(const PreparedSelect& s) {
    s._bind_params();
    return run_prepared_select_impl(s._prepared_statement);
  }

  // Insert
  template <typename Insert>
  size_t insert(const Insert& s) {
    context_t context(this);
    return insert_impl(to_sql_string(context, s));
  }

  template <typename Insert>
  prepared_statement_t prepare_insert(Insert& s) {
    context_t context(this);
    return prepare_impl(to_sql_string(context, s), context._count);
  }

  template <typename PreparedInsert>
  size_t run_prepared_insert(const PreparedInsert& i) {
    i._bind_params();
    return run_prepared_insert_impl(i._prepared_statement);
  }

  // Update
  template <typename Update>
  size_t update(const Update& s) {
    context_t context(this);
    return update_impl(to_sql_string(context, s));
  }

  template <typename Update>
  prepared_statement_t prepare_update(Update& s) {
    context_t context(this);
    return prepare_impl(to_sql_string(context, s), context._count);
  }

  template <typename PreparedUpdate>
  size_t run_prepared_update(const PreparedUpdate& u) {
    u._bind_params();
    return run_prepared_update_impl(u._prepared_statement);
  }

  // Remove
  template <typename Remove>
  size_t remove(const Remove& s) {
    context_t context(this);
    return remove_impl(to_sql_string(context, s));
  }

  template <typename Remove>
  prepared_statement_t prepare_remove(Remove& s) {
    context_t context(this);
    return prepare_impl(to_sql_string(context, s), context._count);
  }

  template <typename PreparedRemove>
  size_t run_prepared_remove(const PreparedRemove& r) {
    r._bind_params();
    return run_prepared_remove_impl(r._prepared_statement);
  }

  //! Execute a single statement (like creating a table).
  //! Note that technically, this supports executing multiple statements today,
  //! but this is likely to change to align with other connectors.
  std::shared_ptr<detail::statement_handle_t> execute(const std::string& stmt) {
    validate_connection_handle();
    if (_handle->config->debug) {
      std::cerr << "PostgreSQL debug: executing: " << stmt << std::endl;
    }

    auto result = std::make_shared<detail::statement_handle_t>(*_handle);
    result->result = PQexec(native_handle(), stmt.c_str());
    result->valid = true;

    return result;
  }

  template <typename Execute,
            typename Enable = typename std::enable_if<
                not std::is_convertible<Execute, std::string>::value,
                void>::type>
  std::shared_ptr<detail::statement_handle_t> execute(const Execute& s) {
    context_t context(this);
    return execute(to_sql_string(context, s));
  }

  template <typename Execute>
  _prepared_statement_t prepare_execute(Execute& s) {
    context_t context(this);
    return prepare_impl(to_sql_string(context, s), context._count);
  }

  template <typename PreparedExecute>
  size_t run_prepared_execute(const PreparedExecute& x) {
    x._prepared_statement._reset();
    x._bind_params();
    return run_prepared_execute_impl(x._prepared_statement);
  }

  template <typename T>
    requires(sqlpp::statement_run_check_t<T>::value)
  auto operator()(const T& t) {
    return sqlpp::statement_handler_t{}.run(t, *this);
  }

  template <typename T>
    requires(sqlpp::statement_prepare_check_t<T>::value)
  auto prepare(const T& t) {
    return sqlpp::statement_handler_t{}.prepare(t, *this);
  }

  //! set the default transaction isolation level to use for new transactions
  void set_default_isolation_level(isolation_level level) {
    std::string level_str = "read uncommmitted";
    switch (level) {
      /// @todo what about undefined ?
      case isolation_level::read_committed:
        level_str = "read committed";
        break;
      case isolation_level::read_uncommitted:
        level_str = "read uncommitted";
        break;
      case isolation_level::repeatable_read:
        level_str = "repeatable read";
        break;
      case isolation_level::serializable:
        level_str = "serializable";
        break;
      default:
        throw sqlpp::exception{"Invalid isolation level"};
    }
    std::string cmd =
        "SET default_transaction_isolation to '" + level_str + "'";
    execute(cmd);
  }

  //! get the currently set default transaction isolation level
  isolation_level get_default_isolation_level() {
    auto res = execute("SHOW default_transaction_isolation;");
    auto status = res->result.status();
    if ((status != PGRES_TUPLES_OK) && (status != PGRES_COMMAND_OK)) {
      throw sqlpp::exception{
          "PostgreSQL error: could not read default_transaction_isolation"};
    }

    auto in = res->result.get_string_value(0, 0);
    if (in == "read committed") {
      return isolation_level::read_committed;
    } else if (in == "read uncommitted") {
      return isolation_level::read_uncommitted;
    } else if (in == "repeatable read") {
      return isolation_level::repeatable_read;
    } else if (in == "serializable") {
      return isolation_level::serializable;
    }
    return isolation_level::undefined;
  }

  //! create savepoint
  void savepoint(const std::string& name) {
    /// NOTE prevent from sql injection?
    execute("SAVEPOINT " + name);
  }

  //! ROLLBACK TO SAVEPOINT
  void rollback_to_savepoint(const std::string& name) {
    /// NOTE prevent from sql injection?
    execute("ROLLBACK TO SAVEPOINT " + name);
  }

  //! release_savepoint
  void release_savepoint(const std::string& name) {
    /// NOTE prevent from sql injection?
    execute("RELEASE SAVEPOINT " + name);
  }

  //! start transaction
  void start_transaction(isolation_level level = isolation_level::undefined) {
    if (_transaction_active) {
      throw sqlpp::exception{"PostgreSQL error: transaction already open"};
    }
    switch (level) {
      case isolation_level::serializable: {
        execute("BEGIN ISOLATION LEVEL SERIALIZABLE");
        break;
      }
      case isolation_level::repeatable_read: {
        execute("BEGIN ISOLATION LEVEL REPEATABLE READ");
        break;
      }
      case isolation_level::read_committed: {
        execute("BEGIN ISOLATION LEVEL READ COMMITTED");
        break;
      }
      case isolation_level::read_uncommitted: {
        execute("BEGIN ISOLATION LEVEL READ UNCOMMITTED");
        break;
      }
      case isolation_level::undefined: {
        execute("BEGIN");
        break;
      }
    }
    _transaction_active = true;
  }

  //! commit transaction (or throw transaction if transaction has finished
  //! already)
  void commit_transaction() {
    if (!_transaction_active) {
      throw sqlpp::exception{
          "PostgreSQL error: transaction failed or finished."};
    }
    execute("COMMIT");
    _transaction_active = false;
  }

  //! rollback transaction
  void rollback_transaction(bool report) {
    if (!_transaction_active) {
      throw sqlpp::exception{
          "PostgreSQL error: transaction failed or finished."};
    }
    if (report) {
      std::cerr << "PostgreSQL warning: rolling back unfinished transaction"
                << std::endl;
    }
    execute("ROLLBACK");
    _transaction_active = false;
  }

  //! report rollback failure
  void report_rollback_failure(const std::string& message) noexcept {
    std::cerr << "PostgreSQL error: " << message << std::endl;
  }

  //! check if transaction is active
  bool is_transaction_active() { return _transaction_active; }

  //! get the last inserted id for a certain table
  uint64_t last_insert_id(const std::string& table,
                          const std::string& fieldname) {
    std::string sql = "SELECT currval('" + table + "_" + fieldname + "_seq')";
    PGresult* res = PQexec(native_handle(), sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
      std::string err{PQresultErrorMessage(res)};
      PQclear(res);
      throw sqlpp::postgresql::undefined_table{err, sql};
    }

    // Parse the number and return.
    std::string in{PQgetvalue(res, 0, 0)};
    PQclear(res);
    return std::stoul(in);
  }

  ::PGconn* native_handle() const { return _handle->native_handle(); }

  std::string escape(const std::string_view& s) const {
    validate_connection_handle();
    // Escape strings
    std::string result;
    result.resize((s.size() * 2) + 1);

    int err;
    size_t length = PQescapeStringConn(native_handle(), &result[0], s.data(),
                                       s.size(), &err);
    result.resize(length);
    return result;
  }

 protected:
  _handle_ptr_t _handle;

  // Constructors
  connection_base() = default;
  connection_base(_handle_ptr_t&& handle) : _handle{std::move(handle)} {}
};

inline auto context_t::escape(std::string_view t) -> std::string {
  return _db->escape(t);
}

using connection = sqlpp::normal_connection<connection_base>;
using pooled_connection = sqlpp::pooled_connection<connection_base>;
}  // namespace sqlpp::postgresql

