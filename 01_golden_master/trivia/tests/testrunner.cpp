#include <stdio.h>
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    printf("Running main() from testrunner.cpp\n");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
