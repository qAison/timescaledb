-- This file and its contents are licensed under the Timescale License.
-- Please see the included NOTICE for copyright information and
-- LICENSE-TIMESCALE for a copy of the license.

\c :TEST_DBNAME :ROLE_SUPERUSER

SELECT * FROM add_server('server1', 'localhost', 'server1', port=>current_setting('port')::integer);
SELECT * FROM add_server('server2', 'localhost', 'server2', port=>current_setting('port')::integer);
SELECT * FROM add_server('server3', 'localhost', 'server3', port=>current_setting('port')::integer);

\des+

CREATE FUNCTION _timescaledb_internal.invoke_distributed_commands()
RETURNS void
AS :TSL_MODULE_PATHNAME, 'tsl_invoke_distributed_commands'
LANGUAGE C STRICT;

CREATE FUNCTION _timescaledb_internal.invoke_faulty_distributed_command()
RETURNS void
AS :TSL_MODULE_PATHNAME, 'tsl_invoke_faulty_distributed_command'
LANGUAGE C STRICT;

SELECT _timescaledb_internal.invoke_distributed_commands();

\c server1
\dt
SELECT * FROM disttable1;
\c server2
\dt
SELECT * FROM disttable1;
\c server3
\dt
SELECT * FROM disttable1;
\c single

-- Verify failed insert command gets fully rolled back
\set ON_ERROR_STOP 0
SELECT _timescaledb_internal.invoke_faulty_distributed_command();
\set ON_ERROR_STOP 1

\c server1
SELECT * from disttable2;
\c server3
SELECT * from disttable2;

-- Test connection session identity
\c :TEST_DBNAME :ROLE_SUPERUSER
\unset ECHO
\o /dev/null
\ir include/remote_exec.sql
\o
\set ECHO all

-- Register is_frontend_session() function and test that it returns false for
-- connections openned by test suite. This simualates behaviour expected
-- with a client connections.
CREATE OR REPLACE FUNCTION is_frontend_session()
RETURNS BOOL
AS :TSL_MODULE_PATHNAME, 'test_is_frontend_session' LANGUAGE C;

\c server1
CREATE OR REPLACE FUNCTION is_frontend_session()
RETURNS BOOL
AS :TSL_MODULE_PATHNAME, 'test_is_frontend_session' LANGUAGE C;
SELECT is_frontend_session();

\c server2
CREATE OR REPLACE FUNCTION is_frontend_session()
RETURNS BOOL
AS :TSL_MODULE_PATHNAME, 'test_is_frontend_session' LANGUAGE C;
SELECT is_frontend_session();

\c server3
CREATE OR REPLACE FUNCTION is_frontend_session()
RETURNS BOOL
AS :TSL_MODULE_PATHNAME, 'test_is_frontend_session' LANGUAGE C;
SELECT is_frontend_session();

\c :TEST_DBNAME :ROLE_SUPERUSER
SELECT is_frontend_session();

-- Ensure peer dist id is already set and can be set only once
\set ON_ERROR_STOP 0
SELECT * FROM test.remote_exec('{server1}', $$ SELECT * FROM _timescaledb_internal.set_peer_dist_id('77348176-09da-4a80-bc78-e31bdf5e63ec'); $$);
\set ON_ERROR_STOP 1

-- Repeat is_frontend_session() test again, but this time using connections openned from frontend
-- to backend nodes. Must return true.
SELECT * FROM test.remote_exec(NULL, $$ SELECT is_frontend_session(); $$);

\c single
DROP DATABASE server1;
DROP DATABASE server2;
DROP DATABASE server3;
