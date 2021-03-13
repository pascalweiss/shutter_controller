# Shutter Remote API

REST API for sending commands to shutter_remote_controller. 

## Installation

Requires the following lib for nrf24:
https://github.com/nRF24/RF24

## Example Request
```
curl localhost:8081/setShutterLevel --data '{"level": 0.64}' --header "Content-Type:application/json"
```
