#Use this script to quickly verify that signals are being read correctly
#and from the anticipated inputs on the LabJack

import sys
import traceback
from datetime import datetime

import u6

if __name__ == "__main__":
    d = u6.U6()
    d.getCalibrationData()
    #Configure LabJack stream variables
    #See labjack.com/support/datasheets/u6/low-level-function-reference/streamconfig
    #for definitions of streamConfig variables

    d.configIO(NumberTimersEnabled=4, TimerCounterPinOffset=0)
    d.getFeedback(u6.Timer0Config(8))
    d.getFeedback(u6.Timer1Config(8))
    d.getFeedback(u6.Timer2Config(8))
    d.getFeedback(u6.Timer3Config(8))
    LastCount1, = d.getFeedback(u6.Timer0())
    LastCount2, = d.getFeedback(u6.Timer2())

    try:
        while True:
            temp, = d.getFeedback(u6.Timer2())
            if temp > 2147483647:
                temp = temp - 4294967294
            # temp = '{:032b}'.format(temp)
            print (temp)
    except KeyboardInterrupt:
        print('LabJack shutdown initiated.')
        d = u6.U6()
        d.streamStop()
        d.close()
        print('Labjack shutdown complete.')
        pass

# # MAX_REQUESTS is the number of packets to be read.
# MAX_REQUESTS = 30
# # SCAN_FREQUENCY is the scan frequency of stream mode in Hz
# SCAN_FREQUENCY = 2000
#
# # Open LabJack
# d = u6.U6()
#
# # For applying the proper calibration to readings.
# d.getCalibrationData()
#
# print("Configuring U6 stream")
#
# d.streamConfig(NumChannels=2, ChannelNumbers=[0, 1], ChannelOptions=[0, 0], SettlingFactor=1, ResolutionIndex=1, ScanFrequency=SCAN_FREQUENCY)
#
# try:
#     print("Start stream")
#     d.streamStart()
#     start = datetime.now()
#     print("Start time is %s" % start)
#
#     missed = 0
#     dataCount = 0
#     packetCount = 0
#
#     for r in d.streamData():
#         if r is not None:
#             # Our stop condition
#             if dataCount >= MAX_REQUESTS:
#                 break
#
#             if r["errors"] != 0:
#                 print("Errors counted: %s ; %s" % (r["errors"], datetime.now()))
#
#             if r["numPackets"] != d.packetsPerRequest:
#                 print("----- UNDERFLOW : %s ; %s" %
#                       (r["numPackets"], datetime.now()))
#
#             if r["missed"] != 0:
#                 missed += r['missed']
#                 print("+++ Missed %s" % r["missed"])
#
#             # Comment out these prints and do something with r
#             print("Average of %s AIN0 readings: %.4f" %
#                   (len(r["AIN0"]), sum(r["AIN0"])/len(r["AIN0"])))
#
#             dataCount += 1
#             packetCount += r['numPackets']
#         else:
#             # Got no data back from our read.
#             # This only happens if your stream isn't faster than the USB read
#             # timeout, ~1 sec.
#             print("No data ; %s" % datetime.now())
# except:
#     print("".join(i for i in traceback.format_exc()))
# finally:
#     stop = datetime.now()
#     d.streamStop()
#     print("Stream stopped.\n")
#     d.close()
#
#     sampleTotal = packetCount * d.streamSamplesPerPacket
#
#     scanTotal = sampleTotal  # sampleTotal / NumChannels
#     print("%s requests with %s packets per request with %s samples per packet = %s samples total." %
#           (dataCount, (float(packetCount)/dataCount), d.streamSamplesPerPacket, sampleTotal))
#     print("%s samples were lost due to errors." % missed)
#     sampleTotal -= missed
#     print("Adjusted number of samples = %s" % sampleTotal)
#
#     runTime = (stop-start).seconds + float((stop-start).microseconds)/1000000
#     print("The experiment took %s seconds." % runTime)
#     print("Actual Scan Rate = %s Hz" % SCAN_FREQUENCY)
#     print("Timed Scan Rate = %s scans / %s seconds = %s Hz" %
#           (scanTotal, runTime, float(scanTotal)/runTime))
#     print("Timed Sample Rate = %s samples / %s seconds = %s Hz" %
#           (sampleTotal, runTime, float(sampleTotal)/runTime))
