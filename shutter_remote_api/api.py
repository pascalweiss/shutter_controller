import logging

from flask import Flask, request

from rf24_sender import RF24Sender

app = Flask(__name__)
app.logger.setLevel(logging.INFO)
writingPipes = [0xF0F0F0F0E1]
senders = RF24Sender(22, 0, writingPipes, 5, 15)


@app.route('/setShutterLevel', methods=['POST'])
def set_shutter_level():
    pipeId = request.json["receiver"]
    level = request.json["level"]
    app.logger.info("received level: {}".format(level))
    sender.send(level, pipeId)
    return "New shutter level: {}".format(level)


if __name__ == '__main__':
    app.run(port=8081, debug=True)
