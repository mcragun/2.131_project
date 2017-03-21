
import PyCmdMessenger
import matplotlib.pyplot as plt


# Initialize an ArduinoBoard instance.  This is where you specify baud rate and
# serial timeot.  If you are using a non ATmega328 board, you might also need
# to set the data sizes (bytes for integers, longs, floats, and doubles).
arduino = PyCmdMessenger.ArduinoBoard("/dev/cu.usbmodem1411",baud_rate=38400)

# List of command names (and formats for their associated arguments). These must
# be in the same order as in the sketch.
commands = [["SweepFreqStart",""],
            ["SweepFreqStop",""],
            ["SetLed","s"],
            ["StatusMsg","s"],
            ["SendList", "f*"]]


# Initialize the messenger
c = PyCmdMessenger.CmdMessenger(arduino, commands)
print(c.receive())
# Send
#c.send("SetLed", 1)
plt.ion()
ydata = [0] * 100
ax1 = plt.axes()

# make plot
line, = plt.plot(ydata)
plt.ylim([0, 600])

c.send("SweepFreqStart")

# start data collection
for i in range(1000):
    data = c.receive()[1] # read data from serial


    #ymin = float(min(data))-10
    #ymax = float(max(data))+10
    #print ("YAY!")
    #plt.ylim([ymin,ymax])
    line.set_ydata(data)  # update the data
    plt.pause(0.000001) # update the plot
# Receive. Should give ["my_name_is",["Bob"],TIME_RECIEVED]

msg = c.receive()
print(msg)


# should give ["sum_is",[5],TIME_RECEIVED]
#print(msg)
