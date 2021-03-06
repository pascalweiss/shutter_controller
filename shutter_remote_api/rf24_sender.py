from __future__ import print_function

import struct

from RF24 import *


class RF24Sender:
    radio = None

    def __init__(self, cepin, cspin, wrinting_pipe, retry_delay, retry_count):
        self.radio = RF24(cepin, cspin)
        self.radio.begin()
        self.radio.enableDynamicPayloads()
        self.radio.setRetries(retry_delay, retry_count)
        self.radio.printDetails()
        self.radio.openWritingPipe(wrinting_pipe)

    def send(self, level):
        buffer = struct.pack("<f", level)
        self.radio.write(buffer)
