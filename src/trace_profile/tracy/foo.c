#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

#include <tracy/TracyC.h>

static void comp_helper(int64_t i) {
  char buf[64];
  int cnt = snprintf(buf, sizeof(buf), "helper(%" PRId64 ")", i);

  // Create an active unnamed zone.
  TracyCZone(ctx, 1);

  // Name the zone.
  TracyCZoneName(ctx, buf, cnt);
  // Add custom text to the zone measurement.
  TracyCZoneText(ctx, buf, cnt);
  // Add custom value to the zone measurement.
  TracyCZoneValue(ctx, i);

  for (int ii = 0; ii < i * 100000; ++ii) {
    /* fake work */
  }

  // End the zone measurement.
  TracyCZoneEnd(ctx);
}

void foo_comp_hook(int64_t cnt) {
  // Create an active named zone.
  TracyCZoneN(ctx, "foo", 1);

  for (int i = 0; i < cnt; ++i) {
    // Plot value.
    TracyCPlot("foo_comp_hook", cnt + i);

    comp_helper(i);
  }

  // Configure plot "foo", probably best done once during initialization..
  TracyCPlotConfig("foo", TracyPlotFormatNumber, 1 /* step */, 1 /* fill */,
                   0xff0000);
  // Plot value.
  TracyCPlot("foo", cnt);

  // End the zone measurement.
  TracyCZoneEnd(ctx);
}
