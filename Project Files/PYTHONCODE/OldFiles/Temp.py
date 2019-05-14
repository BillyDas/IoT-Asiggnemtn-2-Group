import serial
import MySQLdb

device = "/dev/ttyACM0"

arduino = serial.Serial(device, 9600)
dbConn = MySQLdb.connect("localhost","admin","admin", "assignment2") or die("Could Not Connect To Db")

#data = arduino.readline()
#data = str(data)
#data = data.strip('\n')
#data = data.strip('\r')
#Temp = data
#print (Temp)

#with dbConn:
#    cursor = dbConn.cursor()
#    cursor.execute("Insert Into temperatureDB (Temp) VALUES ('%s') " % (Temp, FanStatus))
#    dbConn.commit()
#   cursor.close()

with dbConn:
    cursor = dbConn.cursor()
    cursor.execute("select TempLimit from TempLimit where temp_id=1;")
    row = cursor.fetchone()
    print(row)
    TempLimit = str(row)
    TempLimit = TempLimit.strip('L')
    TempLimit = TempLimit.strip(',')
    print(TempLimit)
    cursor.close()
    
    arduino.write(TempLimit) 





