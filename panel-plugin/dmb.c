#include "dmb_shared.h"
#include "dmb_dialogs.h"
#include "dmb_configure.h"
#include "dmb_renew.h"
#include <time.h>


static DmbPlugin *dmb_plugin;

static DmbPlugin *
plugin_new(XfcePanelPlugin *plugin, GtkWidget *button)
{
    DmbPlugin *sample;

    sample = g_slice_new0(DmbPlugin);
    sample->plugin = plugin;

    plugin_read(sample);

    sample->hvbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(button), sample->hvbox);

    sample->monthLabel = gtk_label_new("");
    gtk_label_set_use_markup(GTK_LABEL(sample->monthLabel), TRUE);
    gtk_box_pack_start(GTK_BOX (sample->hvbox), sample->monthLabel, FALSE, FALSE, 5);

    sample->label = gtk_label_new("");
    gtk_label_set_use_markup(GTK_LABEL(sample->label), TRUE);
    gtk_box_pack_start(GTK_BOX (sample->hvbox), sample->label, FALSE, FALSE, 5);

    return sample;
}

static void
plugin_free(XfcePanelPlugin *plugin,
            DmbPlugin *sample)
{
    gtk_widget_destroy(sample->hvbox);
    g_slice_free (DmbPlugin, sample);
}

static gboolean
plugin_size_changed(XfcePanelPlugin *plugin,
                    gint size,
                    DmbPlugin *sample)
{
    GtkOrientation orientation;

    /* get the orientation of the plugin */
    orientation = xfce_panel_plugin_get_orientation(plugin);

    /* set the widget size */
    if (orientation == GTK_ORIENTATION_HORIZONTAL)
        gtk_widget_set_size_request(GTK_WIDGET (plugin), -1, size);
    else
        gtk_widget_set_size_request(GTK_WIDGET (plugin), size, -1);

    /* we handled the orientation */
    return TRUE;
}

static void
dmb_constructor(XfcePanelPlugin *plugin)
{
    GtkWidget *button = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(plugin), button);
    xfce_panel_plugin_add_action_widget(plugin, button);
    gtk_widget_show(button);

    dmb_plugin = plugin_new(plugin, button);
    gtk_widget_show_all(dmb_plugin->hvbox);

    g_signal_connect (G_OBJECT(plugin), "free-data",
                      G_CALLBACK(plugin_free), dmb_plugin);

    /* show the configure menu item and connect signal */
    xfce_panel_plugin_menu_show_configure(plugin);
    g_signal_connect (G_OBJECT(plugin), "configure-plugin",
                      G_CALLBACK(plugin_configure), dmb_plugin);

    /* show the about menu item and connect signal */
    xfce_panel_plugin_menu_show_about(plugin);
    g_signal_connect (G_OBJECT(plugin), "about",
                      G_CALLBACK(plugin_about), NULL);

    g_signal_connect (G_OBJECT(plugin), "size-changed",
                      G_CALLBACK(plugin_size_changed), dmb_plugin);

    g_signal_connect(G_OBJECT(plugin), "save",
                     G_CALLBACK(plugin_save), dmb_plugin);

    renew_months(dmb_plugin, renew_counter(dmb_plugin));
}

XFCE_PANEL_PLUGIN_REGISTER(dmb_constructor);