#include "dmb_shared.h"


#ifndef __DMB_DIALOGS_H__
#define __DMB_DIALOGS_H__

G_BEGIN_DECLS

void
plugin_configure(XfcePanelPlugin *plugin,
                 DmbPlugin *sample);

void
plugin_about(XfcePanelPlugin *plugin);

G_END_DECLS

#endif