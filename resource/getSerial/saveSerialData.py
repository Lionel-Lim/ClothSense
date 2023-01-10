import serial, time, csv

try:
    ser = serial.Serial('/dev/cu.usbmodem1401', 9600, timeout=1)
    dataSet = []
    while True:
        data = str(ser.readline())
        if len(data) > 10:
            dataSet = data[2:].split("/")
            print(dataSet)
        with open('/Users/dylim/Documents/CASA/CE/ClothSense/resource/getSerial/result/drying_3.csv','a') as f:
            writer = csv.writer(f)
            writer.writerow(dataSet)
        time.sleep(10)
except KeyboardInterrupt:
    pass    