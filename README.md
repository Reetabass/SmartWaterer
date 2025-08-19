# Smart Waterer

A DIY soil moisture monitoring and automatic plant watering system, designed to keep your basil (and other plants) healthy and hydrated.

## Work in Progress

### Completed:
- QT Py S3 board connected  
- Firmware successfully flashed

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
- Battery pack  
- USB-C cable (for development power)

## TODO
- [ ] Read and calibrate soil moisture levels  
- [ ] Trigger pump automatically when soil is dry  
- [ ] Send sensor data to Raspberry Pi  
- [ ] Enable remote watering trigger  
- [ ] Test and finalize battery-powered operation

## License
This project is open-source and available under the MIT License.
