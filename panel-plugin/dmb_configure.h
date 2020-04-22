#include "dmb_shared.h"
//
// Created by admin_01 on 21.04.2020.
//

#ifndef DMB_DMB_CONFIGURE_H
#define DMB_DMB_CONFIGURE_H

static const Date blank_date = {
  .d = 0,
  .m = 0,
  .y = 0
};

void plugin_save(XfcePanelPlugin *plugin, DmbPlugin *data);

void plugin_read(DmbPlugin *d_plugin);

#endif //DMB_DMB_CONFIGURE_H