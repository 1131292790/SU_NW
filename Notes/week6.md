# Week 6
## Switch and Router
### I. Ethernet Switch
1. examine the header of each arriving frame
2. if the Ethernet DA (aka "MAC address") is in the forwarding table, forward the frame to the correct output port(s)
3. if the Ethernet DA is not in the table, broadcast the frame to all ports
4. entries in the table are `learned` by checking to see if the Ethernet SA of arriving packets are already in the table. If not , then add them
### II. Internet Router.
1. if the Ethernet DA of the arriving frame belongs to the router, accept the frame. Else drop it
2. examine the IP version number and length of the datagram
3. decrement TTL, update the IP header checksum
4. check if TTL == 0
5. if the IP DA is in the forwarding table, forward to the next hop
6. if IP DA doesn't match a table entry; a. if there is a Default Route entry, then forward to it (often a BGP router), b. else, drop the packets and send an ICMP message back to the source
7. find the Ethernet DA for the next hop router
8. create a new Ethernet frame and send it.
## Ethernet Frame Format
1. Preamble: trains clock-recovery circuits.
2. Start of Frame Delimiter: indicates start of frame
3. Destination Address: 48-bit globally unique Ethernet address assigned by manufacturer. 1b: unicast/multicast, 1b: local/global address
4. Type: indicates protocol of encapsulated data (e.g. IP = 0x0800)
5. Pad: zeroes used to ensure `minimum frame length`
6. Cyclic Redundancy Check: check sequence to detect bit errors
## MAC - Medium Access Control Protocols
### I. CSMA/CD - Carrier Sense MA / Collision Detection
1. CS: check if the line is quiet before transmitting
2. CD: detect collision as soon as possible. If a collision is detected, stop transmitting; wait a `random time` (binary exp backoff), then return to step 1
3. Host A is guaranteed to know about the collision while it is still transmitting if: P/R > 2L/c
## IXP
1. Autonomous Systems (AS's) usually connect to each other in an Internet eXchange Point(IXP)
### I. Why an IXP uses Ethernet Switches
1. IXP doesn't need to maintain routing tables
2. IXP doesn't need to exchange routing entries with its customers
3. IXP doesn't need to decide how packets are routed
4. merely provides "Link Layer" connectivity among its customers
### II. Ethernet in use today
1. almost all enterprises and campuses use Ethernet as a simple, quick, cheap, easy-to-manage way to interconnect hosts and WiFi APs inside a building. Routers are typically use to connect buildings together
2. Data-centers typically use an Ethernet switch to connect 48-64 servers together in each rack. Called "Top of Rack" or ToR switches. Routing is typically used to connect racks together.
## Fiber-Optical Links
### I. Characteristics
1. High Bandwidth: Lasers can be switched on/off very fast
2. Low Signal Loss: low dispersion or spreading
3. Electromagnetic Interference Immunity: wavelength division multiplexing (WDM)
## What determines the maximum data rate of a cable, fiber, wireless link, etc
### I. Shannon Capacity
1. max error-free rate we can transmit through a channel.
2. the maximum "data rate"
3. under some mild assumptions: Blog2(1 + S/N)
4. max cap depends only on BandWidth and Signal-to-Noise ratio.
## Analog Signals
### I. Sending 0s and 1s
1. FSK
2. ASK
3. PSK
### II. I/Q constellations
1. QPSK -> Quadrature Phase Shift Keying
2. 16-QAM -> Quadrature Amplitude Modulation
### III. Examples Today
1. ASK/OOK: Wired Ethernet
2. FSK: Bluetooth
3. BPSK: 802.11 abgn
4. QPSK: 802.11 abgn, LTE
5. 16-QAM: 802.11 abgn, LTE 
6. 64-QAM: 802.11 abgn, LTE, 5G
7. 256-QAM: 5G
8. 1024-QAM: Home Powerline Communication
9. 32768-QAM: ADSL (digital data over long telephone cables)
## Clocks
If the RX clock is p% slower than the TX clock, then: 𝑇𝑅𝑋 = 𝑇𝑇𝑋 * (1 + 𝑝/100)
After 0.5 / (10−2𝑝) bit times, the RX clock will miss a bit completely.
### I. Synchronous communication on network links
### II. Encoding for clock recovery
1. if clock is not sent separately, the data steram must have enough transitions for the receiver to determine when to sample the arriving data.
2. Advantages of Manchester encoding
- Guarantees one transition per bit period. 
- Ensures d.c. balance (i.e. equal numbers of hi and lo).
3. Disadvantages
- Doubles bandwidth needed in the worst case
4. Advantages of 4b/5b encoding
- More bandwidth efficient (only 25% overhead). 
- Allows extra codes to be used for control information
5. Disadvantages
- Fewer transitions makes clock recovery a little harder.
### III. Elastic buffer -> sizing of {}
1. Hold buffer nominally at B/2.
- At start of new packet, allow buffer to fill to B/2.
- Or, make sure buffer drains to B/2 before new packet.
2. Size buffer so that it does not overflow or underflow
before packet completes.
3. (Rtx > Rrx): Given inter packet gap, size B/2 for no overflow.
4. (Rrx > Rtx): Given max length packet, pick B/2 for no underflow