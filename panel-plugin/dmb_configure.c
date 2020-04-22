#include "dmb_configure.h"
#include "dmb_renew.h"


void
plugin_save(XfcePanelPlugin *plugin, DmbPlugin *data)
{
    XfceRc *rc;
    gchar *file;

    printf("[DMB]: %s\n", "Writing config to file");

    if (!(file = xfce_panel_plugin_save_location(plugin, TRUE)))
        return;

    /* get rid of old values */
    unlink(file);
    rc = xfce_rc_simple_open(file, FALSE);
    g_free(file);

    if (!rc)
        return;

    xfce_rc_write_int_entry(rc, "dmb_y", data->dmb_date.y);
    xfce_rc_write_int_entry(rc, "dmb_m", data->dmb_date.m);
    xfce_rc_write_int_entry(rc, "dmb_d", data->dmb_date.d);

    xfce_rc_write_int_entry(rc, "start_y", data->start_date.y);
    xfce_rc_write_int_entry(rc, "start_m", data->start_date.m);
    xfce_rc_write_int_entry(rc, "start_d", data->start_date.d);

    xfce_rc_close(rc);

    renew_months(data, renew_counter(data));
}

void
plugin_read(DmbPlugin *d_plugin)
{
    XfceRc *rc;
    gchar *file;

    file = xfce_panel_plugin_lookup_rc_file(d_plugin->plugin);

    if (G_LIKELY(file != NULL)) {
        printf("[DMB]: %s\n", "Reading config from file");

//        unlink(file);
        rc = xfce_rc_simple_open(file, TRUE);
        g_free(file);

        if (!rc)
            return;

        const Date dm_date = {
          .y = xfce_rc_read_int_entry(rc, "dmb_y", 0),
          .m = xfce_rc_read_int_entry(rc, "dmb_m", 0),
          .d = xfce_rc_read_int_entry(rc, "dmb_d", 0)
        };

        d_plugin->dmb_date = dm_date;

        const Date st_date = {
          .y = xfce_rc_read_int_entry(rc, "start_y", 0),
          .m = xfce_rc_read_int_entry(rc, "start_m", 0),
          .d = xfce_rc_read_int_entry(rc, "start_d", 0)
        };

        d_plugin->start_date = st_date;

        xfce_rc_close(rc);
        return;
    }

    printf("[DMB]: %s\n", "Applying default settings");
    d_plugin->start_date = blank_date;
    d_plugin->dmb_date = blank_date;
}