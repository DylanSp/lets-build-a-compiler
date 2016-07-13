#include "gtest/gtest.h"

TEST (ExampleTest, RunsCorrectly) {
    TestClass prog;
	prog.run();
	EXPECT_EQ(5, prog.get_variable('X'));
}

class SampleCompilerTest : public ::testing::Test {
public:
    TestClass my_test_class;
    SampleCompilerTest () : my_test_class() {
        my_test_class.run();
    }
    
};

TEST_F(SampleCompilerTest, XEquals5) {
    EXPECT_EQ(my_test_class.get_variable('X'), 5);
}

TEST_F(SampleCompilerTest, Reg0Equals8) {
    EXPECT_EQ(my_test_class.get_register(0), 5);
}