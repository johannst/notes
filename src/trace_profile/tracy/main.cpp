#include <chrono>
#include <thread>

#include <tracy/Tracy.hpp>

#ifdef USE_FOO
extern "C" void foo_comp_hook(int64_t);
#endif

void init() {
  // Create a named zone (active for the current scope).
  // Name will be used when rendering the zone in the thread timeline.
  ZoneScopedN("init()");
  // Set explicit color for the rendered zone.
  ZoneColor(0xff0000);

  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void comp(const char* name) {
  // Track call count.
  static int64_t ccnt = 0;
  ccnt += 1;

  // Create an unnamed zone for the current scope.
  ZoneScoped;
  // Name the zone by formatting the name dynamically.
  // This name is shown for the zone in the thread timeline, however
  // in the zone statistics they are all accounted under one common
  // zone "comp".
  ZoneNameF("comp(%s)", name);
  // Additional text to attach to the zone.
  ZoneTextF("text(%s)", name);
  // Additional value to attach to the zone measurement.
  ZoneValue(ccnt);

  // Statistics for dynamic names, text and values can be looked at in the zone
  // statistics.There measurements can be grouped by different categories.

  // Add a simple plot.
  TracyPlot("comp-plot", ccnt % 4);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

#ifdef USE_FOO
  foo_comp_hook(ccnt);
#endif
}

void post_comp() {
  // Create an unnamed zone for the current scope and capture callstack (max
  // depth 10). Capturing callstack requires platform with TRACY_HAS_CALLSTACK
  // support.
  ZoneScopedS(10);
  // Name the zone, w/o formatting.
  const char name[] = "post_comp()";
  ZoneName(name, sizeof(name));

  // Add trace messages to the timeline.
  TracyMessageL("start sleep in post_comp()");
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  TracyMessageL("end sleep in post_comp()");
}

void fini() {
  // Create a named zone with an explicit color.
  ZoneScopedNC("fini()", 0x00ff00);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
  // Create a named zone.
  ZoneScopedN("main()");

  init();

  int step = 0;
  while (step++ < 10) {
    // Create a frame message, this start a new frame with the name
    // "step" and end the previous frame with the name "step".
    FrameMarkNamed("step");
    // Create a named scope.
    ZoneScopedN("step()");
    comp("a");
    comp("b");
    comp("c");
    post_comp();
  }

  fini();
}
