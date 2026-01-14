import serial
import schedule
import time
from datetime import datetime


def log_message(message):
    log_file_path = "./log.txt"
    with open(log_file_path, "a") as log_file:
        log_file.write(f"{datetime.now()} - {message}\n")

def open_serial_port():
    try:
        ser = serial.Serial(port='COM13', baudrate=115200, timeout=1)
        if ser.is_open: 
            # log_message(f"Serial port {ser.port} opened successfully.")
            print(f"Serial port {ser.port} opened successfully.")
            time.sleep(2)
            ser.close()
            # log_message(f"Serial port {ser.port} closed successfully after 2 seconds.")
            print(f"Serial port {ser.port} closed successfully after 2 seconds.")
    except Exception as e:
        log_message(f"Error opening serial port: {e}")

# Log the scheduled times at the start of the program

schedule.every().day.at("00:35").do(open_serial_port) 
schedule.every().day.at("02:55").do(open_serial_port)
schedule.every().day.at("05:40").do(open_serial_port)
schedule.every().day.at("08:57").do(open_serial_port)  
schedule.every().day.at("11:58").do(open_serial_port)  
schedule.every().day.at("14:59").do(open_serial_port)  
schedule.every().day.at("18:19").do(open_serial_port)  
schedule.every().day.at("21:33").do(open_serial_port)  
schedule.every().day.at("21:34").do(open_serial_port)  

print("Scheduler is running...")
while True:
    try:
        schedule.run_pending()
        time.sleep(1)
    except KeyboardInterrupt:
        print("Scheduler stopped.")