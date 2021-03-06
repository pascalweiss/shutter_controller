import logging

from flask import Flask, request

from rf24_sender import RF24Sender

app = Flask(__name__)
sender = RF24Sender(22, 0, 0xF0F0F0F0E1, 5, 15)
logger = logging.getLogger('shutter_remote_api')


@app.route('/setShutterLevel', methods=['POST'])
def set_shutter_level():
    level = request.json["level"]
    logger.info("received level: {}".format(level))
    sender.send(level)
    return "New shutter level: {}".format(level)


if __name__ == '__main__':
    app.run(debug=True)
