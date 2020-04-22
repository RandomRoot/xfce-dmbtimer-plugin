#include <libxfce4panel/libxfce4panel.h>
#include <libxfce4ui/libxfce4ui.h>
#include <string.h>


#ifndef __COUNTER_H__
#define __COUNTER_H__
G_BEGIN_DECLS

typedef struct
{
    int d, m, y;
} Date;


typedef struct
{
    XfcePanelPlugin *plugin;

    /* panel widgets */
    GtkWidget *ebox;
    GtkWidget *hvbox;
    GtkWidget *label;
    GtkWidget *monthLabel;

    GtkWidget *config_dialog;

    /* option widgets */
    GtkWidget *st_date_btn;
    GtkWidget *fin_date_btn;
    GtkWidget *calendar_wnd;
    GtkWidget *calendar;

    gint date_clk_type;
    gboolean initialized;

    /* Config settings */
    Date dmb_date;
    Date start_date;

    Date selected_date;
}
  DmbPlugin;

G_END_DECLS

#endif /* !__COUNTER_H__ */