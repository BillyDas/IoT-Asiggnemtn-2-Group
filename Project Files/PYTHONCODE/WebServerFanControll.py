import MySQLdb
import RPi.GPIO as GPIO
import re
import sys
import os


GPIO.setmode(GPIO.BCM)
GPIO.setup(14, GPIO.OUT)


dbConn = MySQLdb.connect("localhost","admin","admin", "assignment2") or die("Could Not Connect To Db")


with dbConn:
    cursor = dbConn.cursor()
    cursor.execute("select TempLimit from TempLimit where temp_id=1;")
    TempLimit = cursor.fetchone()
    cursor.close()

with dbConn:
    cursor = dbConn.cursor()
    cursor.execute("SELECT Temp FROM temperatureDB ORDER BY id DESC LIMIT 1")
    ActualTempUnStripped = cursor.fetchone()
    
    ActualTempV1 = str(ActualTempUnStripped)
    ActualTemp = re.sub("[^0-9.]", "", ActualTempV1)
#    ActualTempV2 = ActualTempV1.replace("'","")
#    ActualTempV3 = ActualTempV2.replace(',','')
#    ActualTempV4 = ActualTempV3.replace('(','')
#    ActualTempV5 = ActualTempV4.replace(')','')
    cursor.close()
    intTempLimit = float(TempLimit[0])

#    stringActualTemp = "".join(map(str, ActualTempV3))
    
    floatActualTemp = float(ActualTemp)
    


    if (floatActualTemp > intTempLimit):
        FanStatus = "On"
        print ("Fan Turned On")
        GPIO.output(14, GPIO.HIGH)
    else:
        FanStatus = "Off"
        print ("Fan Turned Off")
        GPIO.output(14, GPIO.LOW)
        
        
with dbConn:
    cursor = dbConn.cursor()
    data = (FanStatus)
    cursor.execute("Update FanStatus Set FanOnOrOff = %s Where id=1", [data])
    dbConn.commit()
    cursor.close()
        
    