import urllib.request
import requests
import threading
import json
import time

while True:
    def dados_display():
        # Informa a URL onde está armazenada as informações
        URL = 'https://api.thingspeak.com/channels/1183823/feeds/last.json?api_key=RQXI8JMR66N07ZVZ'
        # Transcreve essas informações em um formato .json e armazena na variavel 'data'
        get_data = requests.get(URL).json()
        # Pega a informação do 'campo' 1 dentro do .json em 'data', onde está localizada a informação referente a temperatura - e assim respectivamente nas proximas variaveis.
        temperatura = get_data['field1']
        humidade = get_data['field2']
        corrente = get_data['field3']
        rpm = get_data['field4']
        print(
            f'Temperatura: {temperatura}°C\nHumidade: {humidade}%\nCorrente Elétrica: {corrente}A\nRPM: {rpm}r/min\n')

    if __name__ == '__main__':  # Garante que a função só será chamada caso a anterior for executada corretamente
        dados_display()

    # Tempo de leitura de um novo dado (mais informações na escrita de informações do .ino)
    time.sleep(15)
