The Microchip TCN75 is an I2C Bus temperature sensor device. It provides accurate temperture measurement at the PCB for embedded designs.

This driver provides the following functions:


READ TEMPERATURE
BYTE read_temperature (SIGNED_WORD *temperature)
Returns 1 if temperature was read sucessfully, 0 if not
temperature =
Temperature reading x0.5�C (0�0000 = 0�C)


Please view the sample source code files for full information regarding the driver.