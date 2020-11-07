from pushbullet import Pushbullet
import time
api_key = 'API'
temperatura = int(input('Digite a temperatura da maquina  '))
umidade = int(input('Digite a Umidade da maquina  '))
ca = int(input('Digite a Corrente elétrica da maquina  '))
rpm = int(input('Digite a RPM da maquina  '))

if temperatura > 95:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "Temperatura esta acima do normal")
    time.sleep(3)
elif temperatura < 30:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "Temperatura esta abaixo do normal")
    time.sleep(3)
if temperatura >= 30 and temperatura <= 95:
    print('Temperatura Normal')
if umidade > 80:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "Umidade esta acima do normal")
    time.sleep(3)
elif umidade < 40:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "Umidade esta abaixo do normal")
    time.sleep(3)
if umidade >= 40 and umidade <= 85:
    print('Umidade Normal')
if ca > 300:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "Corrente elétrica esta acima do normal")
    time.sleep(3)
elif ca < 200:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "Corrente elétrica esta abaixo do normal")
    time.sleep(3)
if ca >= 200 and ca <= 300:
    print('Corrente Eletrica Normal')
if rpm > 3500:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "RPM esta acima do normal")
elif rpm < 2000:
    pb = Pushbullet(api_key)
    push = pb.push_note("ALERTA", "RPM esta abaixo do normal")
if rpm >= 2000 and rpm <= 3500:
    print('RPM Normal')
