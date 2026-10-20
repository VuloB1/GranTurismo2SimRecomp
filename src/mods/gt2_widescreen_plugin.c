#include "mod_plugins.h"

static void gt2_widescreen_activate(void) {
    psx_mod_set_native_wide_projection_correction(1);
    (void)psx_mod_set_fixed_display_aspect(16u, 9u);
}

PSX_MOD_CONSTRUCTOR(gt2_register_widescreen_plugin) {
    (void)psx_mod_register_activation_plugin("gt2.widescreen.16-9", gt2_widescreen_activate);
}
