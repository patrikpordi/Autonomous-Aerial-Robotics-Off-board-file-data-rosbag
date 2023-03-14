from rosbags.rosbag1 import Reader
from rosbags.serde import deserialize_cdr
import numpy as np
from matplotlib import pyplot as plt
from pathlib import Path
from rosbags.serde import deserialize_cdr, ros1_to_cdr

from rosbags.typesys import get_types_from_msg, register_types

list_1=[]
list_2=[]


add_types = {}
msgpath = Path("/opt/ros/noetic/share/mavros_msgs/msg/PositionTarget.msg")
msgdef = msgpath.read_text(encoding='utf-8')
add_types.update(get_types_from_msg(msgdef,"mavros_msgs/msg/PositionTarget" ))

register_types(add_types)
# create reader instance and open for reading
with Reader('/home/pordipatrik/ws/2023-02-23-14-20-32.bag') as reader:
     # iterate over messages
    for connection, timestamp, rawdata in reader.messages():
        if connection.topic == '/mavros/local_position/pose':
            msg = deserialize_cdr(ros1_to_cdr(rawdata, connection.msgtype), connection.msgtype)
            list_1.append((msg.pose.position.x,msg.pose.position.y,msg.pose.position.z))
        if connection.topic == '/mavros/setpoint_raw/local':
            msg = deserialize_cdr(ros1_to_cdr(rawdata, connection.msgtype), connection.msgtype)
            list_2.append((msg.position.x,msg.position.y,msg.position.z))
arr_1=np.array(list_1)
arr_2=np.array(list_2)
x_axes=np.arange(0,6787/32,1/32)
x_axes2=np.arange(0,4527/20,1/20)

print(arr_2.shape)
print(x_axes.shape)
# print(arr_1)
# print(arr_2)

fig = plt.figure(figsize=(12, 6))

ax2 = fig.add_subplot(121)
ax = fig.add_subplot(122)

ax.set_title('Actual position')
ax.plot(x_axes,arr_1[:,:])
ax.set_xlim([0, 45])
ax.set_xlabel("Time [s]")
ax.set_ylabel("Position [m]")
ax.legend(['X', 'Y','Z'])

ax2.set_title('Target Position')
ax2.plot(x_axes2,arr_2[:,:])
ax2.set_xlim([0, 45])
ax2.set_xlabel("Time [s]")
ax2.set_ylabel("Position [m]")
ax2.legend(['X', 'Y','Z'])

plt.show()
#ax2.set_title('Truncated view')
##ax2.plot(x_axes,arr_2[:,:])
##ax2.set_xlim([0, 45])

##plt.show()
# plt.plot(x_axes2,arr_1[:,:])
# plt.xlim([0, 45])
# plt.show()
