/*
 * This file and its contents are licensed under the Apache License 2.0.
 * Please see the included NOTICE for copyright information and
 * LICENSE-APACHE for a copy of the license.
 */
#include <postgres.h>
#include <catalog/pg_type.h>
#include <fmgr.h>
#include <utils/builtins.h>
#include <utils/date.h>
#include <utils/datetime.h>
#include <utils/fmgrprotos.h>
#include <utils/timestamp.h>

#include "utils.h"
#include "ts_date_trunc.h"

TSDLLEXPORT Datum
ts_date_trunc(PG_FUNCTION_ARGS)
{
	Interval *interval = PG_GETARG_INTERVAL_P(0);
	DateADT date = PG_GETARG_DATEADT(1);
	//Timestamp origin = DEFAULT_ORIGIN;
	//Timestamp timestamp, result;
	//int64 period = -1;

	if (DATE_NOT_FINITE(date))
		PG_RETURN_DATEADT(date);

	// Do nothing yet
	PG_RETURN_DATEADT(date);

	//period = get_interval_period_timestamp_units(interval);
	/* check the period aligns on a date */
	//check_period_is_daily(period);

	/* convert to timestamp (NOT tz), bucket, convert back to date */
	//timestamp = DatumGetTimestamp(DirectFunctionCall1(date_timestamp, PG_GETARG_DATUM(1)));
	//if (PG_NARGS() > 2)
	//	origin = DatumGetTimestamp(DirectFunctionCall1(date_timestamp, PG_GETARG_DATUM(2)));

	//Assert(!TIMESTAMP_NOT_FINITE(timestamp));

	//TIME_BUCKET_TS(period, timestamp, result, origin);

	//PG_RETURN_DATUM(DirectFunctionCall1(timestamp_date, TimestampGetDatum(result)));
}
