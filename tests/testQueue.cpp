#include <gtest/gtest.h>
#include <iostream>
#include "Queue.h"

TEST(Queue, vote) {
    Queue queue;
    ASSERT_NO_THROW(queue.push(2));
}

TEST(Queue, sleep) {
    Queue queue;
    ASSERT_NO_THROW(queue.pop());
}
