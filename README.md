# esp8266-relay
Esp8266 project to control 4 relays in Normally Open mode.
**Attention!** Pay attention to main voltage items. Don't plug main voltage items while your esp is connected to your PC. Be sure to not touch exposed cables. Don't do anything if you don't know what you're doing please. If you are following this project just to try it don't use main voltage devices at all, just use your relay modules, as they have a built-in led that turns on and off giving you a harmless feedback.

## Circuit
With this project you can control 4 relay modules. Here I'll just show one, cause I only have one relay module at the moment, and so this is the circuit I made.
![Relais-Module-NodeMCU_Steckplatine webp](https://user-images.githubusercontent.com/88981092/180991265-87579b7f-b2ad-42ce-99eb-d7626d66cbde.png)

### How to extend the circuit to control more than one module
Connect the signal line as follows (refers to NodeMCU ESP8266):
Relay 0 --- D2
Relay 1 --- D4
Relay 2 --- D14
Relay 3 --- D12
