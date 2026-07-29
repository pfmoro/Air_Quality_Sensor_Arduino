import serial
import time

# Configuração da porta (Ajuste para COM3 no Windows ou /dev/ttyUSB0 no Linux)
PORTA_SERIAL = 'COM5' 
BAUD_RATE = 115200

def enviar_comando_rzero(novo_rzero):
    try:
        # Abre a conexão Serial
        arduino = serial.Serial(PORTA_SERIAL, BAUD_RATE, timeout=2)
        time.sleep(2)  # Aguarda o Arduino reiniciar após a abertura da porta
        
        # Formata o comando
        comando = f"SET_RZERO={novo_rzero}\n"
        print(f"Enviando: {comando.strip()}")
        
        # Envia a string codificada em bytes
        arduino.write(comando.encode('utf-8'))
        
        # Aguarda a resposta de confirmação do Arduino
        resposta = arduino.readline().decode('utf-8').strip()
        print(f"Resposta do Arduino: {resposta}")
        
        arduino.close()
        
    except serial.SerialException as e:
        print(f"Erro de comunicação Serial: {e}")

if __name__ == "__main__":
    valor = input("Digite o novo valor de RZero (ex: 76.43): ")
    enviar_comando_rzero(valor)