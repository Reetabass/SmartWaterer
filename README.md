# Smart Waterer

A DIY soil moisture monitoring and automatic plant watering system, designed to keep your basil (and other plants) healthy and hydrated.

## Work in Progress

To view the current code, you will find the main.c file within the src folder.

### Completed:
-Prototype built
![IMG_2681](https://github.com/user-attachments/assets/ce1b78ab-2ccb-4a0a-b1f6-ac54e35ab7d0)
-5v relay to control the pump
-Moisture sensor using ADC

### Currently working on:
- Writing firmware to implement core system features

## Planned Features
- Soil moisture sensing using a capacitive sensor  
- Automated watering via a 5V relay and submersible pump  
- USB-C power for development, with plans to transition to battery operation for outdoor use  
- Communication with a Raspberry Pi server to:
  - Send real-time moisture data
  - Allow remote manual watering control

## Hardware Components
- Adafruit QT Py S3  
- Capacitive soil moisture sensor  
- 5V relay module  
- Submersible water pump  
- Battery pack (for pump)
- USB-C cable (for development power)

## TODO
- [ ] Read and calibrate soil moisture levels  
- [ ] Trigger pump automatically when soil is dry  
- [ ] Send sensor data to Raspberry Pi  
- [ ] Enable remote watering trigger  
- [ ] Test and finalize battery-powered operation

## License
This project is open-source and available under the MIT License.
