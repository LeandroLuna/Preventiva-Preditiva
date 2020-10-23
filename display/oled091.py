import time
import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

# Raspberry Pi configuracao pinagem:
RST = None     # no PiOLED esse pino nao e' usado
# A pinagem a seguir so' e' usado no oled SPI:
DC = 23
SPI_PORT = 0
SPI_DEVICE = 0



# 128x32 display com hardware I2C:
disp = Adafruit_SSD1306.SSD1306_128_32(rst=RST)

# Inicializa a biblioteca.
disp.begin()

# Limpa o display.
disp.clear()
disp.display()

# Cria uma imagem vazia para desenhar no display.
# Certifica que criou uma imagem com modo '1' para coloracao de 1-bit.
width = disp.width
height = disp.height
image = Image.new('1', (width, height))

# Pega o objeto de desenho para 'desenhar' no display.
draw = ImageDraw.Draw(image)

# Desenha um retangulo preto para limpar a imagem.
draw.rectangle((0,0,width,height), outline=0, fill=0)

# Desenha algumas formas.
# Primeiramente define algumas constantes para habilitar o facil redicionamento das formas.
padding = -2
top = padding
bottom = height-padding
# Move da essquerda para direita para manter a 'posicao' atual de x para o desenhar das formas.
x = 0


# Carrega a fonte padrao.
font = ImageFont.load_default()

while True:
    
    temperatura = 30 # Esses valores seriam substituidos por aqueles lidos no servidor
    humidade = 70
    corrente = 0.07
    rpm = 110
    
    draw.rectangle((0,0,width,height), outline=0, fill=0)

    # Escreve 4 linhas de texto - e suas propriedades.

    draw.text((x, top),       "Temperatura: " + str(temperatura) + "oC",  font=font, fill=255)
    draw.text((x, top+8),     "Humidade: " + str(humidade) + "%", font=font, fill=255)
    draw.text((x, top+16),    "Corrente Eletrica: " + str(corrente) + "A",  font=font, fill=255)
    draw.text((x, top+25),    "RPM: " + str(rpm) + "r/min",  font=font, fill=255)
    print("Novas informacoes foram enviadas para o display!")

    # Exibe a imagem.
    disp.image(image)
    disp.display()
    time.sleep(.1)
    
    time.sleep(15) # Obter novos dados do servidor **teoricamente**
