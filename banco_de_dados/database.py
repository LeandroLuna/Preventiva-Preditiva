#!/usr/bin/env python3
import time
import urllib
import requests
import threading
import json

# Importa o conector mariadb e seus devidos modulos
import mysql.connector as mariadb
import mysql.connector

# Estabelece a conexao com banco de dados
db = mariadb.connect(user="pjbosch",
                            host="localhost",
                            password="1254",
                            database="projeto_preditivo"
							)
cursor= db.cursor()

while True:
	data = time.strftime('%Y-%m-%d %H:%M:%S')
	URL = 'https://api.thingspeak.com/channels/1183823/feeds/last.json?api_key=RQXI8JMR66N07ZVZ'
	get_data = requests.get(URL).json()
	temperatura = get_data['field1']
	humidade = get_data['field2']
	corrente = get_data['field3']
	rpm = get_data['field4']

	dados = (temperatura, humidade, corrente, rpm, data)

#	cursor.execute("CREATE TABLE ifm(temperatura FLOAT,"
#	      		   "humidade FLOAT,"
#			       "corrente FLOAT,"
#			       "rpm FLOAT,"
#				   "data TEXT)")
			   	
	cursor.execute("INSERT INTO ifm(temperatura, humidade, corrente, rpm, data) VALUES (%s, %s, %s, %s, %s);",dados)
	db.commit()
	print('Novas informacoes inseridas com sucesso no banco de dados!')
	time.sleep(15)	
