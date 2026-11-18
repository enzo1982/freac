 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_TEST_CHECK
#define H_FREAC_TEST_CHECK

#include <smooth.h>

#include <stdio.h>

using namespace smooth;

/* Checks for the test programs. Check prints each failed check, and
 * CheckSummary prints the result and returns the exit code.
 */
static Int	 failures = 0;

static Void Check(Bool condition, const String &description)
{
	if (condition) return;

	failures++;

	printf("FAIL: %s\n", (char *) description);
}

static Int CheckSummary()
{
	if (failures == 0) printf("All checks passed.\n");
	else		   printf("%d checks failed.\n", (int) failures);

	return failures == 0 ? 0 : 1;
}

/* Several values of a key as one string, for comparison.
 */
static inline String Join(const Array<String> &values)
{
	return String::Implode(values, "|");
}

#endif
