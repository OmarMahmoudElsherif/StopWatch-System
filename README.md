# StopWatch-System

# Used ATmega32 Microcontroller with frequency 1Mhz.

# Configured Timer1 in ATmega32 with CTC mode to count the Stop Watch time.

# Used six Common Anode 7-segments.

# Connected the six 7-segments in the project using the multiplexed technique. You should use one 7447 decoder for all 7-segments and control the enable/disable for each 7-segement using a NPN BJT transistor connect to one of the MCU pins.

# We connected more than one 7-segment display by using the Multiplexing method. In this method, at a time one 7-segment display is driven by the Microcontroller and the rest are OFF. It keeps switching the displays using transistors. Due to the persistence of vision, it appears as a normal display.

# Configured External Interrupt INT0 with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be reset.

# Configured External Interrupt INT1 with raising edge. Connect a push button with the external pull-down resistor. If a raising edge detected the Stop Watch time should be paused.

# Configured External Interrupt INT2 with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be resumed.
