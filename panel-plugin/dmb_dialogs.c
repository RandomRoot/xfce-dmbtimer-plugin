#include "dmb_dialogs.h"
#include "dmb_configure.h"
#include "dmb_help_func.h"


static const char *months[12] = {
  "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"
};

void renew_date_on_buttons(DmbPlugin *dmb_plugin);

static void response_cb(GtkWidget *dlg, gint response, DmbPlugin *sample)
{
    gtk_widget_destroy(dlg);
}

static void on_calendar_selected(GtkWidget *widget, DmbPlugin *dmb_plugin)
{
    gtk_calendar_get_date(GTK_CALENDAR (dmb_plugin->calendar),
                          (guint *) &dmb_plugin->selected_date.y, (guint *) &dmb_plugin->selected_date.m,
                          (guint *) &dmb_plugin->selected_date.d);
    dmb_plugin->selected_date.m++;
}

static gboolean close_calendar_window(DmbPlugin *dmb_plugin)
{
    gtk_widget_destroy(dmb_plugin->calendar_wnd);
    dmb_plugin->calendar_wnd = NULL;
    dmb_plugin->calendar = NULL;

    return TRUE;
}

static void
summary_dialog_response(const GtkWidget *dlg,
                        const gint response,
                        DmbPlugin *dmb_plugin)
{
    if (response == GTK_RESPONSE_ACCEPT) {
        if (dmb_plugin->date_clk_type) {
            dmb_plugin->dmb_date = dmb_plugin->selected_date;
        } else {
            dmb_plugin->start_date = dmb_plugin->selected_date;
        }
        renew_date_on_buttons(dmb_plugin);
        plugin_save(dmb_plugin->plugin, dmb_plugin);
    }

    close_calendar_window(dmb_plugin);
}


static GtkWidget *pop_calendar_window(DmbPlugin *dmb_plugin)
{
    GtkWidget *window, *vbox;
    GtkWidget *parent = dmb_plugin->config_dialog;
    GdkScreen *screen;
    GtkCalendarDisplayOptions display_options;

    window = gtk_dialog_new_with_buttons("Выберите дату",
                                         GTK_WINDOW(
                                           gtk_widget_get_toplevel(GTK_WIDGET(dmb_plugin->plugin))),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         _("Cancel"),
                                         GTK_RESPONSE_REJECT,
                                         _("OK"),
                                         GTK_RESPONSE_ACCEPT,
                                         NULL
    );

    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window), TRUE);
    gtk_window_set_skip_pager_hint(GTK_WINDOW(window), TRUE);
    g_object_set_data(G_OBJECT(window), "calendar-parent", parent);

    /* set screen number */
    screen = gtk_widget_get_screen(parent);
    gtk_window_set_screen(GTK_WINDOW(window), screen);

    dmb_plugin->calendar = gtk_calendar_new();
    display_options = GTK_CALENDAR_SHOW_HEADING | GTK_CALENDAR_SHOW_DAY_NAMES;
    gtk_calendar_set_display_options(GTK_CALENDAR (dmb_plugin->calendar), display_options);

    Date to_select_date = dmb_plugin->initialized ?
                          dmb_plugin->date_clk_type == 0 ? dmb_plugin->start_date : dmb_plugin->dmb_date
                                                  : get_current_date();

    gtk_calendar_select_month(GTK_CALENDAR(dmb_plugin->calendar), to_select_date.m - 1, to_select_date.y);
    gtk_calendar_select_day(GTK_CALENDAR(dmb_plugin->calendar), to_select_date.d);

    vbox = gtk_dialog_get_content_area(GTK_DIALOG(window));
    gtk_box_pack_start(GTK_BOX(vbox), dmb_plugin->calendar, TRUE, TRUE, 0);


    g_signal_connect(GTK_CALENDAR(dmb_plugin->calendar), "day_selected",
                     G_CALLBACK(on_calendar_selected),
                     dmb_plugin);
    g_signal_connect_swapped(G_OBJECT(window), "delete-event",
                             G_CALLBACK(close_calendar_window),
                             dmb_plugin);
    g_signal_connect_swapped(G_OBJECT(window), "focus-out-event",
                             G_CALLBACK(close_calendar_window),
                             dmb_plugin);

    g_signal_connect(G_OBJECT(window), "response",
                     G_CALLBACK(summary_dialog_response), dmb_plugin);

    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_MENU);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_widget_show_all(window);

    return window;
}

static gboolean dt_clicked(GtkWidget *widget, GdkEventButton *event, DmbPlugin *dmb_plugin)
{
    if (event->button != 1 || event->state & GDK_CONTROL_MASK)
        return FALSE;

    if (dmb_plugin == NULL)
        return FALSE;

    if (dmb_plugin->calendar_wnd != NULL) {
        close_calendar_window(dmb_plugin);
    } else {
        /* draw calendar */
        dmb_plugin->calendar_wnd = pop_calendar_window(dmb_plugin);
    }
    return TRUE;
}

static gboolean st_dt_clicked(GtkWidget *widget, GdkEventButton *event, DmbPlugin *dmb_plugin)
{
    dmb_plugin->date_clk_type = 0;
    return dt_clicked(widget, event, dmb_plugin);
}

static gboolean fi_dt_clicked(GtkWidget *widget, GdkEventButton *event, DmbPlugin *dmb_plugin)
{
    dmb_plugin->date_clk_type = 1;
    return dt_clicked(widget, event, dmb_plugin);
}

void set_label_text_on_date_buttons(GtkButton *button, Date date)
{
    char *date_text =
      date.y != 0 ? g_markup_printf_escaped("%02d %s %02d", date.d, months[date.m - 1], date.y)
                  : g_markup_printf_escaped("%s", "???");

    gtk_button_set_label(button, date_text);
    g_free(date_text);
}

void
renew_date_on_buttons(DmbPlugin *dmb_plugin)
{
    set_label_text_on_date_buttons(GTK_BUTTON(dmb_plugin->st_date_btn), dmb_plugin->start_date);
    set_label_text_on_date_buttons(GTK_BUTTON(dmb_plugin->fin_date_btn), dmb_plugin->dmb_date);
}

void add_new_date_button_with_text(DmbPlugin *dmb_plugin, GtkWidget *grid, GtkWidget *button, char *left_text, gint row)
{
    GtkWidget *label = gtk_label_new("");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_label_set_markup(GTK_LABEL(label), g_markup_printf_escaped("<span><b>%s</b></span>", left_text));
    gtk_grid_attach(GTK_GRID (grid), label, 0, row, 1, 1);
    g_object_set(label, "margin", 5, NULL);
    gtk_grid_attach(GTK_GRID (grid), button, 1, row, 1, 1);
    gtk_widget_set_hexpand(button, TRUE);
    g_object_set(button, "margin", 5, NULL);
}


void
plugin_configure(XfcePanelPlugin *plugin, DmbPlugin *dmb_plugin)
{
    GtkWidget *dialog_vbox;

    dmb_plugin->config_dialog = xfce_titled_dialog_new_with_buttons("Настройка даты ДМБ",
                                                                    GTK_WINDOW(
                                                                      gtk_widget_get_toplevel(GTK_WIDGET(plugin))),
                                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                    _("_Close"),
                                                                    GTK_RESPONSE_OK,
                                                                    NULL
    );
    gtk_window_set_icon_name(GTK_WINDOW(dmb_plugin->config_dialog), "security-low");
    dialog_vbox = gtk_dialog_get_content_area(GTK_DIALOG(dmb_plugin->config_dialog));

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER (dialog_vbox), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX (vbox), grid, TRUE, TRUE, 5);

    /* Поле ввода даты начала службы */

    dmb_plugin->st_date_btn = gtk_button_new();
    add_new_date_button_with_text(dmb_plugin, grid, dmb_plugin->st_date_btn, "Начало службы:", 0);
    g_signal_connect(dmb_plugin->st_date_btn, "button-press-event",
                     G_CALLBACK(st_dt_clicked), dmb_plugin);

    /* Поле ввода даты окончания службы */

    dmb_plugin->fin_date_btn = gtk_button_new();
    add_new_date_button_with_text(dmb_plugin, grid, dmb_plugin->fin_date_btn, "Окончание службы:", 1);
    g_signal_connect(dmb_plugin->fin_date_btn, "button-press-event",
                     G_CALLBACK(fi_dt_clicked), dmb_plugin);

    /* ================= */

    g_signal_connect(dmb_plugin->config_dialog, "response", G_CALLBACK(response_cb), dmb_plugin);

    gtk_container_set_border_width(GTK_CONTAINER(dmb_plugin->config_dialog), 2);
    gtk_widget_set_size_request(dmb_plugin->config_dialog, 280, 200);

    gtk_widget_show_all(dmb_plugin->config_dialog);
    renew_date_on_buttons(dmb_plugin);
}


void
plugin_about(XfcePanelPlugin *plugin)
{
    GdkPixbuf *icon;
    const gchar *auth[] = {
      "Mikhail Prokofev <mprokofev@divcu.com>",
      NULL
    };
    icon = xfce_panel_pixbuf_from_source("security-low", NULL, 48);

    gtk_show_about_dialog
      (NULL,
       "logo", icon,
       "license", xfce_get_license_text(XFCE_LICENSE_TEXT_GPL),
       "version", PACKAGE_VERSION,
       "program-name", PACKAGE_NAME,
       "comments", _("Показывает время, оставшееся до конца службы в армии"),
       "copyright", _("Copyright (c) 2019-2020\n"),
       "authors", auth,
       NULL);
}