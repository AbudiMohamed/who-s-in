import requests
from time import sleep
from mfrc522.MFRC522 import MFRC522
import RPi.GPIO as GPIO
import signal

GPIO.setmode(GPIO.BOARD)
GPIO.setup(8, GPIO.OUT, initial=GPIO.LOW) 
GPIO.setup(10, GPIO.OUT, initial=GPIO.LOW) 
GPIO.setup(12, GPIO.OUT, initial=GPIO.LOW) 
MIFAREReader = MFRC522()
TRAINERS = []
URL = 'http://192.168.178.82:8000/api'

def turn_led_on (led) :
    GPIO.setup(led, GPIO.OUT) #Active le contrôle du GPIO
    GPIO.output(led, GPIO.HIGH) #Allume la led

#Définit la fonction permettant d'éteindre une led
def turn_led_off (led) :
    GPIO.setup(led, GPIO.OUT) #Active le contrôle du GPIO
    GPIO.output(led, GPIO.LOW) #Eteind la led

def convert_uid_to_int(uid:[]):
    s = ''
    for id in uid:
        s = s + str(id)
    return int(s)

try:
    while True:
        (status,TagType) = MIFAREReader.MFRC522_Request(MIFAREReader.PICC_REQIDL)
        if status == MIFAREReader.MI_OK:
            print ("Card detected")
        (status,uid) = MIFAREReader.MFRC522_Anticoll()
        if status == MIFAREReader.MI_OK:

            id = convert_uid_to_int(uid)
            print(id)
            if id not in TRAINERS:
                payload = requests.get(url=f'{URL}/checkin?id={id}').text
                print(payload)
                if payload != 'false':
                    TRAINERS.append(id)
                    print(f'Access Granted')
                    GPIO.output(8, GPIO.HIGH) # Turn on
                    sleep(3)                  # Sleep for 1 second
                    GPIO.output(8, GPIO.LOW)
                elif payload == 'false':
                    print("Unautherised")
                    GPIO.output(12, GPIO.HIGH) # Turn on
                    sleep(3)                  # Sleep for 1 second
                    GPIO.output(12, GPIO.LOW)  # Turn off
            elif id in TRAINERS:
                payload = requests.get(url=f'{URL}/checkout?id={id}').text
                TRAINERS.remove(id)
                print(f'{id} CHECKED-OUT')
                print(payload)
                GPIO.output(10, GPIO.HIGH) # Turn on
                sleep(3)                  # Sleep for 1 second
                GPIO.output(10, GPIO.LOW)
except KeyboardInterrupt:
    GPIO.cleanup()
    raise
