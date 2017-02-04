#include "handler.h"

class ha_foo : public handler
{
public:
    ha_foo(handlerton *hton, TABLE_SHARE *share);
    ~ha_foo()
    {
    }

    const char *table_type() const { return "Foo"; }

    const char **bas_ext() const;

    Table_flags table_flags() const
    {
        return 0;
    }

    ulong index_flags(uint idx, uint part, bool all_parts) const
    {
        return 0;
    }

    int open(const char *name, int mode, uint open_flags);
    int close(void);

    int rnd_init(bool scan);
    int rnd_next(uchar *buf);
    int rnd_pos(uchar *buf, uchar *pos);
    void position(const uchar *record);
    int info(uint flags);

    int create(const char *name, TABLE *table,
               HA_CREATE_INFO *info);

    THR_LOCK_DATA **store_lock(THD *thd,
                               THR_LOCK_DATA **to,
                               enum thr_lock_type lock_type);
};
