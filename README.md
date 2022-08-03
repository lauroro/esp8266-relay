# esp8266-relay
Esp8266 project to control 4 relays in Normally Open mode.
![20220726_120540](https://user-images.githubusercontent.com/88981092/181010780-2c0d2499-4ecd-499e-bef6-1b43fcac9063.jpg)

- **Pay attention while handling main voltage items**. Don't plug main voltage items while your esp is connected to your PC. Be sure to not touch exposed cables. Don't do anything if you don't know what you're doing please. If you are following this project just to try it don't use main voltage devices at all, just use your relay modules, as they have a built-in led that turns on and off giving you a harmless feedback.

## Circuit
With this project you can control 4 relay modules. Here I'll just show one, cause I only have one relay module at the moment, and so this is the circuit I made.
![Relais-Module-NodeMCU_Steckplatine webp](https://user-images.githubusercontent.com/88981092/180991265-87579b7f-b2ad-42ce-99eb-d7626d66cbde.png)

### How to extend the circuit to control more than one module
Connect the signal line as follows (refers to NodeMCU ESP8266):
- Relay 0 --- D1
- Relay 1 --- D2
- Relay 2 --- D5
- Relay 3 --- D6

## What's needed
- 1x Esp8266.
- 1-4x Relay module
- (Optional if you don't want to solder) 1x Breadboard
- 3x Cables (or jumpers if you opt for the breadboard)
- Arduino IDE
- Clone this repo: you need both the sketch and the data folder.

## Libraries used
1. If you use Arduino IDE be sure to [install esp8266 support](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) first.
2. External libraries (look into each library's readme file to see further dependancies):
    - [Arduino ESP8266 LittleFS Filesystem Uploader](https://github.com/earlephilhower/arduino-esp8266littlefs-plugin)
    - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

## Project Structure
Using LittleFS uploader you will upload actual html, css and javascript files on the esp8266. As it works under Arduino IDE, just open a new project -> paste the sketch.ino content -> press CTRL+k -> from the project folder create a new folder named "data" -> copy the data folder content of this repo on your data folder. At the end, assuming you are working from a "sketch" folder you will have this situation:
```
sketch/
  |
  ꜔ sketch.ino
  ꜔ data/
      |
      ꜔ index.html
      ꜔ style.css
      ꜔ main.js
      ꜔ on.svg
      ꜔ off.svg
```
Follow the LittleFS usage guide to flash the data/ content on the esp filesystem.

## How it works
### Sketch
The _websocket_ functionality from **ESPAsyncWebServer** grants the possibility to efficiently exchange informations between the mcu and all the clients connected to it. It's also used to notify every client about some change of state, and all this in realtime. The esp will connect to wifi (be sure to replace ssid and password so that they match your credentials: once it connects it will print ip address and MAC on the serial monitor, so you can use the ip address to connect to the esp, and MAC address for binding purposes). While running the program will read inputs from the clients and will update it's state. In particular, it will performs some appropriate _digitalWrite_ on the desired pin. Thanks to **LittleFS** we can upload files directly in the esp's filesystem. This grants the possibility to write dedicated html, css, javascript files, but also icons, images, libraries (the space is still very limited by the way).
### data/ files
Inside this folder there is all the necessary for the web-interface. In the file **main.js** it's written the client-side logic. When the DOM is loaded the client sends an http request to the esp to ask for the relays configuration. The response is a string that contains the definition of _divs_, each cointaining some text and _buttons_, one for each relay. This string will be inserted in the html file dinamically hacking into the DOM. Upon a click event on these buttons, a toggle update is sent to the server (the esp) via websocket. Once the server handled the toggle request it will notify every client about the change. Every client will read the notification and will change the appropriate buttons' style accordingly (it will swap the button background image from 'on' to 'of' or viceversa). Here in main.js we also see that the client open a websocket to communicate with the esp (at _"/ws"_). 

## Demo
This is a demo of the final result.

https://user-images.githubusercontent.com/88981092/181009117-963f96fb-b093-48e4-b3e4-dded8c0c6ba1.mp4

This is to show that the client interface is responsive.

https://user-images.githubusercontent.com/88981092/181008683-d791d194-fb39-4da3-b968-99ee2bcb979d.mp4

And finally, a close up on the web-interface from mobile.

https://user-images.githubusercontent.com/88981092/181011890-6bae343d-cd67-4a6c-982b-f4af08a033cb.mp4

## Single relay mode and beyond
The most recent feature adds the possibility to tweak the UI a little bit, if needed. In particular, it's possible to limit the view to only one relay toggle button. It's also possible to specify the text content of the page header (this is now possible also in the all relays view). This is handled is the URL.
```
<ESP-IP-ADDRESS>[/?name=name-value&relays=relays-value]

// Fields
name-value : everything
relays-value : all || r0 || r1 || r2 || r3 || everything else -> default 

// Defaults
name-value : "Relays Controller"
relays-value = "all";
```

![screen](https://user-images.githubusercontent.com/88981092/182624747-43a7a0bc-5384-487e-b36f-b927f8913c26.png)
