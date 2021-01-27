# Week 3
## Type of Delays
1. Propagation Delay: l/c
2. Packetization Delay: p/r
3. Total time to send a packet across a link: p/r + l/c
4. end to end delay = sum(t + Q(t))
## Simple Model of a Router Queue
Q(t) = A(t) - D(t)
1. Q(t): queuing packets at time t
2. A(t): arrival packets at time t
3. D(t): departure packets at time t
4. question: average occupancy - area of integration(A(t) - D(t)) / time length
## Queuing Algorithms
1. FIFO: No priorities, no guaranteed rates
2. Strict Priorities: high priority traffic "sees" a network with no low priority traffic
3. Weighted Fair Queueing (WFQ): scheduling flows in order of their bit-by-bit finishing times
## Guarantee the Delay
### I. Leaky Bucket
1. send packet if and only if we have tokens in the bucket
2. Number of bits that can be sent in any period of length t is bounded by: `σ + ρt`, also called a `(σ, ρ)` regulator.
3. If `ΦR > ρ` and `b > σ` then delay through the first router for all packets in the flow ≤ `b / ΦR`
4. Φ: channel weight; ρ: token rate; σ: bucket size
## Packet Switching
### I. Ethernet Switch
1. Examine the header of each arriving frame.
2. If the Ethernet DA is in the forwarding table, forward the frame to the correct output port(s).
3. If the Ethernet DA is not in the table, broadcast the frame to all ports (except the one through which the frame arrived).
4. Entries in the table are learned by examining the Ethernet SA of arriving packets
#### Address Lookup
hash table
### II. Internet Router
1. If the Ethernet DA of the arriving frame belongs to the router, accept the frame. Else drop it.
2. Examine the IP version number and length of the datagram.
3. Decrement the TTL, update the IP header checksum.
4. Check to see if TTL == 0.
5. If the IP DA is in the forwarding table, forward to the correct egress port(s) for the next hop.
6. Find the Ethernet DA for the next hop router.
7. Create a new Ethernet frame and send it.
#### Address Lookup
Longest Prefix Match (using trie)
### III. Generic/Abstract Lookups
1. <Match, Action>
2. Generalization of lookups and forwarding action in switches, routers, firewalls, etc.
3. This led to an abstraction for controlling switches using <match,action> rules, called OpenFlow