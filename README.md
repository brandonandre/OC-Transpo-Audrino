# OC Transpo Audrino
Using Audrino with an ESP8266WiFi sensor, we can access the OC Transpo API to make a bus tracker.

# Board Requirements
For this project I used the following board and shelid;

| Name                   | Website                                       |
| -----------------------|:---------------------------------------------:| 
| Wemos D1 Mini Pro      | https://www.wemos.cc/product/d1-mini-pro.html |
| Wemos OLED Shield      | https://www.wemos.cc/product/oled-shield.html | 

As long as the board contains a ESP8266WiFi board which is required to access the internet, the screen is also not required since you can get the information based on light codes.

# How to read the bus tracker
There are two main ways to use this board. If you perfer to just use a light to tell you if the bus is close or use a OLED screen to view the status.

## Reading the light
| Light Pattern       | Description                            |   
|---------------------|:--------------------------------------:|
| No Light            | Do not leave for the bus, too far away |
| Solid Light         | Leave now to arrive to stop            |
| Blinking Light      | Wait for the next bus. Bus at the stop.|

## Reading OLED display
Once it connects to the internet and retrives the OC Transpo API you will be shown on the display with a number.

# Building
1. Change the API App ID and Secret code. 
2. Change the route number (Example: 118)
3. Change the bus stop. (Can be found on the bus stop signs. This determines the direction of the route.)
4. Change the go time variable.
5. Change the wait time variable.
6. Change WiFi Host
7. Change WiFi Password
8. Change the bus name.

# Future Updates
* Animations between screen
* Ability to cycle between two routes.
* Better leaving predictions.
* Better optimization with calls.
* Better WiFi optimization (Reconnection functionality)
