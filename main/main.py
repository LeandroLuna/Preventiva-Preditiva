import urllib.request
import requests
import threading
import json
import time

while True:
    def dados_display():
        URL = 'https://api.thingspeak.com/channels/1183823/feeds/last.json'
        get_data = requests.get(URL).json()
        temperatura = get_data['field1']
        humidade = get_data['field2']
        corrente = get_data['field3']
        print(
            f'Temperatura: {temperatura}°C\nHumidade: {humidade}%\nCorrente Elétrica: {corrente}A\n')

    if __name__ == '__main__':
        dados_display()

    time.sleep(15)
