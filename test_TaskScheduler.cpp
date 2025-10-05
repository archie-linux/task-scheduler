#include <gtest/gtest.h>
#include "TaskScheduler.cpp"

TEST(TaskSchedulerTest, AddTaskAndList) {
    TaskScheduler scheduler;
    scheduler.add_task("Task 1", 2);
    scheduler.add_task("Task 2", 1, "2025-06-10");

    // Both tasks should be executable (no dependencies)
    EXPECT_TRUE(scheduler.can_execute(1));
    EXPECT_TRUE(scheduler.can_execute(2));
}

TEST(TaskSchedulerTest, AddDependencyAndExecution) {
    TaskScheduler scheduler;
    scheduler.add_task("Task A", 2);
    scheduler.add_task("Task B", 1);
    EXPECT_TRUE(scheduler.add_dependency(2, 1)); // Task B depends on Task A

    // Task B should not be executable before Task A
    EXPECT_TRUE(scheduler.can_execute(1));
    EXPECT_FALSE(scheduler.can_execute(2));

    // Execute Task A
    scheduler.execute_next();
    EXPECT_TRUE(scheduler.can_execute(2));
}

TEST(TaskSchedulerTest, CycleDetection) {
    TaskScheduler scheduler;
    scheduler.add_task("Task X", 1);
    scheduler.add_task("Task Y", 2);
    EXPECT_TRUE(scheduler.add_dependency(2, 1));
    EXPECT_FALSE(scheduler.add_dependency(1, 2)); // Should fail due to cycle
}
