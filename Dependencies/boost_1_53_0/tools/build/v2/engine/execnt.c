/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Copyright 2007 Rene Rivera.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "jam.h"
#include "lists.h"
#include "execcmd.h"
#include "pathsys.h"
#include "string.h"
#include "output.h"
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#ifdef USE_EXECNT

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <tlhelp32.h>

/*
 * execnt.c - execute a shell command on Windows NT
 *
 * If $(JAMSHELL) is defined, uses that to formulate execvp()/spawnvp().
 * The default is:
 *
 *  /bin/sh -c %        [ on UNIX/AmigaOS ]
 *  cmd.exe /c %        [ on Windows NT ]
 *
 * Each word must be an individual element in a jam variable value.
 *
 * In $(JAMSHELL), % expands to the command string and ! expands to
 * the slot number (starting at 1) for multiprocess (-j) invocations.
 * If $(JAMSHELL) doesn't include a %, it is tacked on as the last
 * argument.
 *
 * Don't just set JAMSHELL to /bin/sh or cmd.exe - it won't work!
 *
 * External routines:
 *  exec_cmd() - launch an async command execution.
 *  exec_wait() - wait and drive at most one execution completion.
 *
 * Internal routines:
 *  onintr() - bump intr to note command interruption.
 *
 * 04/08/94 (seiwald) - Coherent/386 support added.
 * 05/04/94 (seiwald) - async multiprocess interface
 * 01/22/95 (seiwald) - $(JAMSHELL) support
 * 06/02/97 (gsar)    - full async multiprocess support for Win32
 */

/* get the maximum command line length according to the OS */
int maxline();

/* delete and argv list */
static void free_argv(const char * *);
/* Convert a command string into arguments for spawnvp. */
static const char** string_to_args(const char*);
/* bump intr to note command interruption */
static void onintr(int);
/* If the command is suitable for execution via spawnvp */
long can_spawn(const char*);
/* Add two 64-bit unsigned numbers, h1l1 and h2l2 */
static FILETIME add_64(
    unsigned long h1, unsigned long l1,
    unsigned long h2, unsigned long l2);
static FILETIME add_FILETIME(FILETIME t1, FILETIME t2);
static FILETIME negate_FILETIME(FILETIME t);
/* Convert a FILETIME to a number of seconds */
static double filetime_seconds(FILETIME t);
/* record the timing info for the process */
static void record_times(HANDLE, timing_info*);
/* calc the current running time of an *active* process */
static double running_time(HANDLE);
/* */
DWORD get_process_id(HANDLE);
/* terminate the given process, after terminating all its children */
static void kill_process_tree(DWORD, HANDLE);
/* waits for a command to complete or for the given timeout, whichever is first */
static int try_wait(int timeoutMillis);
/* reads any pending output for running commands */
static void read_output();
/* checks if a command ran out of time, and kills it */
static int try_kill_one();
/* */
static double creation_time(HANDLE);
/* Recursive check if first process is parent (directly or indirectly) of
the second one. */
static int is_parent_child(DWORD, DWORD);
/* */
static void close_alert(HANDLE);
/* close any alerts hanging around */
static void close_alerts();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int intr = 0;
static int cmdsrunning = 0;
static void (* istat)( int );


/* The list of commands we run. */
static struct
{
    string action;   /* buffer to hold action */
    string target;   /* buffer to hold target */
    string command;  /* buffer to hold command being invoked */

    /* Temporary batch file used to execute the action when needed. */
    char * tempfile_bat;

    /* Pipes for communicating with the child process. Parent reads from (0),
     * child writes to (1).
     */
    HANDLE pipe_out[ 2 ];
    HANDLE pipe_err[ 2 ];

    string              buffer_out;   /* buffer to hold stdout, if any */
    string              buffer_err;   /* buffer to hold stderr, if any */

    PROCESS_INFORMATION pi;           /* running process information */
    DWORD               exit_code;    /* executed command's exit code */
    int                 exit_reason;  /* reason why a command completed */

    /* Function called when the command completes. */
    void (* func)( void * closure, int status, timing_info *, const char *, const char * );

    /* Opaque data passed back to the 'func' callback called when the command
     * completes.
     */
    void * closure;
}
cmdtab[ MAXJOBS ] = { { 0 } };


/*
 * Execution unit tests.
 */

void execnt_unit_test()
{
#if !defined( NDEBUG )
    /* vc6 preprocessor is broken, so assert with these strings gets confused.
     * Use a table instead.
     */
    typedef struct test { char * command; int result; } test;
    test tests[] = {
        { "x", 0 },
        { "x\n ", 0 },
        { "x\ny", 1 },
        { "x\n\n y", 1 },
        { "echo x > foo.bar", 1 },
        { "echo x < foo.bar", 1 },
        { "echo x \">\" foo.bar", 0 },
        { "echo x \"<\" foo.bar", 0 },
        { "echo x \\\">\\\" foo.bar", 1 },
        { "echo x \\\"<\\\" foo.bar", 1 } };
    int i;
    for ( i = 0; i < sizeof( tests ) / sizeof( *tests ); ++i )
        assert( !can_spawn( tests[ i ].command ) == tests[ i ].result );

    {
        char * long_command = BJAM_MALLOC_ATOMIC( MAXLINE + 10 );
        assert( long_command != 0 );
        memset( long_command, 'x', MAXLINE + 9 );
        long_command[ MAXLINE + 9 ] = 0;
        assert( can_spawn( long_command ) == MAXLINE + 9 );
        BJAM_FREE( long_command );
    }

    {
        /* Work around vc6 bug; it doesn't like escaped string
         * literals inside assert
         */
        const char * * argv = string_to_args(" \"g++\" -c -I\"Foobar\"" );
        char const expected[] = "-c -I\"Foobar\"";

        assert( !strcmp( argv[ 0 ], "g++" ) );
        assert( !strcmp( argv[ 1 ], expected ) );
        free_argv( argv );
    }
#endif
}


/*
 * exec_cmd() - launch an async command execution.
 */

void exec_cmd
(
    const char * command,
    void (* func)( void * closure, int status, timing_info *, const char * invoked_command, const char * command_output ),
    void       * closure,
    LIST       * shell,
    const char * action,
    const char * target
)
{
    int      slot;
    int      raw_cmd = 0 ;
    const char   * argv_static[ MAXARGC + 1 ];  /* +1 for NULL */
    const char * * argv = argv_static;
    char   * p;
    const char * command_orig = command;

    /* Check to see if we need to hack around the line-length limitation. Look
     * for a JAMSHELL setting of "%", indicating that the command should be
     * invoked directly.
     */
    if ( !list_empty( shell ) && !strcmp( object_str( list_front( shell ) ), "%" ) && list_next( list_begin( shell ) ) == list_end( shell ) )
    {
        raw_cmd = 1;
        shell = 0;
    }

    /* Find a slot in the running commands table for this one. */
    for ( slot = 0; slot < MAXJOBS; ++slot )
        if ( !cmdtab[ slot ].pi.hProcess )
            break;
    if ( slot == MAXJOBS )
    {
        printf( "no slots for child!\n" );
        exit( EXITBAD );
    }

    /* Compute the name of a temp batch file, for possible use. */
    if ( !cmdtab[ slot ].tempfile_bat )
    {
        char const * tempdir = path_tmpdir();
        DWORD procID = GetCurrentProcessId();

        /* SVA - allocate 64 bytes extra just to be safe. */
        cmdtab[ slot ].tempfile_bat = BJAM_MALLOC_ATOMIC( strlen( tempdir ) + 64 );

        sprintf( cmdtab[ slot ].tempfile_bat, "%s\\jam%d-%02d.bat",
            tempdir, procID, slot );
    }

    /* Trim leading, -ending- white space */
    while ( *( command + 1 ) && isspace( *command ) )
        ++command;

    /* Write to .BAT file unless the line would be too long and it meets the
     * other spawnability criteria.
     */
    if ( raw_cmd && ( can_spawn( command ) >= MAXLINE ) )
    {
        if ( DEBUG_EXECCMD )
            printf("Executing raw command directly\n");
    }
    else
    {
        FILE * f = 0;
        int tries = 0;
        raw_cmd = 0;

        /* Write command to bat file. For some reason this open can fail
         * intermitently. But doing some retries works. Most likely this is due
         * to a previously existing file of the same name that happens to be
         * opened by an active virus scanner. Pointed out and fixed by Bronek
         * Kozicki.
         */
        for ( ; !f && ( tries < 4 ); ++tries )
        {
            f = fopen( cmdtab[ slot ].tempfile_bat, "w" );
            if ( !f && ( tries < 4 ) ) Sleep( 250 );
        }
        if ( !f )
        {
            printf( "failed to write command file!\n" );
            exit( EXITBAD );
        }
        fputs( command, f );
        fclose( f );

        command = cmdtab[ slot ].tempfile_bat;

        if ( DEBUG_EXECCMD )
        {
            if ( !list_empty( shell ) )
                printf( "using user-specified shell: %s", object_str( list_front( shell ) ) );
            else
                printf( "Executing through .bat file\n" );
        }
    }

    /* Formulate argv; If shell was defined, be prepared for % and ! subs.
     * Otherwise, use stock cmd.exe.
     */
    if ( shell )
    {
        int i;
        char jobno[ 4 ];
        int gotpercent = 0;
        LISTITER shell_iter = list_begin( shell ), shell_end = list_end( shell );

        sprintf( jobno, "%d", slot + 1 );

        for ( i = 0; shell_iter != shell_end && ( i < MAXARGC ); ++i, shell_iter = list_next( shell_iter ) )
        {
            switch ( object_str( list_item( shell_iter ) )[ 0 ] )
            {
                case '%': argv[ i ] = command; ++gotpercent; break;
                case '!': argv[ i ] = jobno; break;
                default : argv[ i ] = object_str( list_item( shell_iter ) );
            }
            if ( DEBUG_EXECCMD )
                printf( "argv[%d] = '%s'\n", i, argv[ i ] );
        }

        if ( !gotpercent )
            argv[ i++ ] = command;

        argv[ i ] = 0;
    }
    else if ( raw_cmd )
    {
        argv = string_to_args( command );
    }
    else
    {
        argv[ 0 ] = "cmd.exe";
        argv[ 1 ] = "/Q/C";  /* anything more is non-portable */
        argv[ 2 ] = command;
        argv[ 3 ] = 0;
    }

    /* Catch interrupts whenever commands are running. */
    if ( !cmdsrunning++ )
        istat = signal( SIGINT, onintr );

    /* Start the command. */
    {
        SECURITY_ATTRIBUTES sa
            = { sizeof( SECURITY_ATTRIBUTES ), 0, 0 };
        SECURITY_DESCRIPTOR sd;
        STARTUPINFO si
            = { sizeof( STARTUPINFO ), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        string cmd;

        /* Init the security data. */
        InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
        SetSecurityDescriptorDacl( &sd, TRUE, NULL, FALSE );
        sa.lpSecurityDescriptor = &sd;
        sa.bInheritHandle = TRUE;

        /* Create the stdout, which is also the merged out + err, pipe. */
        if ( !CreatePipe( &cmdtab[ slot ].pipe_out[ 0 ],
            &cmdtab[ slot ].pipe_out[ 1 ], &sa, 0 ) )
        {
            perror( "CreatePipe" );
            exit( EXITBAD );
        }

        /* Create the stdout, which is also the merged out+err, pipe. */
        if ( globs.pipe_action == 2 )
        {
            if ( !CreatePipe( &cmdtab[ slot ].pipe_err[ 0 ],
                &cmdtab[ slot ].pipe_err[ 1 ], &sa, 0 ) )
            {
                perror( "CreatePipe" );
                exit( EXITBAD );
            }
        }

        /* Set handle inheritance off for the pipe ends the parent reads from. */
        SetHandleInformation( cmdtab[ slot ].pipe_out[ 0 ], HANDLE_FLAG_INHERIT, 0 );
        if ( globs.pipe_action == 2 )
            SetHandleInformation( cmdtab[ slot ].pipe_err[ 0 ], HANDLE_FLAG_INHERIT, 0 );

        /* Hide the child window, if any. */
        si.dwFlags |= STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        /* Set the child outputs to the pipes. */
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdOutput = cmdtab[ slot ].pipe_out[ 1 ];
        if ( globs.pipe_action == 2 )
        {
            /* Pipe stderr to the action error output. */
            si.hStdError = cmdtab[ slot ].pipe_err[ 1 ];
        }
        else if ( globs.pipe_action == 1 )
        {
            /* Pipe stderr to the console error output. */
            si.hStdError = GetStdHandle( STD_ERROR_HANDLE );
        }
        else
        {
            /* Pipe stderr to the action merged output. */
            si.hStdError = cmdtab[ slot ].pipe_out[ 1 ];
        }

        /* Let the child inherit stdin, as some commands assume it's available. */
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

        /* Save the operation for exec_wait() to find. */
        cmdtab[ slot ].func = func;
        cmdtab[ slot ].closure = closure;
        if ( action && target )
        {
            string_copy( &cmdtab[ slot ].action, action );
            string_copy( &cmdtab[ slot ].target, target );
        }
        else
        {
            string_free( &cmdtab[ slot ].action );
            string_new ( &cmdtab[ slot ].action );
            string_free( &cmdtab[ slot ].target );
            string_new ( &cmdtab[ slot ].target );
        }
        string_copy( &cmdtab[ slot ].command, command_orig );

        /* Put together the command we run. */
        {
            const char * * argp = argv;
            string_new( &cmd );
            string_copy( &cmd, *(argp++) );
            while ( *argp )
            {
                string_push_back( &cmd, ' ' );
                string_append( &cmd, *(argp++) );
            }
        }

        /* Create output buffers. */
        string_new( &cmdtab[ slot ].buffer_out );
        string_new( &cmdtab[ slot ].buffer_err );

        /* Run the command by creating a sub-process for it. */
        if (
            ! CreateProcess(
                NULL                    ,  /* application name               */
                cmd.value               ,  /* command line                   */
                NULL                    ,  /* process attributes             */
                NULL                    ,  /* thread attributes              */
                TRUE                    ,  /* inherit handles                */
                CREATE_NEW_PROCESS_GROUP,  /* create flags                   */
                NULL                    ,  /* env vars, null inherits env    */
                NULL                    ,  /* current dir, null is our       */
                                           /* current dir                    */
                &si                     ,  /* startup info                   */
                &cmdtab[ slot ].pi         /* child process info, if created */
                )
            )
        {
            perror( "CreateProcess" );
            exit( EXITBAD );
        }

        /* Clean up temporary stuff. */
        string_free( &cmd );
    }

    /* Wait until we are under the limit of concurrent commands. Do not trust
     * globs.jobs alone.
     */
    while ( ( cmdsrunning >= MAXJOBS ) || ( cmdsrunning >= globs.jobs ) )
        if ( !exec_wait() )
            break;

    if ( argv != argv_static )
        free_argv( argv );
}


/*
 * exec_wait()
 *  * wait and drive at most one execution completion.
 *  * waits for one command to complete, while processing the i/o for all
 *    ongoing commands.
 *
 *   Returns 0 if called when there were no more commands being executed or 1
 * otherwise.
 */

int exec_wait()
{
    int i = -1;

    /* Handle naive make1() which does not know if cmds are running. */
    if ( !cmdsrunning )
        return 0;

    /* Wait for a command to complete, while snarfing up any output. */
    do
    {
        /* Check for a complete command, briefly. */
        i = try_wait(500);
        /* Read in the output of all running commands. */
        read_output();
        /* Close out pending debug style dialogs. */
        close_alerts();
        /* Check if a command ran out of time. */
        if ( i < 0 ) i = try_kill_one();
    }
    while ( i < 0 );

    /* We have a command... process it. */
    --cmdsrunning;
    {
        timing_info time;
        int rstat;

        /* The time data for the command. */
        record_times( cmdtab[ i ].pi.hProcess, &time );

        /* Clear the temp file. */
        if ( cmdtab[ i ].tempfile_bat )
        {
            unlink( cmdtab[ i ].tempfile_bat );
            BJAM_FREE( cmdtab[ i ].tempfile_bat );
            cmdtab[ i ].tempfile_bat = NULL;
        }

        /* Find out the process exit code. */
        GetExitCodeProcess( cmdtab[ i ].pi.hProcess, &cmdtab[ i ].exit_code );

        /* The dispossition of the command. */
        if ( intr )
            rstat = EXEC_CMD_INTR;
        else if ( cmdtab[ i ].exit_code != 0 )
            rstat = EXEC_CMD_FAIL;
        else
            rstat = EXEC_CMD_OK;

        /* Output the action block. */
        out_action(
            cmdtab[ i ].action.size     > 0 ? cmdtab[ i ].action.value     : 0,
            cmdtab[ i ].target.size     > 0 ? cmdtab[ i ].target.value     : 0,
            cmdtab[ i ].command.size    > 0 ? cmdtab[ i ].command.value    : 0,
            cmdtab[ i ].buffer_out.size > 0 ? cmdtab[ i ].buffer_out.value : 0,
            cmdtab[ i ].buffer_err.size > 0 ? cmdtab[ i ].buffer_err.value : 0,
            cmdtab[ i ].exit_reason );

        /* Call the callback, may call back to jam rule land. Assume -p0 in
         * effect so only pass buffer containing merged output.
         */
        (*cmdtab[ i ].func)(
            cmdtab[ i ].closure,
            rstat,
            &time,
            cmdtab[ i ].command.value,
            cmdtab[ i ].buffer_out.value );

        /* Clean up the command data, process, etc. */
        string_free( &cmdtab[ i ].action  ); string_new( &cmdtab[ i ].action  );
        string_free( &cmdtab[ i ].target  ); string_new( &cmdtab[ i ].target  );
        string_free( &cmdtab[ i ].command ); string_new( &cmdtab[ i ].command );
        if ( cmdtab[ i ].pi.hProcess   ) { CloseHandle( cmdtab[ i ].pi.hProcess   ); cmdtab[ i ].pi.hProcess   = 0; }
        if ( cmdtab[ i ].pi.hThread    ) { CloseHandle( cmdtab[ i ].pi.hThread    ); cmdtab[ i ].pi.hThread    = 0; }
        if ( cmdtab[ i ].pipe_out[ 0 ] ) { CloseHandle( cmdtab[ i ].pipe_out[ 0 ] ); cmdtab[ i ].pipe_out[ 0 ] = 0; }
        if ( cmdtab[ i ].pipe_out[ 1 ] ) { CloseHandle( cmdtab[ i ].pipe_out[ 1 ] ); cmdtab[ i ].pipe_out[ 1 ] = 0; }
        if ( cmdtab[ i ].pipe_err[ 0 ] ) { CloseHandle( cmdtab[ i ].pipe_err[ 0 ] ); cmdtab[ i ].pipe_err[ 0 ] = 0; }
        if ( cmdtab[ i ].pipe_err[ 1 ] ) { CloseHandle( cmdtab[ i ].pipe_err[ 1 ] ); cmdtab[ i ].pipe_err[ 1 ] = 0; }
        string_free( &cmdtab[ i ].buffer_out ); string_new( &cmdtab[ i ].buffer_out );
        string_free( &cmdtab[ i ].buffer_err ); string_new( &cmdtab[ i ].buffer_err );
        cmdtab[ i ].exit_code = 0;
        cmdtab[ i ].exit_reason = EXIT_OK;
    }

    return 1;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void free_argv( const char * * args )
{
    BJAM_FREE( (void *)args[ 0 ] );
    BJAM_FREE( (void *)args );
}


/*
 * For more details on Windows cmd.exe shell command-line length limitations see
 * the following MSDN article:
 *     http://support.microsoft.com/default.aspx?scid=kb;en-us;830473
 */

int maxline()
{
    OSVERSIONINFO os_info;
    os_info.dwOSVersionInfoSize = sizeof( os_info );
    GetVersionEx( &os_info );

    if ( os_info.dwMajorVersion >= 5 ) return 8191; /* XP >     */
    if ( os_info.dwMajorVersion == 4 ) return 2047; /* NT 4.x   */
    return 996;                                     /* NT 3.5.1 */
}


/*
 * Convert a command string into arguments for spawnvp(). The original code,
 * inherited from ftjam, tried to break up every argument on the command-line,
 * dealing with quotes, but that is really a waste of time on Win32, at least.
 * It turns out that all you need to do is get the raw path to the executable in
 * the first argument to spawnvp(), and you can pass all the rest of the
 * command-line arguments to spawnvp() in one, un-processed string.
 *
 * New strategy: break the string in at most one place.
 */

static const char * * string_to_args( char const * string )
{
    int            src_len;
    int            in_quote;
    char         * line;
    char   const * src;
    char         * dst;
    const char * * argv;

    /* Drop leading and trailing whitespace if any. */
    while ( isspace( *string ) )
        ++string;

    src_len = strlen( string );
    while ( ( src_len > 0 ) && isspace( string[ src_len - 1 ] ) )
        --src_len;

    /* Copy the input string into a buffer we can modify. */
    line = (char *)BJAM_MALLOC_ATOMIC( src_len + 1 );
    if ( !line )
        return 0;

    /* Allocate the argv array.
     *   element 0: stores the path to the executable
     *   element 1: stores the command-line arguments to the executable
     *   element 2: NULL terminator
     */
    argv = (const char * *)BJAM_MALLOC( 3 * sizeof( const char * ) );
    if ( !argv )
    {
        BJAM_FREE( line );
        return 0;
    }

    /* Strip quotes from the first command-line argument and find where it ends.
     * Quotes are illegal in Win32 pathnames, so we do not need to worry about
     * preserving escaped quotes here. Spaces can not be escaped in Win32, only
     * enclosed in quotes, so removing backslash escapes is also a non-issue.
     */
    in_quote = 0;
    for ( src = string, dst = line ; *src; ++src )
    {
        if ( *src == '"' )
            in_quote = !in_quote;
        else if ( !in_quote && isspace( *src ) )
            break;
        else
            *dst++ = *src;
    }
    *dst++ = 0;
    argv[ 0 ] = line;

    /* Skip whitespace in src. */
    while ( isspace( *src ) )
        ++src;

    argv[ 1 ] = dst;

    /* Copy the rest of the arguments verbatim. */
    src_len -= src - string;

    /* Use strncat() because it appends a trailing nul. */
    *dst = 0;
    strncat( dst, src, src_len );

    argv[ 2 ] = 0;

    return argv;
}


static void onintr( int disp )
{
    ++intr;
    printf( "...interrupted\n" );
}


/*
 * can_spawn() - If the command is suitable for execution via spawnvp(), return
 * a number >= the number of characters it would occupy on the command-line.
 * Otherwise, return zero.
 */

long can_spawn( const char * command )
{
    const char * p;
    char inquote = 0;

    /* Move to the first non-whitespace. */
    command += strspn( command, " \t" );

    p = command;

    /* Look for newlines and unquoted i/o redirection. */
    do
    {
        p += strcspn( p, "'\n\"<>|" );

        switch ( *p )
        {
        case '\n':
            /* Skip over any following spaces. */
            while ( isspace( *p ) )
                ++p;
            /* Must use a .bat file if there is anything significant following
             * the newline.
             */
            if ( *p )
                return 0;
            break;

        case '"':
        case '\'':
            if ( ( p > command ) && ( p[ -1 ] != '\\' ) )
            {
                if ( inquote == *p )
                    inquote = 0;
                else if ( inquote == 0 )
                    inquote = *p;
            }
            ++p;
            break;

        case '<':
        case '>':
        case '|':
            if ( !inquote )
                return 0;
            ++p;
            break;
        }
    }
    while ( *p );

    /* Return the number of characters the command will occupy. */
    return p - command;
}


/* 64-bit arithmetic helpers. */

/* Compute the carry bit from the addition of two 32-bit unsigned numbers. */
#define add_carry_bit( a, b ) ( (((a) | (b)) >> 31) & (~((a) + (b)) >> 31) & 0x1 )

/* Compute the high 32 bits of the addition of two 64-bit unsigned numbers, h1l1 and h2l2. */
#define add_64_hi( h1, l1, h2, l2 ) ((h1) + (h2) + add_carry_bit(l1, l2))


/*
 * Add two 64-bit unsigned numbers, h1l1 and h2l2.
 */

static FILETIME add_64
(
    unsigned long h1, unsigned long l1,
    unsigned long h2, unsigned long l2
)
{
    FILETIME result;
    result.dwLowDateTime = l1 + l2;
    result.dwHighDateTime = add_64_hi( h1, l1, h2, l2 );
    return result;
}


static FILETIME add_FILETIME( FILETIME t1, FILETIME t2 )
{
    return add_64( t1.dwHighDateTime, t1.dwLowDateTime, t2.dwHighDateTime,
        t2.dwLowDateTime );
}


static FILETIME negate_FILETIME( FILETIME t )
{
    /* 2s complement negation */
    return add_64( ~t.dwHighDateTime, ~t.dwLowDateTime, 0, 1 );
}


/*
 * Convert a FILETIME to a number of seconds.
 */

static double filetime_seconds( FILETIME t )
{
    return t.dwHighDateTime * ( (double)( 1UL << 31 ) * 2.0 * 1.0e-7 ) + t.dwLowDateTime * 1.0e-7;
}


/*
 * What should be a simple conversion, turns out to be horribly complicated by
 * the defficiencies of MSVC and the Win32 API.
 */

static time_t filetime_dt( FILETIME t_utc )
{
    static int calc_time_diff = 1;
    static double time_diff;
    if ( calc_time_diff )
    {
        struct tm t0_;
        FILETIME f0_local;
        FILETIME f0_;
        SYSTEMTIME s0_;
        GetSystemTime( &s0_ );
        t0_.tm_year = s0_.wYear-1900;
        t0_.tm_mon = s0_.wMonth-1;
        t0_.tm_wday = s0_.wDayOfWeek;
        t0_.tm_mday = s0_.wDay;
        t0_.tm_hour = s0_.wHour;
        t0_.tm_min = s0_.wMinute;
        t0_.tm_sec = s0_.wSecond;
        t0_.tm_isdst = 0;
        SystemTimeToFileTime( &s0_, &f0_local );
        LocalFileTimeToFileTime( &f0_local, &f0_ );
        time_diff = filetime_seconds( f0_ ) - (double)mktime( &t0_ );
        calc_time_diff = 0;
    }
    return ceil( filetime_seconds( t_utc ) - time_diff );
}


static void record_times( HANDLE process, timing_info * time )
{
    FILETIME creation;
    FILETIME exit;
    FILETIME kernel;
    FILETIME user;
    if ( GetProcessTimes( process, &creation, &exit, &kernel, &user ) )
    {
        time->system = filetime_seconds( kernel   );
        time->user   = filetime_seconds( user     );
        time->start  = filetime_dt     ( creation );
        time->end    = filetime_dt     ( exit     );
    }
}


#define IO_BUFFER_SIZE ( 16 * 1024 )

static char ioBuffer[ IO_BUFFER_SIZE + 1 ];


static void read_pipe
(
    HANDLE   in,  /* the pipe to read from */
    string * out
)
{
    DWORD bytesInBuffer = 0;
    DWORD bytesAvailable = 0;

    do
    {
        /* check if we have any data to read */
        if ( !PeekNamedPipe( in, ioBuffer, IO_BUFFER_SIZE, &bytesInBuffer, &bytesAvailable, NULL ) )
            bytesAvailable = 0;

        /* read in the available data */
        if ( bytesAvailable > 0 )
        {
            /* we only read in the available bytes, to avoid blocking */
            if ( ReadFile( in, ioBuffer,
                bytesAvailable <= IO_BUFFER_SIZE ? bytesAvailable : IO_BUFFER_SIZE,
                &bytesInBuffer, NULL ) )
            {
                if ( bytesInBuffer > 0 )
                {
                    /* Clean up some illegal chars. */
                    int i;
                    for ( i = 0; i < bytesInBuffer; ++i )
                    {
                        if ( ( (unsigned char)ioBuffer[ i ] < 1 ) )
                            ioBuffer[ i ] = '?';
                    }
                    /* Null, terminate. */
                    ioBuffer[ bytesInBuffer ] = '\0';
                    /* Append to the output. */
                    string_append( out, ioBuffer );
                    /* Subtract what we read in. */
                    bytesAvailable -= bytesInBuffer;
                }
                else
                {
                    /* Likely read a error, bail out. */
                    bytesAvailable = 0;
                }
            }
            else
            {
                /* Definitely read a error, bail out. */
                bytesAvailable = 0;
            }
        }
    }
    while ( bytesAvailable > 0 );
}


static void read_output()
{
    int i;
    for ( i = 0; i < globs.jobs && i < MAXJOBS; ++i )
    {
        /* Read stdout data. */
        if ( cmdtab[ i ].pipe_out[ 0 ] )
            read_pipe( cmdtab[ i ].pipe_out[ 0 ], & cmdtab[ i ].buffer_out );
        /* Read stderr data. */
        if ( cmdtab[ i ].pipe_err[ 0 ] )
            read_pipe( cmdtab[ i ].pipe_err[ 0 ], & cmdtab[ i ].buffer_err );
    }
}


/*
 * Waits for a single child process command to complete, or the timeout,
 * whichever comes first. Returns the index of the completed command in the
 * cmdtab array, or -1.
 */

static int try_wait( int timeoutMillis )
{
    int i;
    int num_active;
    int wait_api_result;
    HANDLE active_handles[ MAXJOBS ];
    int active_procs[ MAXJOBS ];

    /* Prepare a list of all active processes to wait for. */
    for ( num_active = 0, i = 0; i < globs.jobs; ++i )
    {
        if ( cmdtab[ i ].pi.hProcess )
        {
            active_handles[ num_active ] = cmdtab[ i ].pi.hProcess;
            active_procs[ num_active ] = i;
            ++num_active;
        }
    }

    /* Wait for a child to complete, or for our timeout window to expire. */
    wait_api_result = WaitForMultipleObjects( num_active, active_handles,
        FALSE, timeoutMillis );
    if ( ( WAIT_OBJECT_0 <= wait_api_result ) &&
        ( wait_api_result < WAIT_OBJECT_0 + num_active ) )
    {
        /* Rerminated process detected - return its index. */
        return active_procs[ wait_api_result - WAIT_OBJECT_0 ];
    }

    /* Timeout. */
    return -1;
}


static int try_kill_one()
{
    /* Only need to check if a timeout was specified with the -l option. */
    if ( globs.timeout > 0 )
    {
        int i;
        for ( i = 0; i < globs.jobs; ++i )
        {
            double t = running_time( cmdtab[ i ].pi.hProcess );
            if ( t > (double)globs.timeout )
            {
                /* The job may have left an alert dialog around, try and get rid
                 * of it before killing
                 */
                close_alert( cmdtab[ i ].pi.hProcess );
                /* We have a "runaway" job, kill it. */
                kill_process_tree( 0, cmdtab[ i ].pi.hProcess );
                /* And return it marked as a timeout. */
                cmdtab[ i ].exit_reason = EXIT_TIMEOUT;
                return i;
            }
        }
    }
    return -1;
}


static void close_alerts()
{
    /* We only attempt this every 5 seconds, or so, because it is not a cheap
     * operation, and we will catch the alerts eventually. This check uses
     * floats as some compilers define CLOCKS_PER_SEC as a float or double.
     */
    if ( ( (float)clock() / (float)( CLOCKS_PER_SEC * 5 ) ) < ( 1.0 / 5.0 ) )
    {
        int i;
        for ( i = 0; i < globs.jobs; ++i )
            close_alert( cmdtab[ i ].pi.hProcess );
    }
}


/*
 * Calc the current running time of an *active* process.
 */

static double running_time( HANDLE process )
{
    FILETIME creation;
    FILETIME exit;
    FILETIME kernel;
    FILETIME user;
    FILETIME current;
    if ( GetProcessTimes( process, &creation, &exit, &kernel, &user ) )
    {
        /* Compute the elapsed time. */
        GetSystemTimeAsFileTime( &current );
        return filetime_seconds( add_FILETIME( current,
            negate_FILETIME( creation ) ) );
    }
    return 0.0;
}


/* It is just stupidly silly that one has to do this. */
typedef struct PROCESS_BASIC_INFORMATION__
{
    LONG  ExitStatus;
    PVOID PebBaseAddress;
    ULONG AffinityMask;
    LONG  BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION_;
typedef LONG (__stdcall * NtQueryInformationProcess__)(
    HANDLE ProcessHandle,
    LONG ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength);
static NtQueryInformationProcess__ NtQueryInformationProcess_ = NULL;
static HMODULE NTDLL_ = NULL;
DWORD get_process_id( HANDLE process )
{
    PROCESS_BASIC_INFORMATION_ pinfo;
    if ( !NtQueryInformationProcess_ )
    {
        if ( ! NTDLL_ )
            NTDLL_ = GetModuleHandleA( "ntdll" );
        if ( NTDLL_ )
            NtQueryInformationProcess_
                = (NtQueryInformationProcess__)GetProcAddress( NTDLL_, "NtQueryInformationProcess" );
    }
    if ( NtQueryInformationProcess_ )
    {
        LONG r = (*NtQueryInformationProcess_)( process,
            /* ProcessBasicInformation == */ 0, &pinfo,
            sizeof( PROCESS_BASIC_INFORMATION_ ), NULL );
        return pinfo.UniqueProcessId;
    }
    return 0;
}


/*
 * Not really optimal, or efficient, but it is easier this way, and it is not
 * like we are going to be killing thousands, or even tens of processes.
 */

static void kill_process_tree( DWORD pid, HANDLE process )
{
    HANDLE process_snapshot_h = INVALID_HANDLE_VALUE;
    if ( !pid )
        pid = get_process_id( process );
    process_snapshot_h = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

    if ( INVALID_HANDLE_VALUE != process_snapshot_h )
    {
        BOOL ok = TRUE;
        PROCESSENTRY32 pinfo;
        pinfo.dwSize = sizeof( PROCESSENTRY32 );
        for (
            ok = Process32First( process_snapshot_h, &pinfo );
            ok == TRUE;
            ok = Process32Next( process_snapshot_h, &pinfo ) )
        {
            if ( pinfo.th32ParentProcessID == pid )
            {
                /* Found a child, recurse to kill it and anything else below it.
                 */
                HANDLE ph = OpenProcess( PROCESS_ALL_ACCESS, FALSE,
                    pinfo.th32ProcessID );
                if ( NULL != ph )
                {
                    kill_process_tree( pinfo.th32ProcessID, ph );
                    CloseHandle( ph );
                }
            }
        }
        CloseHandle( process_snapshot_h );
    }
    /* Now that the children are all dead, kill the root. */
    TerminateProcess( process, -2 );
}


static double creation_time( HANDLE process )
{
    FILETIME creation;
    FILETIME exit;
    FILETIME kernel;
    FILETIME user;
    FILETIME current;
    return GetProcessTimes( process, &creation, &exit, &kernel, &user )
        ? filetime_seconds( creation )
        : 0.0;
}


/*
 * Recursive check if first process is parent (directly or indirectly) of the
 * second one. Both processes are passed as process ids, not handles. Special
 * return value 2 means that the second process is smss.exe and its parent
 * process is System (first argument is ignored).
 */

static int is_parent_child( DWORD parent, DWORD child )
{
    HANDLE process_snapshot_h = INVALID_HANDLE_VALUE;

    if ( !child )
        return 0;
    if ( parent == child )
        return 1;

    process_snapshot_h = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( INVALID_HANDLE_VALUE != process_snapshot_h )
    {
        BOOL ok = TRUE;
        PROCESSENTRY32 pinfo;
        pinfo.dwSize = sizeof( PROCESSENTRY32 );
        for (
            ok = Process32First( process_snapshot_h, &pinfo );
            ok == TRUE;
            ok = Process32Next( process_snapshot_h, &pinfo ) )
        {
            if ( pinfo.th32ProcessID == child )
            {
                /* Unfortunately, process ids are not really unique. There might
                 * be spurious "parent and child" relationship match between two
                 * non-related processes if real parent process of a given
                 * process has exited (while child process kept running as an
                 * "orphan") and the process id of such parent process has been
                 * reused by internals of the operating system when creating
                 * another process.
                 *
                 * Thus additional check is needed - process creation time. This
                 * check may fail (i.e. return 0) for system processes due to
                 * insufficient privileges, and that is OK.
                 */
                double tchild = 0.0;
                double tparent = 0.0;
                HANDLE hchild = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, pinfo.th32ProcessID );
                CloseHandle( process_snapshot_h );

                /* csrss.exe may display message box like following:
                 *   xyz.exe - Unable To Locate Component
                 *   This application has failed to start because
                 *   boost_foo-bar.dll was not found. Re-installing the
                 *   application may fix the problem
                 * This actually happens when starting test process that depends
                 * on a dynamic library which failed to build. We want to
                 * automatically close these message boxes even though csrss.exe
                 * is not our child process. We may depend on the fact that (in
                 * all current versions of Windows) csrss.exe is directly child
                 * of the smss.exe process, which in turn is directly child of
                 * the System process, which always has process id == 4. This
                 * check must be performed before comparison of process creation
                 * times.
                 */
                if ( !stricmp( pinfo.szExeFile, "csrss.exe" ) &&
                    ( is_parent_child( parent, pinfo.th32ParentProcessID ) == 2 ) )
                    return 1;
                if ( !stricmp( pinfo.szExeFile, "smss.exe" ) &&
                    ( pinfo.th32ParentProcessID == 4 ) )
                    return 2;

                if ( hchild )
                {
                    HANDLE hparent = OpenProcess( PROCESS_QUERY_INFORMATION,
                        FALSE, pinfo.th32ParentProcessID );
                    if ( hparent )
                    {
                        tchild = creation_time( hchild );
                        tparent = creation_time( hparent );
                        CloseHandle( hparent );
                    }
                    CloseHandle( hchild );
                }

                /* Return 0 if one of the following is true:
                 *  1. we failed to read process creation time
                 *  2. child was created before alleged parent
                 */
                if ( ( tchild == 0.0 ) || ( tparent == 0.0 ) ||
                    ( tchild < tparent ) )
                    return 0;

                return is_parent_child( parent, pinfo.th32ParentProcessID ) & 1;
            }
        }

        CloseHandle( process_snapshot_h );
    }

    return 0;
}

typedef struct PROCESS_HANDLE_ID { HANDLE h; DWORD pid; } PROCESS_HANDLE_ID;


/*
 * This function is called by the operating system for each topmost window.
 */

BOOL CALLBACK close_alert_window_enum( HWND hwnd, LPARAM lParam )
{
    char buf[ 7 ] = { 0 };
    PROCESS_HANDLE_ID p = *( (PROCESS_HANDLE_ID *)lParam );
    DWORD pid = 0;
    DWORD tid = 0;

    /* We want to find and close any window that:
     *  1. is visible and
     *  2. is a dialog and
     *  3. is displayed by any of our child processes
     */
    if ( !IsWindowVisible( hwnd ) )
        return TRUE;

    if ( !GetClassNameA( hwnd, buf, sizeof( buf ) ) )
        return TRUE;  /* Failed to read class name; presume it is not a dialog. */

    if ( strcmp( buf, "#32770" ) )
        return TRUE;  /* Not a dialog */

    /* GetWindowThreadProcessId() returns 0 on error, otherwise thread id of
     * window message pump thread.
     */
    tid = GetWindowThreadProcessId( hwnd, &pid );

    if ( tid && is_parent_child( p.pid, pid ) )
    {
        /* Ask really nice. */
        PostMessageA( hwnd, WM_CLOSE, 0, 0 );
        /* Now wait and see if it worked. If not, insist. */
        if ( WaitForSingleObject( p.h, 200 ) == WAIT_TIMEOUT )
        {
            PostThreadMessageA( tid, WM_QUIT, 0, 0 );
            WaitForSingleObject( p.h, 300 );
        }

        /* Done, we do not want to check any other window now. */
        return FALSE;
    }

    return TRUE;
}


static void close_alert( HANDLE process )
{
    DWORD pid = get_process_id( process );
    /* If process already exited or we just can not get its process id, do not
     * go any further.
     */
    if ( pid )
    {
        PROCESS_HANDLE_ID p;
        p.h = process;
        p.pid = pid;
        EnumWindows( &close_alert_window_enum, (LPARAM)&p );
    }
}


void exec_done( void )
{
}


#endif /* USE_EXECNT */
