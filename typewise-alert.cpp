#include "typewise-alert.h"
#include <stdio.h>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if (value < lowerLimit) {
    return TOO_LOW;
  }
  if (value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

int getLowerLimit(CoolingType coolingType) {
  switch (coolingType) {
    case PASSIVE_COOLING:
      return 0;
    case HI_ACTIVE_COOLING:
      return 0;
    case MED_ACTIVE_COOLING:
      return 0;
    default:
      return 0; // Default case for safety
  }
}

int getUpperLimit(CoolingType coolingType) {
  switch (coolingType) {
    case PASSIVE_COOLING:
      return 35;
    case HI_ACTIVE_COOLING:
      return 45;
    case MED_ACTIVE_COOLING:
      return 40;
    default:
      return 0; // Default case for safety
  }
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  int lowerLimit = getLowerLimit(coolingType);
  int upperLimit = getUpperLimit(coolingType);
  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  if (alertTarget == TO_CONTROLLER) {
    sendToController(breachType);
  } else if (alertTarget == TO_EMAIL) {
    sendToEmail(breachType);
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recipient = "a.b@c.com";
  switch (breachType) {
    case TOO_LOW:
      printf("To: %s\n", recipient);
      printf("Hi, the temperature is too low\n");
      break;
    case TOO_HIGH:
      printf("To: %s\n", recipient);
      printf("Hi, the temperature is too high\n");
      break;
    case NORMAL:
      break;
  }
}
