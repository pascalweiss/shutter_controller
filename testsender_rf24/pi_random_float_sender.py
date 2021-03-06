#!/usr/bin/env python

#
# Example using Dynamic Payloads
#
#  This is an example of how to use payloads of a varying (dynamic) size.
#

from __future__ import print_function
import time
import random
import struct
from RF24 import *


def build_radio():
    # Generic:
    radio = RF24(22, 0)
    pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]
    radio.begin()
    radio.setRetries(5, 15)
    radio.printDetails()
    radio.openWritingPipe(pipes[0])
    return radio


# Wait here until we get a response, or timeout
def wait_till_available(radio, timeout=500):
    millis = lambda: int(round(time.time() * 1000))
    started_waiting_at = millis()
    timeout_exceeded = False
    while (not radio.available()) and (not timeout):
        if (millis() - started_waiting_at) > timeout:
            timeout_exceeded = True
    return timeout_exceeded


def get_payload(i):
    return "{}".format(i)


def str_to_bytes(msg):
    return bytearray(msg.encode("utf-8"))


def run(radio):
    i = 0
    while 1:
        f = 0.0;
        buffer = struct.pack("<f", random.random())
        radio.write(buffer);


if __name__ == "__main__":
    radio = build_radio()
    run(radio)
