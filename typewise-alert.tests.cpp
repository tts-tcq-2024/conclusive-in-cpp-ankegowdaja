#include <gtest/gtest.h>
#include "typewise-alert.h"

// Helper function to capture stdout
std::string captureStdout(void (*func)()) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    func();
    std::cout.rdbuf(old);
    return oss.str();
}

// Test cases
TEST(TypeWiseAlertTestSuite, InfersBreachAccordingToLimits) {
    // Test for Passive Cooling
    EXPECT_EQ(inferBreach(-1, 0, 35), TOO_LOW);
    EXPECT_EQ(inferBreach(10, 0, 35), NORMAL);
    EXPECT_EQ(inferBreach(36, 0, 35), TOO_HIGH);

    // Test for HI Active Cooling
    EXPECT_EQ(inferBreach(-1, 0, 45), TOO_LOW);
    EXPECT_EQ(inferBreach(20, 0, 45), NORMAL);
    EXPECT_EQ(inferBreach(46, 0, 45), TOO_HIGH);

    // Test for MED Active Cooling
    EXPECT_EQ(inferBreach(-1, 0, 40), TOO_LOW);
    EXPECT_EQ(inferBreach(20, 0, 40), NORMAL);
    EXPECT_EQ(inferBreach(41, 0, 40), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreach) {
    // Test for Passive Cooling
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 10), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 36), TOO_HIGH);

    // Test for HI Active Cooling
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);

    // Test for MED Active Cooling
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 20), NORMAL);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, SendsAlertsCorrectly) {
    // Set up BatteryCharacter for testing
    BatteryCharacter batteryCharPassive = {PASSIVE_COOLING, "BrandX"};
    BatteryCharacter batteryCharHiActive = {HI_ACTIVE_COOLING, "BrandY"};

    // Capture output for TO_EMAIL
    std::string output;

    // Test sending email for Passive Cooling
    output = captureStdout([&]() {
        checkAndAlert(TO_EMAIL, batteryCharPassive, -1);
    });
    EXPECT_NE(output.find("Hi, the temperature is too low"), std::string::npos);

    output = captureStdout([&]() {
        checkAndAlert(TO_EMAIL, batteryCharPassive, 36);
    });
    EXPECT_NE(output.find("Hi, the temperature is too high"), std::string::npos);

    // Test sending email for HI Active Cooling
    output = captureStdout([&]() {
        checkAndAlert(TO_EMAIL, batteryCharHiActive, -1);
    });
    EXPECT_NE(output.find("Hi, the temperature is too low"), std::string::npos);

    output = captureStdout([&]() {
        checkAndAlert(TO_EMAIL, batteryCharHiActive, 46);
    });
    EXPECT_NE(output.find("Hi, the temperature is too high"), std::string::npos);

    // Test sending to controller
    output = captureStdout([&]() {
        checkAndAlert(TO_CONTROLLER, batteryCharPassive, 36);
    });
    EXPECT_NE(output.find("feed : 2"), std::string::npos); // 2 for TOO_HIGH
}
