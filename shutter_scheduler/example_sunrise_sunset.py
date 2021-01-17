# This is a sample Python script.

# Press Umschalt+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

import datetime
import pytz
from suntime import Sun, SunTimeException


def print_sunrise():
    latitude = 48.742672
    longitude = 8.969841

    sun = Sun(latitude, longitude)
    date = datetime.date(2020, 7, 1)
    abd_sr = sun.get_local_sunrise_time(date)
    abd_ss = sun.get_local_sunset_time(date)
    print('sunrise {}; sunset {}.'
          .format(abd_sr.strftime('%H:%M'), abd_ss.strftime('%H:%M')))


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print_sunrise()
