#ifndef MINUNIT_H
#define MINUNIT_H
#include <stdio.h>
/**
 * Borrowed from http://www.jera.com/techinfo/jtns/jtn002.html
 *
 * License:
 *      You may use the code in this tech note for any purpose,
 *      with the understanding that it comes with NO WARRANTY.
 */
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
    if (message) return message; } while (0)
#define mu_run_suite(suite) do { char *message = suite(); \
    if (message) return message; } while(0);

#define mu_suite_start(e) int _suite_start__ ##e = tests_run; fprintf(stderr, "-- Starting suite %s\n", #e);
#define mu_suite_end(e) fprintf(stderr, "-- Ending suite %s: %d tests passed\n", #e, tests_run - _suite_start__ ##e);
#define mu_msg(m) fprintf(stderr, m "\n");

extern int tests_run;
#endif
