// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 1999-2002,2013-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#include "cx-hackery.hpp"

#define CX_TESTING
#include "cx-tracedebug.hpp"

#include <errno.h>
#include <cstdlib>
#include <cstdarg>

// changing this to a static and wrapping it with get()/set()
// because g++ 4.8.4 didn't seem to be extern'ing the original form
// properly, causing there to be multiple copies in different
// compilation units (leading, ultimately, to a segfault)
static bool g_initialized = false;
static bool g_enabled = true;

static FILE *g_debugfile = nullptr;
static FILE *g_errorfile = nullptr;

static char const *g_topicenv = nullptr;
static  char const *g_traceenv = nullptr;
static  char const *g_tracefile = nullptr;


#ifdef CX_OPT_TRACING
static U64 g_tracelevel = 0;
static char g_tracetab[256] = "";
#endif

// this has to be defined even when CX_OPT_TRACING is undefined,
// because even then it is still referenced by cx-exceptions
char const *cx_trace_methodname="{{{ unknown method/function }}}";


#ifdef CX_TESTING
char const *
CX::get_topicenv()
{
  return g_topicenv;
}


char const *
CX::get_traceenv()
{
  return g_traceenv;
}


char const *
CX::get_tracefile()
{
  return g_tracefile;
}
#endif

static void
_init_tracefile()
{
  if (!g_debugfile)
  {
    g_tracefile = std::getenv("CX_TRACEFILE");
    if (!g_tracefile)
    {
      // defaults
      CX::set_debugfile(stdout);
      CX::set_errorfile(stderr);
    }
    else
    {
      FILE* file;
      errno = 0;
      if (!(file = fopen(g_tracefile, "w")))
      {
        fprintf(stderr, "Unable to open CX_TRACEFILE '%s'\n", g_tracefile);
        perror("fopen(3) reports");
        fprintf(stderr, "All CX-related debug output is disabled.\n");
        g_enabled = false;
      }
      CX::set_debugfile(file);
      CX::set_errorfile(file);
    }
  }

  g_initialized = true;
}


bool
CX::is_enabled()
{
  if (!g_initialized)
    _init_tracefile();

  return g_enabled;
}


void CX::flush()
{
  if (g_debugfile) ::fflush(g_debugfile);
  if (g_errorfile) ::fflush(g_errorfile);
}


void
CX::set_debugfile(FILE* file)
{
  if (g_debugfile) ::fflush(g_debugfile);
  if (g_errorfile) ::fflush(g_errorfile);
  g_debugfile = file;
}


void
CX::set_errorfile(FILE* file)
{
  if (g_debugfile) ::fflush(g_debugfile);
  if (g_errorfile) ::fflush(g_errorfile);
  g_errorfile = file;
}

static const char *
_init_env_string(const char* name)
{
  char* envstr;

  if (envstr = std::getenv(name))
    envstr = strdup(envstr);
  else
    envstr = strdup("");

  return envstr;
}


static int
_trace_vfprintf_1(bool trace,
    FILE* file, char const* format, va_list args)
{
#ifdef CX_OPT_TRACING
  if (trace)
    fprintf(file, "%s", g_tracetab);
#endif

  // TODO: what if the string to be output is multi-lined?
  // Maybe we should detect that and insert indentations, somehow...
  return ::vfprintf(file, format, args);
}


static int
_trace_vfprintf(bool trace,
    FILE* file, char const* format, va_list args)
{
  if (file && CX::is_enabled())
    return _trace_vfprintf_1(trace, file, format, args);

  return 0;
}


static int
_trace_fprintf(bool trace, FILE* file, char const* format, ...)
{
  if (file && CX::is_enabled())
  {
    int ret;
    va_list args;
    va_start(args, format);
    /* TODO: doesn't work with PRIu64, etc? */
    ret = _trace_vfprintf_1(trace, file, format, args);
    va_end(args);
    return ret;
  }

  return 0;
}


#ifdef CX_OPT_TRACING

static void
_update_trace_tab()
{
  U64 i;

  // The point is to allow more than 64 levels of trace
  // (for robustness,) but to only tab-in for the first
  // 64 trace levels (a practical limitation.)

  for (i = 0; i < (CX_MIN((U64)g_tracelevel, 64UL)*3); ++i)
    g_tracetab[i] = ' ';

  g_tracetab[CX_MIN((U64)g_tracelevel, 64UL)*1] = '\x0';
}

U64 CX::get_tracelevel()
{
  return g_tracelevel;
}

void CX::set_tracelevel(U64 level)
{
  g_tracelevel = level;
  _update_trace_tab();
}


void
CX::shift_in()
{
  g_tracelevel++;
  _update_trace_tab();
}


void
CX::shift_out()
{
  if (g_tracelevel)
  {
    g_tracelevel -= !!g_tracelevel;
    g_tracetab[CX_MAX(0, ((long)g_tracelevel))*1] = '\x0';
  }
}


bool
CX::is_section_active(const char* section)
{
  if (!CX::is_enabled())
    return false;

  if (!g_traceenv)
    g_traceenv = _init_env_string("CX_TRACE");

  if (!g_traceenv || !*g_traceenv)
    return false;  // empty is treated the same as undefined

  // TODO: better globbing support
  if (strstr(g_traceenv, "*"))
    return true;  // asterisk in CX_TRACE means "show all trace sections"

  // section names present in CX_TRACE are the only ones traced.
  const char* found = strstr(g_traceenv, section);
  if (!found)
    return false;

  // filter out partial matches
  size_t len = strlen(section);
  bool begin = (found == g_traceenv) || (found[-1] == ' ');
  bool end   = (strlen(found) == len)  || (found[len] == ' ');
  return (begin && end);
}


void
CX::traceout(char const* section, char const* format, ...)
{
  if (!CX::is_enabled())
    return;

  if (!CX::is_section_active(section))
    return;

  va_list args;
  va_start(args, format);
  _trace_vfprintf_1(true, g_debugfile, format, args);
  va_end(args);
}

#endif


static const char *
_my_strchrnul(const char* s, int c)
{
  if (!s) return nullptr;

  while (*s && (*s != c))
    ++s;

  return s;
}


static size_t
_get_next_word(const char** start, size_t len, char* word)
{
  while (*start && **start == ' ')
    ++*start; // skip past leading delims

  const char* end = _my_strchrnul(*start, ' ');
  len = CX_MIN(len-1, end - *start);
  strncpy(word, *start, len);
  word[len] = '\0';
  *start = *end ? end+1 : nullptr;

  return len;
}


static bool
_is_topic_active(const char* topic)
{
  if (!g_topicenv)
    g_topicenv = _init_env_string("CX_TOPICS");

  if (!g_topicenv || !*g_topicenv)
    return false;

  //g_topicenv "foo:bar" should only match exact topic "foo:bar"
  //g_topicenv "foo:" should match any topic beginning with "foo:"
  //g_topicenv ":bar" should match any topic ending with ":bar"
  //g_topicenv "*" should match any topic [special]
  //g_topicenv "foo:bar*" should ONLY match the literal "foo:bar*"
  const char* start = g_topicenv;
  char word[64];
  const char* found;
  do
  {
    size_t len = _get_next_word(&start, sizeof(word), word);
    if (len)
    {
      if ((len ==1) && (*word == '*') && topic && *topic)
        return true;

      if (found = strstr(/*haystack*/topic, /*needle*/word))
      {
        // if word didn't match @ the beginning of topic,
        // then word needs to start with a colon.
        if (found != topic)
          return (*word == ':');

        // if word didn't match all the way to the end of topic,
        // then word needs to end with a colon
        if (strlen(found) > len)
          return (word[len-1] == ':');

        return true;
      }
    }
  } while (start);

  return false;
}



void
CX::debugout(char const* format, ...)
{
  if (!CX::is_enabled())
    return;

  int ret;
  va_list args;
  va_start(args, format);

  _trace_vfprintf_1(true, g_debugfile, format, args);
  va_end(args);
}


void
CX::warning(bool test, char const* format, ...)
{
  if (!test)
    return;

  if (!CX::is_enabled())
    return;

  _trace_fprintf(true, g_errorfile, "??? ");

  va_list args;
  va_start(args, format);
  _trace_vfprintf_1(false, g_errorfile, format, args);
  va_end(args);
}


void
CX::topicout(char const* topic, char const* format, ...)
{
  if (!CX::is_enabled())
    return;

  if (!_is_topic_active(topic))
    return;

  _trace_fprintf(true, g_debugfile, "[%s]  ", topic);

  va_list args;
  va_start(args, format);
  _trace_vfprintf_1(false, g_debugfile, format, args);
  va_end(args);

  // TODO: need compile- or runtime-test to enable this flush
  // (don't want it always enabled for perf reasons)
#if 0
  ::fflush(g_debugfile);
#endif
}


void
CX::errorout(bool test, char const* format, ...)
{
  if (!test)
    return;

  va_list args;
  va_start(args, format);

  if (!CX::is_enabled())
  {
    // CX output is disabled-- usually because an invalid filename
    // was given for $CX_TRACEFILE-- but this is an error message,
    // and hence likely important.  So, try sending it to stderr
    ::vfprintf(stderr, format, args);
    ::fflush(stderr);
  }
  else
  {
    // flushing can preserve correct order of output when
    // g_debugfile and g_errorfile are not the same streams.
    bool same = (g_debugfile == g_errorfile);
    if (!same) ::fflush(g_debugfile);
    /////_trace_fprintf(true, g_errorfile, "!!! ");
    _trace_vfprintf(false, g_errorfile, format, args);
    if (!same) ::fflush(g_errorfile);
  }

  va_end(args);
}

char const* g_assert_messages[] =
{
  "DEUS EX MACHINA: a tragedy has occurred, and \n"
  "                     higher powers have been summoned.",

  "THUS SPAKE ZARATHUSTRA:\n"
  "      'You must be ready to burn yourself in your own flame; \n"
  "       how could you rise anew if you have not first become ashes?'",

  "OZYMANDIAS: 'Look upon my Works, ye Mighty, and despair!'",

  "#INCLUDE <HUBRIS>  // this code was too clever by half",

  "ASSERTION HAIKU:  Something you entered\n"
  "                      transcended parameters;\n"
  "                      program aborted.",
};

char const*
CX::get_assert_message()
{
  srand(time(nullptr));
  int msg = rand() % (sizeof(g_assert_messages) / sizeof(char const*));
  return g_assert_messages[msg];
}

