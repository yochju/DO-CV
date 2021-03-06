// ========================================================================== //
// This file is part of DO++, a basic set of libraries in C++ for computer 
// vision.
//
// Copyright (C) 2014 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public 
// License v. 2.0. If a copy of the MPL was not distributed with this file, 
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

// STL.
#include <iostream>
// Google Test.
#include <gtest/gtest.h>
// DO-CV.
#include <DO/Core.hpp>
#include <DO/Graphics.hpp>
#include <DO/Graphics/GraphicsUtilities.hpp>
// Local class.
#include "event_scheduler.hpp"

using namespace DO;
using namespace std;

QPointer<EventScheduler> scheduler;

class TestSleepFunctions: public testing::Test
{
protected:
  Window test_window_;

  TestSleepFunctions()
  {
    test_window_ = openWindow(300, 300);
    scheduler->set_receiver(test_window_);
  }

  virtual ~TestSleepFunctions()
  {
    closeWindow(test_window_);
  }
};

TEST_F(TestSleepFunctions, test_milliSleep)
{
  int delay_ms = 20;
  Timer timer;
  timer.restart();
  milliSleep(delay_ms);
  double elapsed = timer.elapsedMs();

  double tol_ms = 1.;
  EXPECT_NEAR(elapsed, static_cast<double>(delay_ms), tol_ms);
}

TEST_F(TestSleepFunctions, test_microSleep)
{
  int delay_us = 100*1000; // 100 ms because 1000 us = 1 ms.
  Timer timer;
  timer.restart();
  microSleep(delay_us);
  double elapsed = timer.elapsedMs();

  double tol_us = 5;
  EXPECT_NEAR(elapsed, static_cast<double>(delay_us)*1e-3, tol_us);

}

int worker_thread_task(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv); 
  return RUN_ALL_TESTS();
}

#undef main
int main(int argc, char **argv)
{
  // Create Qt Application.
  GraphicsApplication gui_app_(argc, argv);

  // Create an event scheduler on the GUI thread.
  scheduler = new EventScheduler;
  // Connect the user thread and the event scheduler.
  QObject::connect(&getUserThread(), SIGNAL(sendEvent(QEvent *, int)),
                   scheduler.data(), SLOT(schedule_event(QEvent*, int)));

  // Run the worker thread 
  gui_app_.registerUserMain(worker_thread_task);
  int return_code = gui_app_.exec();

  // Cleanup and terminate
  scheduler.clear();
  return return_code;
}