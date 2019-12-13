import requests as req
import serial as ser

url = "http://18.222.136.187/assgn12/api/index.php"
port = "COM5" #or whatever port you're Arduino uses
arduino = ser.Serial(port)

while True:
  try:
    # decode byte array into string, strip white space, split
    # CSV into list
    raw_data = str(arduino.readline().decode()).strip().split(",")
    print(raw_data)
    # keys should match $_POST names just like form
    data = { "apikey": "123", 
        "temperature": raw_data[0], 
        "pressure": raw_data[1],
        "humidity" : raw_data[2],
        "altitude" : raw_data[3],
        "lux" : raw_data[4],
        }
    print(data)
    response = req.post(url, data)

    print(response.text)
  except Exception as e:
    print(str(e))

