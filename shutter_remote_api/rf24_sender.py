from __future__ import print_function

import struct

from RF24 import *


class RF24Sender:
    radio = None
    writingPipes = None

    def __init__(self, cepin, cspin, wrinting_pipes, retry_delay, retry_count):
        self.writingPipes = wrinting_pipes
        self.radio = RF24(cepin, cspin)
        self.radio.begin()
        self.radio.setRetries(retry_delay, retry_count)
        self.radio.printDetails()

    def send(self, level, pipeId):
        pipe = self.writingPipes[pipeId]
        self.radio.openWritingPipe(pipe)
        buffer = struct.pack("<f", level)
        self.radio.write(buffer)
