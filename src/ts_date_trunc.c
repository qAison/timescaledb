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

TS_FUNCTION_INFO_V1(ts_date_trunc);

TSDLLEXPORT Datum
ts_date_trunc(PG_FUNCTION_ARGS)
{
	Interval *interval = PG_GETARG_INTERVAL_P(0);
	DateADT date = PG_GETARG_DATEADT(1);
	//Timestamp origin = DEFAULT_ORIGIN;
	//Timestamp timestamp, result;
	//int64 period = -1;

	if (PG_NARGS() > 2)
	{
	//if (PG_NARGS() > 2)
	//	origin = DatumGetTimestamp(DirectFunctionCall1(date_timestamp, PG_GETARG_DATUM(2)));
		ereport(ERROR,
			(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			 errmsg("`origin` argument is not supported yet")));
	}

	if((interval->time != 0) || (interval->day != 0))
	{
		ereport(ERROR,
			(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			 errmsg("only months are supported at the moment")));
	}

	// DateADT is number of days (integer) since 2000-01-01
	// 7752 == 2021-03-23
	//printf("interval->month = %d\n", interval->month);
	//printf("date = %d\n", date); 

	if (DATE_NOT_FINITE(date))
		PG_RETURN_DATEADT(date);

	// Do nothing yet
	PG_RETURN_DATEADT(date);

	//period = get_interval_period_timestamp_units(interval);
	/* check the period aligns on a date */
	//check_period_is_daily(period);

	/* convert to timestamp (NOT tz), bucket, convert back to date */
	//timestamp = DatumGetTimestamp(DirectFunctionCall1(date_timestamp, PG_GETARG_DATUM(1)));


	//Assert(!TIMESTAMP_NOT_FINITE(timestamp));

	//TIME_BUCKET_TS(period, timestamp, result, origin);

	//PG_RETURN_DATUM(DirectFunctionCall1(timestamp_date, TimestampGetDatum(result)));
}
