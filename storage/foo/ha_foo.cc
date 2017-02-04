#include <mysql/plugin.h>
#include <mysql/psi/mysql_file.h>
#include "ha_foo.h"

#define CSV_EXT ".CSV"
#define CSM_EXT ".CSM"

static handler *foo_create_handler(handlerton *hton,
                                   TABLE_SHARE *table,
                                   MEM_ROOT *mem_root)
{
    return new (mem_root) ha_foo(hton, table);
}

static int foo_init_func(void *p)
{
    handlerton *foo_hton = (handlerton *) p;
    foo_hton->state     = SHOW_OPTION_YES;
    foo_hton->create    = foo_create_handler;
    foo_hton->flags     = HTON_CAN_RECREATE;

    return 0;
}

#ifdef HAVE_PSI_INTERFACE

static PSI_file_key csv_key_file_metadata, csv_key_file_data;

#endif /* HAVE_PSI_INTERFACE */

ha_foo::ha_foo(handlerton *hton, TABLE_SHARE *table_arg)
    :handler(hton, table_arg)
{}

static const char *ha_foo_exts[] = {
    CSV_EXT,
    CSM_EXT,
    NullS
};

const char **ha_foo::bas_ext() const
{
    return ha_foo_exts;
}

int ha_foo::open(const char *name, int mode, uint open_flags)
{
    DBUG_ENTER("ha_foo::open");
    DBUG_RETURN(0);
}

int ha_foo::close(void)
{
    DBUG_ENTER("ha_foo::close");
    DBUG_RETURN(0);
}

int ha_foo::rnd_init(bool scan)
{
    DBUG_ENTER("ha_foo::rnd_init");
    DBUG_RETURN(0);
}

int ha_foo::rnd_next(uchar *buf)
{
    DBUG_ENTER("ha_foo::rnd_next");
    DBUG_RETURN(HA_ERR_END_OF_FILE);
}

int ha_foo::rnd_pos(uchar *buf, uchar *pos)
{
    DBUG_ENTER("ha_foo::rnd_pos");
    DBUG_RETURN(0);
}

void ha_foo::position(const uchar *record)
{
    DBUG_ENTER("ha_foo::position");
    DBUG_VOID_RETURN;
}

int ha_foo::info(uint flags)
{
    DBUG_ENTER("ha_foo::info");
    DBUG_RETURN(0);
}

int ha_foo::create(const char *name, TABLE *table,
                   HA_CREATE_INFO *info)
{
    char name_buff[FN_REFLEN];
    File create_file;
    DBUG_ENTER("ha_foo::create");

    if ((create_file = mysql_file_create(csv_key_file_metadata,
                                         fn_format(name_buff, name, "", CSM_EXT,
                                                   MY_REPLACE_EXT|MY_UNPACK_FILENAME),
                                         0, O_RDWR | O_TRUNC, MYF(MY_WME))) < 0)
        DBUG_RETURN(-1);

    mysql_file_close(create_file, MYF(0));

    if ((create_file = mysql_file_create(csv_key_file_data,
                                         fn_format(name_buff, name, "", CSV_EXT,
                                                   MY_REPLACE_EXT|MY_UNPACK_FILENAME),
                                         0, O_RDWR | O_TRUNC, MYF(MY_WME))) < 0)
        DBUG_RETURN(-1);

    mysql_file_close(create_file, MYF(0));

    DBUG_RETURN(0);
}

THR_LOCK_DATA **ha_foo::store_lock(THD *thd,
                                   THR_LOCK_DATA **to,
                                   enum thr_lock_type lock_type)
{
    return to;
}

struct st_mysql_storage_engine foo_storage_engine =
{ MYSQL_HANDLERTON_INTERFACE_VERSION };

mysql_declare_plugin(foo)
{
    MYSQL_STORAGE_ENGINE_PLUGIN,
    &foo_storage_engine,
    "Foo",
    "plding",
    "Foo storage engine",
    PLUGIN_LICENSE_GPL,
    foo_init_func,
    NULL,
    0x0001,
    NULL,
    NULL,
    NULL,
    0,
}
mysql_declare_plugin_end;
