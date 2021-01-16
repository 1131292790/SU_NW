# Week 2
## TCP Datagram Sturcture
### I. Source Port Address
16 bit field that holds the port address of the application that is sending the data segment.
### II. Destination Port Address
16 bit field that holds the port address of the application in the host that is receiving the data segment.
### III. Sequence Number
32 bit field that holds the sequence number, i.e, the byte number of the first byte that is sent in that particular segment. It is used to reassemble the message at the receiving end if the segments are received out of order.
### IV. Acknowledgement Number
32 bit field that holds the acknowledgement number, i.e, the byte number that the receiver `expects to receive next`. It is an acknowledgment for the previous bytes being received successfully.
### V. Header Length (HLEN)
This is a 4 bit field that indicates the length of the TCP header by number of 4-byte words in the header, i.e, if the header is of 20 bytes(min length of TCP header), then this field will hold 5 (because 5 x 4 = 20) and the maximum length: 60 bytes, then it’ll hold the value 15(because 15 x 4 = 60). Hence, the value of this field is always between 5 and 15.
### VI. Control flags
These are 6 1-bit control bits that control connection establishment, connection termination, connection abortion, flow control, mode of transfer etc. Their function is:
- URG: Urgent pointer is valid
- ACK: Acknowledgement number is valid( used in case of cumulative acknowledgement)
- PSH: Request for push
- RST: Reset the connection
- SYN: Synchronize sequence numbers
- FIN: Terminate the connection
### VII. Window size
This field tells the window size of the sending TCP in bytes.
### VIII. Checksum
This field holds the checksum for error control. It is mandatory in TCP as opposed to UDP.
### IX. Urgent pointer
This field (valid only if the URG control flag is set) is used to point to data that is urgently required that needs to reach the receiving process at the earliest. The value of this field is added to the sequence number to get the byte number of the last urgent byte.
## Source
1. https://www.geeksforgeeks.org/services-and-segment-structure-in-tcp/