# CH32 Deep Sleep
Simple deep sleep with autowakeup for CH32VXXX RISC-V
Based on https://github.com/cnlohr/ch32v003fun

Version: 1.0

Date: 2024-08-15

Tested Devices:
* CH32V00x

Untested, but should work:
* CH32V10x
* CH32V20x
* CH32V30x
* CH32X035

## Functions:

### DeepSleep.init(BASE,NUM)

Set base periode to BASE and sleep duration to NUM base periodes.
Default is 480ms and 2 periodes --> ~ 1sec.
You can change the values at any time by clling DeepSleep.init() again.

Valid values for BASE:

BASE_MS_1

BASE_MS_2

BASE_MS_4

BASE_MS_8

BASE_MS_16

BASE_MS_32

BASE_MS_80

BASE_MS_480

### DeepSleep.sleep()

Enter deep sleep for the set amount of time, abutowakeup.

### DeepSleep.sleepI()

Enter deep sleep for the set amount of time, autowakeup and IRQ wakeup.

## Note
You *MUST* add a delay(5000) to your setup(). If you don't and your loop() enters
DeepSleep.sleep() you'll have a hard time reprogramming the device. You have been warned.

© Dr. Nikolaus Klepp - office@klepp.biz
