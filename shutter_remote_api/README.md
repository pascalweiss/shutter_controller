# Shutter Remote API

REST API for sending commands to shutter_remote_controller. 

## Installation

Requires the following lib for nrf24:
https://github.com/nRF24/RF24

## Example Request
Provide `level` and `receiver` as json via POST
```
curl localhost:8081/setShutterLevel --data '{"level": 0.1, "receiver":0}' --header "Content-Type:application/json"
```
