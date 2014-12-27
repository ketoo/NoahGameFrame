# include "jam.h"
# include "pathsys.h"
# include "strings.h"
# include "object.h"
# include "filesys.h"
# include "lists.h"

void file_build1( PATHNAME * f, string * file )
{
    if ( DEBUG_SEARCH )
    {
        printf("build file: ");
        if ( f->f_root.len )
            printf( "root = '%.*s' ", f->f_root.len, f->f_root.ptr );
        if ( f->f_dir.len )
            printf( "dir = '%.*s' ", f->f_dir.len, f->f_dir.ptr );
        if ( f->f_base.len )
            printf( "base = '%.*s' ", f->f_base.len, f->f_base.ptr );
        printf( "\n" );
    }

    /* Start with the grist.  If the current grist isn't */
    /* surrounded by <>'s, add them. */

    if ( f->f_grist.len )
    {
        if ( f->f_grist.ptr[0] != '<' )
            string_push_back( file, '<' );
        string_append_range(
            file, f->f_grist.ptr, f->f_grist.ptr + f->f_grist.len );
        if ( file->value[file->size - 1] != '>' )
            string_push_back( file, '>' );
    }
}

static struct hash * filecache_hash = 0;
static file_info_t filecache_finfo;

file_info_t * file_info( OBJECT * filename )
{
    file_info_t *finfo = &filecache_finfo;
    int found;

    if ( !filecache_hash )
        filecache_hash = hashinit( sizeof( file_info_t ), "file_info" );

    filename = path_as_key( filename );

    finfo = (file_info_t *)hash_insert( filecache_hash, filename, &found );
    if ( !found )
    {
        /* printf( "file_info: %s\n", filename ); */
        finfo->name = object_copy( filename );
        finfo->is_file = 0;
        finfo->is_dir = 0;
        finfo->size = 0;
        finfo->time = 0;
        finfo->files = L0;
    }

    object_free( filename );

    return finfo;
}

static LIST * files_to_remove = L0;

static void remove_files_atexit(void)
{
    LISTITER iter = list_begin( files_to_remove ), end = list_end( files_to_remove );
    for ( ; iter != end; iter = list_next( iter ) )
    {
        remove( object_str( list_item( iter ) ) );
    }
    list_free( files_to_remove );
    files_to_remove = L0;
}

static void free_file_info ( void * xfile, void * data )
{
    file_info_t * file = (file_info_t *)xfile;
    object_free( file->name );
    list_free( file->files );
}

void file_done()
{
    remove_files_atexit();
    if ( filecache_hash )
    {
        hashenumerate( filecache_hash, free_file_info, (void *)0 );
        hashdone( filecache_hash );
    }
}

void file_remove_atexit( OBJECT * path )
{
    files_to_remove = list_push_back( files_to_remove, object_copy( path ) );
}
