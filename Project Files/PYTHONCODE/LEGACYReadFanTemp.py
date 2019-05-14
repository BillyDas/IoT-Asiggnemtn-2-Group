import serial
import MySQLdb
import re

device = "/dev/ttyACM0"
arduino = serial.Serial(device, 9600)

dbConn = MySQLdb.connect("localhost","admin","admin", "assignment2") or die("Could Not Connect To Db")

data = arduino.readline()
data = str(data)
data = data.strip('\n')
data = data.strip('\r')
Temp = data
print ("Temperature Added", Temp)

with dbConn:
    cursor = dbConn.cursor()
    cursor.execute("Insert Into temperatureDB (Temp) VALUES ('%s') " % (Temp))
    dbConn.commit()
    cursor.close()

    
    
    







