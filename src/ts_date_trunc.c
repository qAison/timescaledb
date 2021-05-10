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
	int origin_year = 2000, origin_month = 1, origin_day = 1;
	int year, month, day;
	int delta, bucket_number;

	if (PG_NARGS() > 2)
	{
		DateADT origin_date = PG_GETARG_DATUM(2);
		j2date(origin_date + POSTGRES_EPOCH_JDATE, &origin_year, &origin_month, &origin_day);

		if(origin_day != 1)
		{
			ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			 	errmsg("Invalid `origin` day, YYYY-MM-01 expected")));
		}
	}

	if((interval->time != 0) || (interval->day != 0))
	{
		ereport(ERROR,
			(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			 errmsg("only months are supported at the moment")));
	}

	if (DATE_NOT_FINITE(date))
		PG_RETURN_DATEADT(date);

	j2date(date + POSTGRES_EPOCH_JDATE, &year, &month, &day);

	if((year < origin_year) || ((year == origin_year) && (month < origin_month)))
	{
		ereport(ERROR,
			(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			 errmsg("`date` < `origin` not supported, choose another `origin`")));
	}

	delta = (year*12 + month) - (origin_year*12 + origin_month);
	bucket_number = delta / interval->month;
	year = origin_year + (bucket_number * interval->month)/12;
	month = (((origin_year*12 + (origin_month-1)) + (bucket_number * interval->month)) % 12) + 1;
	day = 1;

	date = date2j(year, month, day) - POSTGRES_EPOCH_JDATE;
	PG_RETURN_DATEADT(date);
}
