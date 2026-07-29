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

def enviar_calibracao_solo(seco, molhado):
    try:
        # Inicia a comunicação serial
        arduino = serial.Serial(PORTA_SERIAL, BAUD_RATE, timeout=2)
        time.sleep(2)  # Aguarda o reset do Arduino

        # Formata o comando esperado pelo logger.cpp
        comando = f"SET_SOIL={seco},{molhado}\n"
        print(f"[ENVIANDO] {comando.strip()}")
        
        # Envia os dados
        arduino.write(comando.encode('utf-8'))
        
        # Lê a resposta JSON do Arduino
        resposta = arduino.readline().decode('utf-8').strip()
        print(f"[RESPOSTA] {resposta}")
        
        arduino.close()
        
    except serial.SerialException as e:
        print(f"[ERRO] Falha de conexao: {e}")
    except ValueError:
        print("[ERRO] Por favor, insira valores inteiros validos.")

if __name__ == "__main__":
    valor = input("Digite o novo valor de RZero (ex: 76.43): ")
    enviar_comando_rzero(valor)
    print("--- Calibração do Sensor Capacitivo de Solo ---")
    try:
        valor_seco = int(input("Digite o valor ADC com o sensor SECO (no ar): "))
        valor_molhado = int(input("Digite o valor ADC com o sensor MOLHADO (na agua): "))
        enviar_calibracao_solo(valor_seco, valor_molhado)
    except ValueError:
        print("[ERRO] Os valores de calibração devem ser números inteiros!")