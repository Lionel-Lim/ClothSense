# ClothSense

CASA0016

DY Lim

* * *

  

![](http://localhost:9425/images/c1b11536-7deb-4060-b856-c49acf1203f5.jpeg)

## When is it going to be dried?

###   

### No dryer.

Energy Bill, Damage on cloth, Noise...

  

Every time I needed to touch the cloth to check whether it is dried or still wet.

However, it is **_hard to predict_** even if i do so. 

Also, I want to **_boost the drying process_** and want to **_know the condition_** where it is located(Bad air quality, weather).

  

ClothSense is going to  **monitor**:

- Temperature
- Humidity
- ~~Air Quality~~

ClothSense is going to **boost**:

- Dry State

ClothSense is going to **show:**

- Estimated Time to dry
- Current Environmental Condition

###   

## Component

- Arduino Uno (MCU)
- ~~BME680~~ DHT22 (Environment Sensor) - Digital Pin
- max7219 LED Matrix 8x32 (Display) - SPI
- NF-F12 5V PMW (Fan) - PWM
- Buttons - Digital Pin

  

## Workflow

![](http://localhost:9425/images/65b1f9c9-5059-4b16-b5ae-42253287b557.png)

  

## Build Process

### First Prototype

![](http://localhost:9425/images/a965379b-d051-42b9-ae5d-6bfdb4b226b5.png)  

Tested Fan Control using PWM control

By pressing button, Uno changes fan speed for noise control.

![](http://localhost:9425/images/55688d8f-8f8c-4ddb-87db-3514510ee90c.png)  

[Arduino PWM Tutorial - Arduino Project Hub](https://create.arduino.cc/projecthub/muhammad-aqib/arduino-pwm-tutorial-ae9d71)  

  

### Second Prototype

![](http://localhost:9425/images/1213f4df-0544-4aa9-a8c4-c65307eda173.png)  

[Old-fashioned vintage radio isolated on a white background - | CanStock (canstockphoto.com)](https://www.canstockphoto.com/old-fashioned-vintage-radio-isolated-on-82587771.html)  

Inspired by the old fashion radios, got the idea from Prof. Andy.

  

![](http://localhost:9425/images/982c5723-b4ff-43a7-ab0e-ef83bd6093bc.png)  

3D Printing Enclosure

- Slow
- Not Precise
- Good for 3d solid object prototyping

Removed servo

Moved LED Matrix

Changed sie

  

### Third Prototype

![](http://localhost:9425/images/abc93835-26d8-4c76-951c-06ebfb5f4913.png)  

![](http://localhost:9425/images/d88af8ba-775a-4ee6-9d4f-5a50a94d498d.jpeg)  

Laser cut wood enclosure

- Fast
- Precise

3D Printed top lid to hide LED matrix and for aesthetic reason

  

with the enclosure, the device was tested and some problems are found:

1. BME680 library uses more than 30% of maximum sketch storage. (From 70% to 117%)

- [Reduce the size and memory usage of your sketch – Arduino Help Center](https://support.arduino.cc/hc/en-us/articles/360013825179-Reduce-the-size-and-memory-usage-of-your-sketch)  
    
- Remove BME680 and use  DHT22
- Use Arduino Uno instead of Leonardo (LoRa Library)
- Optimised variables and functions

3. Did not design the component mounting

- Made some mounting brackets
- ![](http://localhost:9425/images/5f301b1e-915b-435e-95f6-9ef12db263cf.jpeg)  
    

  

### Final Build

Modularised parts

![](http://localhost:9425/images/1ce0a8f7-6a10-4c30-b78e-c392fd43dba1.jpeg) ![](http://localhost:9425/images/1c0e3eff-6608-427f-9409-922dc34b161a.jpeg)

  

Used DHT22

![](http://localhost:9425/images/ed67920e-300b-444d-84e0-961939e7e633.jpeg)  

![](http://localhost:9425/images/4cb51150-4156-4f55-b198-d3ff9d0d71b2.jpeg)  

  

Added mesh to prevent the interference between the fan and wires

![](http://localhost:9425/images/bf99227b-b9bc-420b-a301-27ff90cd2668.jpeg)  

![](http://localhost:9425/images/81d22b71-f546-4fe2-87cd-4b94b15b7248.jpeg)  

  

Introduced the mounting bracket

![](http://localhost:9425/images/097a3f5d-d074-4a8a-9289-28d31aa242a5.jpeg)  

  

Result

![](http://localhost:9425/images/bd999468-23cf-47e6-8c8c-8b88cf63810d.jpeg)  

![](http://localhost:9425/images/a7b3d021-064b-484e-85ee-26fd9b698d2b.png)  

![](http://localhost:9425/images/50d484b1-130d-4fef-9fc4-0c2241c8223c.jpeg)  

![](http://localhost:9425/images/b55368e4-0d01-4e3d-b6f8-7eba9f8418dd.jpeg)  

![](http://localhost:9425/images/c73377bb-a535-409a-b1ba-858350322714.jpeg)  

  

## If i had more time, I would

- Use wireless data transmission such as WiFi, LoRa...
- Improve the enclosure
- Order a custom PCB board to reduce the device size

  

## Where can it be used?

![](http://localhost:9425/images/71460ba0-4807-41e9-a745-6ed08437ac9c.png)  

[A Grade Dehydrated Vegetables, Pan India, Packaging Size: 5 Kg at Rs 350/kg in New Delhi (indiamart.com)](https://www.indiamart.com/proddetail/dehydrated-vegetables-24169149248.html)  

![](http://localhost:9425/images/750f08aa-71a3-4f84-9b5e-4d7b0796a330.png)  

[9 Hat and Coat Storage Ideas When You Don't Have a Coat Closet | Apartment Therapy](https://www.apartmenttherapy.com/9-creative-ways-to-organize-your-coats-and-hats-by-the-door-when-you-dont-have-a-coat-closet-239859)  

  

## Lesson Learnt

- Test, Test, Test and Test.

- Even though the several design, Some improvement can be made.

- Laser Cutter is better than what I expected.

- If the prototype shape is not complicate, Laser cutter is a good option. 

- The bouncing issue is too difficult (If you only use software debouncing).

- Depends on the type of button, wire, etc..., the button needed to be calibrated.

  

## Reference

_Adafruit BME680_ (no date) _Adafruit Learning System_. Available at: [https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas/overview](https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas/overview) (Accessed: 21 November 2022).

_BME680 Breakout - Air Quality, Temperature, Pressure, Humidity Sensor - Pimoroni_ (no date). Available at: [https://shop.pimoroni.com/products/bme680-breakout](https://shop.pimoroni.com/products/bme680-breakout) (Accessed: 21 November 2022).

_Leonardo | Arduino Documentation_ (no date). Available at: [https://docs.arduino.cc/hardware/leonardo](https://docs.arduino.cc/hardware/leonardo) (Accessed: 28 November 2022).

_OctoPrint Login_ (no date). Available at: [http://octopi-prusa-2.celab/login/?redirect=%2F%3F&permissions=STATUS%2CSETTINGS\_READ](http://octopi-prusa-2.celab/login/?redirect=%2F%3F&permissions=STATUS%2CSETTINGS_READ) (Accessed: 6 December 2022).

_PWM conflict? One DC servo, one PWM fan motor - Using Arduino / Motors, Mechanics, Power and CNC_ (2020) _Arduino Forum_. Available at: [https://forum.arduino.cc/t/pwm-conflict-one-dc-servo-one-pwm-fan-motor/662796](https://forum.arduino.cc/t/pwm-conflict-one-dc-servo-one-pwm-fan-motor/662796) (Accessed: 3 December 2022).

_UNO R3 | Arduino Documentation_ (no date). Available at: [https://docs.arduino.cc/hardware/uno-rev3](https://docs.arduino.cc/hardware/uno-rev3) (Accessed: 14 December 2022).