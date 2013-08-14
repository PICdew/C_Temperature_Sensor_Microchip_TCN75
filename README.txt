
######################################################################
##### Open sourced by IBEX, an electronic product design company #####
#####    based in South East England.  http://www.ibexuk.com     #####
######################################################################

The Microchip TCN75 is an I2C Bus temperature sensor device. It provides accurate temperture measurement at the PCB for embedded designs.

This driver provides the following functions:


READ TEMPERATURE
BYTE read_temperature (SIGNED_WORD *temperature)
Returns 1 if temperature was read sucessfully, 0 if not
temperature =
Temperature reading x0.5ºC (0×0000 = 0ºC)


Please view the sample source code files for full information regarding the driver.