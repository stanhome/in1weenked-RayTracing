#pragma once

//#define mu_log(format, ...) SCLOG(format, ##__VA_ARGS__)

#define _mu_text(n) #n
#define _mu_text_(n) _mu_text(n)
#define _mu_location() __FILE__ " (" _mu_text_(__LINE__ ) "): "

#define mu_assert(message, test) do { if (!(test)) return "[failed] " message "  --> file:" _mu_location(); else mu_log("[successed] " ); } while (0)
#define mu_run_test(test) do { char *message = test(); ++tests_run; \
if (message ) mu_log(message); else ++tests_successed; } while (0)
#define mu_testResult mu_log(""); mu_log( "TEST RESULTS:"); \
if (tests_run == tests_successed ) { mu_log("ALL TESTS PASSED"); } else { mu_log("SOME TESTS FAILED" ); }

extern int tests_run;
extern int tests_successed;

typedef void(*LogFunc)(const char *msg);
static LogFunc mu_log;

static void setLogFunc(LogFunc log)
{
	mu_log = log;
}