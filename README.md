# auto-stepper


## Usage volatile
[documentation](https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/)


A variable should be declared `volatile` whenever its value can be changed by something beyond the control of the code section in which it appears, such as a concurrently executing thread. In the Arduino, the only place that this is likely to occur is in sections of code associated with interrupts, called an interrupt service routine.

