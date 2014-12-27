/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "object.h"
# include "lists.h"
# include "assert.h"

/*
 * lists.c - maintain lists of objects
 *
 * 08/23/94 (seiwald) - new list_append()
 * 09/07/00 (seiwald) - documented lol_*() functions
 */

struct freelist_node { struct freelist_node *next; };

static struct freelist_node *freelist[32];  /* junkpile for list_free() */

static unsigned get_bucket( unsigned size )
{
    unsigned bucket = 0;
    while ( size > ( 1u << bucket ) ) ++bucket;
    return bucket;
}

static LIST * list_alloc( unsigned size )
{
    unsigned bucket = get_bucket( size );
    if ( freelist[ bucket ] )
    {
        struct freelist_node * result = freelist[ bucket ];
        freelist[ bucket ] = result->next;
        return (LIST *)result;
    }
    else
    {
        return (LIST *)BJAM_MALLOC( sizeof( LIST ) + ( 1u << bucket ) * sizeof( OBJECT * ) );
    }
}

static void list_dealloc( LIST * l )
{
    unsigned size = list_length( l );
    unsigned bucket;
    struct freelist_node * node = (struct freelist_node *)l;

    if ( size == 0 ) return;

    bucket = get_bucket( size );;

#ifdef BJAM_NO_MEM_CACHE

    BJAM_FREE( node );

#else

    node->next = freelist[ bucket ];
    freelist[ bucket ] = node;

#endif

}

/*
 * list_append() - append a list onto another one, returning total
 */

LIST * list_append( LIST * l, LIST * nl )
{
    if ( list_empty( nl ) )
    {
        /* Just return l */
    }
    else if ( list_empty( l ) )
    {
        l = nl;
    }
    else
    {
        int l_size = list_length( l );
        int nl_size = list_length( nl );
        int size = l_size + nl_size;
        unsigned bucket;
        int i;

        bucket = get_bucket( size );
        /* Do we need to reallocate? */
        if ( l_size <= ( 1u << (bucket - 1) ) )
        {
            LIST * result = list_alloc( size );
            memcpy( list_begin( result ), list_begin( l ), l_size * sizeof( OBJECT * ) );
            list_dealloc( l );
            l = result;
        }

        l->impl.size = size;
        memcpy( list_begin( l ) + l_size, list_begin( nl ), nl_size *  sizeof( OBJECT * ) );
        list_dealloc( nl );
        return l;
    }

    return l;
}

LISTITER list_begin( LIST * l )
{
    if ( l )
        return (LISTITER)( (char *)l + sizeof(LIST) );
    else
        return 0;
}

LISTITER list_end( LIST * l )
{
    if ( l )
        return list_begin( l ) + l->impl.size;
    else
        return 0;
}

LIST * list_new( OBJECT * value )
{   
    LIST * head;
    if ( freelist[ 0 ] )
    {
        struct freelist_node * result = freelist[ 0 ];
        freelist[ 0 ] = result->next;
        head = (LIST *)result;
    }
    else
    {
        head = BJAM_MALLOC( sizeof( LIST * ) + sizeof( OBJECT * ) );
    }

    head->impl.size = 1;
    list_begin( head )[ 0 ] = value;

    return head;
}

/*
 * list_push_back() - tack a string onto the end of a list of strings
 */

LIST * list_push_back( LIST * head, OBJECT * value )
{
    unsigned int size = list_length( head );
    unsigned int i;

    if ( DEBUG_LISTS )
        printf( "list > %s <\n", object_str( value ) );

    /* If the size is a power of 2, reallocate. */
    if ( size == 0 )
    {
        head = list_alloc( 1 );
    }
    else if ( ( ( size - 1 ) & size ) == 0 )
    {
        LIST * l = list_alloc( size + 1 );
        memcpy( l, head, sizeof( LIST ) + size * sizeof( OBJECT * ) );
        list_dealloc( head );
        head = l;
    }

    list_begin( head )[ size ] = value;
    head->impl.size = size + 1;

    return head;
}


/*
 * list_copy() - copy a whole list of strings (nl) onto end of another (l).
 */

LIST * list_copy( LIST * l )
{
    int size = list_length( l );
    int i;
    LIST * result;

    if ( size == 0 ) return L0;

    result = list_alloc( size );
    result->impl.size = size;
    for ( i = 0; i < size; ++i )
    {
        list_begin( result )[ i ] = object_copy( list_begin( l )[ i ] );
    }
    return result;
}


LIST * list_copy_range( LIST *l, LISTITER first, LISTITER last )
{
    if ( first == last )
    {
        return L0;
    }
    else
    {
        int size = last - first;
        LIST * result = list_alloc( size );
        LISTITER dest = list_begin( result );
        result->impl.size = size;
        for ( ; first != last; ++first, ++dest )
        {
            *dest = object_copy( *first );
        }
        return result;
    }
}


/*
 * list_sublist() - copy a subset of a list of strings.
 */

LIST * list_sublist( LIST * l, int start, int count )
{
    int end = start + count;
    int size = list_length( l );
    if ( start >= size ) return L0;
    if ( end > size ) end = size;
    return list_copy_range( l, list_begin( l ) + start, list_begin( l ) + end );
}


static int str_ptr_compare( void const * va, void const * vb )
{
    OBJECT * a = *( (OBJECT * *)va );
    OBJECT * b = *( (OBJECT * *)vb );
    return strcmp(object_str(a), object_str(b));
}


LIST * list_sort( LIST * l )
{
    int len;
    int ii;
    LIST * result;

    if ( !l )
        return L0;

    len = list_length( l );
    result = list_copy( l );

    qsort( list_begin( result ), len, sizeof( OBJECT * ), str_ptr_compare );

    return result;
}


/*
 * list_free() - free a list of strings
 */

void list_free( LIST * head )
{
    if ( !list_empty( head ) )
    {
        LISTITER iter = list_begin( head ), end = list_end( head );
        for ( ; iter != end; iter = list_next( iter ) )
        {
            object_free( list_item( iter ) );
        }
        list_dealloc( head );
    }
}


/*
 * list_pop_front() - remove the front element from a list of strings
 */

LIST * list_pop_front( LIST * l )
{
    unsigned size = list_length( l );
    assert( size != 0 );
    --size;
    object_free( list_front( l ) );

    if ( size == 0 )
    {
        list_dealloc( l );
        return L0;
    }
    else if ( ( ( size - 1 ) & size ) == 0 )
    {
        LIST * nl = list_alloc( size );
        nl->impl.size = size;
        memcpy( list_begin( nl ), list_begin( l ) + 1, size * sizeof( OBJECT * ) );
        list_dealloc( l );
        return nl;
    }
    else
    {
        l->impl.size = size;
        memmove( list_begin( l ), list_begin( l ) + 1, size * sizeof( OBJECT * ) );
        return l;
    }
}

LIST *  list_reverse( LIST * l )
{
    int size = list_length( l );
    if ( size == 0 ) return L0;
    else
    {
        LIST * result = list_alloc( size );
        int i;
        result->impl.size = size;
        for ( i = 0; i < size; ++i )
        {
            list_begin( result )[ i ] = object_copy( list_begin( l )[ size - i - 1 ] );
        }
        return result;
    }
}

int list_cmp( LIST * t, LIST * s )
{
    int status = 0;
    LISTITER t_it = list_begin( t ), t_end = list_end( t );
    LISTITER s_it = list_begin( s ), s_end = list_end( s );

    while ( !status && ( t_it != t_end || s_it != s_end ) )
    {
        const char *st = t_it != t_end ? object_str( list_item( t_it ) ) : "";
        const char *ss = s_it != s_end ? object_str( list_item( s_it ) ) : "";

        status = strcmp( st, ss );

        t_it = t_it != t_end ? list_next( t_it ) : t_it;
        s_it = s_it != s_end ? list_next( s_it ) : s_it;
    }

    return status;
}

int list_is_sublist( LIST * sub, LIST * l )
{
    LISTITER iter = list_begin( sub ), end = list_end( sub );
    for ( ; iter != end; iter = list_next( iter ) )
    {
        if ( !list_in( l, list_item( iter ) ) )
            return 0;
    }
    return 1;
}

/*
 * list_print() - print a list of strings to stdout
 */

void list_print( LIST * l )
{
    LISTITER iter = list_begin( l ), end = list_end( l );
    if ( iter != end )
    {
        printf( "%s", object_str( list_item( iter ) ) );
        iter = list_next( iter );
        for ( ; iter != end; iter = list_next( iter ) )
            printf( " %s", object_str( list_item( iter ) ) );
    }
}


/*
 * list_length() - return the number of items in the list
 */

int list_length( LIST * l )
{
    if ( l )
        return l->impl.size;
    else
        return 0;
}


int list_in( LIST * l, OBJECT * value )
{
    LISTITER iter = list_begin( l ), end = list_end( l );
    for ( ; iter != end; iter = list_next( iter ) )
        if ( object_equal( list_item( iter ), value ) )
            return 1;
    return 0;
}


LIST * list_unique( LIST * sorted_list )
{
    LIST * result = L0;
    OBJECT * last_added = 0;

    LISTITER iter = list_begin( sorted_list ), end = list_end( sorted_list );
    for ( ; iter != end; iter = list_next( iter ) )
    {
        if ( !last_added || !object_equal( list_item( iter ), last_added ) )
        {
            result = list_push_back( result, object_copy( list_item( iter ) ) );
            last_added = list_item( iter );
        }
    }
    return result;
}

void list_done()
{
    int i;
    int total = 0;
    for ( i = 0; i < sizeof( freelist ) / sizeof( freelist[ 0 ] ); ++i )
    {
        struct freelist_node *l, *tmp;
        int bytes;
        for( l = freelist[ i ]; l;  )
        {
            tmp = l;
            l = l->next;
            BJAM_FREE( tmp );
        }
    }
}


/*
 * lol_init() - initialize a LOL (list of lists).
 */

void lol_init( LOL * lol )
{
    lol->count = 0;
}


/*
 * lol_add() - append a LIST onto an LOL.
 */

void lol_add( LOL * lol, LIST * l )
{
    if ( lol->count < LOL_MAX )
        lol->list[ lol->count++ ] = l;
}


/*
 * lol_free() - free the LOL and its LISTs.
 */

void lol_free( LOL * lol )
{
    int i;
    for ( i = 0; i < lol->count; ++i )
        list_free( lol->list[ i ] );
    lol->count = 0;
}


/*
 * lol_get() - return one of the LISTs in the LOL.
 */

LIST * lol_get( LOL * lol, int i )
{
    return i < lol->count ? lol->list[ i ] : L0;
}


/*
 * lol_print() - debug print LISTS separated by ":".
 */

void lol_print( LOL * lol )
{
    int i;

    for ( i = 0; i < lol->count; ++i )
    {
        if ( i )
            printf( " : " );
        list_print( lol->list[ i ] );
    }
}

#ifdef HAVE_PYTHON

PyObject *list_to_python(LIST *l)
{
    PyObject *result = PyList_New(0);
    LISTITER iter = list_begin( l ), end = list_end( l );

    for (; iter != end; iter = list_next( iter ) )
    {
        PyObject* s = PyString_FromString(object_str(list_item(iter)));
        PyList_Append(result, s);
        Py_DECREF(s);
    }

    return result;
}

LIST *list_from_python(PyObject *l)
{
    LIST * result = L0;

    Py_ssize_t i, n;
    n = PySequence_Size(l);
    for (i = 0; i < n; ++i)
    {
        PyObject *v = PySequence_GetItem(l, i);        
        result = list_push_back(result, object_new (PyString_AsString(v)));
        Py_DECREF(v);
    }

    return result;
}

#endif
