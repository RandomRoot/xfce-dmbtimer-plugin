#include "dmb_renew.h"
#include <math.h>


static char *
construct_text(DmbPlugin *dmb_plugin)
{
    const gint year_length = getDifference(dmb_plugin->start_date, dmb_plugin->dmb_date);
    const gint start_after_today = getDifference(dmb_plugin->start_date, get_current_date());
    const gint dmb_after_today = getDifference(get_current_date(), dmb_plugin->dmb_date);

    const gboolean not_init = dmb_plugin->start_date.y == 0 || dmb_plugin->dmb_date.y == 0;

    if (!not_init && year_length >= 0 && start_after_today >= 0 && dmb_after_today > 0) {

        const gint days_done = getDifference(dmb_plugin->start_date, get_current_date());
        const gint days_left = getDifference(get_current_date(), dmb_plugin->dmb_date);
        const gdouble week_left = getLeftWeeks(dmb_plugin->dmb_date);
        const gdouble percentage = 100 - (days_left * 1.0 / year_length) * 100;
        return g_markup_printf_escaped(
          "<span size=\"large\" foreground=\"#FD7C6E\">★ </span> "
          "<span >%s<b>%d</b> %s (<b>%.0f W</b>)</span> "
          "<span size=\"large\" foreground=\"#FD7C6E\"> ★ </span> "
          "<span >%s<b>%d</b> %s (<b>%.2f %%</b>)</span> ",
          "осталось: ", days_left, get_day_word_with_ending(days_left), week_left,
          "прошло: ", days_done, get_day_word_with_ending(days_done), percentage);
    } else {
        return NULL;
    }
}

void
renew_months(DmbPlugin *dmb_plugin, gboolean counter_result)
{
    dmb_plugin->initialized = counter_result;

    Date cur_date = get_current_date();
    const gint year_length = getDifference(dmb_plugin->start_date, dmb_plugin->dmb_date);
    const gint max_months = ceil(year_length / 31.0);
    char *markup;

    if (counter_result) {
        GString *next_months = g_string_new("");
        GString *previous_months = g_string_new("");

        const int month_diff = (dmb_plugin->dmb_date.y - dmb_plugin->start_date.y) * 12;

        int i = 1;
        int check_condition;

        if (cur_date.m >= 1 && cur_date.m <= dmb_plugin->start_date.m) {
            check_condition = month_diff + cur_date.m - dmb_plugin->start_date.m - 1;
        } else {
            check_condition = (month_diff - 12) + cur_date.m - dmb_plugin->start_date.m - 1;
        }

        for (; i <= check_condition; i++) {
            g_string_append_printf(previous_months, "%d ", i);
        }
        if (cur_date.d > dmb_plugin->start_date.d) {
            g_string_append_printf(previous_months, "%d ", i);
            i++;
        }

        int cur_month = i++;

        for (; i <= max_months; i++) {
            g_string_append_printf(next_months, " %d", i);
        }

        // char *previous, *current, *next;
        markup = g_markup_printf_escaped(
          "<span><span foreground=\"gold\">%s</span> <span  foreground=\"#FD7C6E\"><b>%d</b></span> "
          "<span foreground=\"gray\">%s</span></span>", previous_months->str, cur_month, next_months->str);
    } else {
        markup = g_markup_printf_escaped("%s", "");
    }
    gtk_label_set_markup(GTK_LABEL(dmb_plugin->monthLabel), markup);
    g_free(markup);
}


gboolean
renew_counter(DmbPlugin *dmb_plugin)
{
    char *markup = construct_text(dmb_plugin);

    if (markup == NULL) {
        markup = g_markup_printf_escaped("<span size=\"large\" foreground=\"#FD7C6E\">★ </span>"
                                         "<span><b>%s</b></span> "
                                         "<span size=\"large\" foreground=\"#FD7C6E\">★ </span> ",
                                         "Даты службы не установлены");
        gtk_label_set_markup(GTK_LABEL(dmb_plugin->label), markup);
        g_free(markup);
        return False;
    } else {
        gtk_label_set_markup(GTK_LABEL(dmb_plugin->label), markup);
        g_free(markup);
        return True;
    }
}
