#include <catch2/catch_test_macros.hpp>
#include <eventpp/eventqueue.h>

TEST_CASE("eventpp basic event queue usage", "[eventpp]") {
  using Queue = eventpp::EventQueue<int, void(int)>;

  Queue q;

  int sum = 0;

  // Listener 1
  q.appendListener(1, [&](int value) { sum += value; });

  // Listener 2
  q.appendListener(1, [&](int value) { sum += value * 10; });

  // Enqueue a few events
  q.enqueue(1, 3);
  q.enqueue(1, 2);
  q.enqueue(1, 5);

  // Nothing has executed yet
  REQUIRE(sum == 0);

  // Process all queued events
  q.process();

  // Both listeners fire for each enqueue:
  // value = 3 →   3 + 30
  // value = 2 →   2 + 20
  // value = 5 →   5 + 50
  // total = 110
  REQUIRE(sum == 110);
}
