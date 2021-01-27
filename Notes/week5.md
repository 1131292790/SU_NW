# Week 5
## Routing Algo
### I. How does a Router know where to send a packet
Here are three ways
1. Flooding: Every router sends an arriving packet to every neighbor
2. Source Routing: End host lists the routers to visit along the way (in each packet)
3. Distributed Algorithm: Routers talk to each other and construct forwarding tables using a clever algorithm
### II. Distributed Bellman-Ford Algorithm
1. router maintains cost to each of other routers
2. router sends the newest cost to its neighbor every T seconds
3. when router learn a lower-cost path, it updates its table
### III. Dijkstra
## Autonomous Systems and Border Gateway Protocol
### I. AS
1. In the Internet, Autonomous Systems (AS’s) have Border Routers (orange). The border routers route packets to each other using the Border Gateway Protocol
2. Autonomous Systems (AS’s) usually connect to each other in an Internet eXchange Point (IXP)
### II. BGP
#### A. 
BGP routers advertise routes to their neighbors, containing:
1. A prefix
2. The list of AS’s indicating the path the packet will take to reach the prefix
#### B. 
1. Paths with loops are detected locally and ignored.
2. A BGP router may connect to several peers and receive multiple different advertised paths for the same prefix.
3. Local policies chosen by the AS administrator pick the preferred path.
#### C.
1. BGP neighbors (“peers”) establish a TCP connection.
2. The TCP connection is manually configured at both ends.
3. Neighbors send “keepalive” messages every 60 seconds.
4. BGP is sometimes called a “Path vector” algorithm.
5. It is not a link-state or a distance-vector routing protocol.
6. When an advertised path changes, the path vector is first “withdrawn”, then the new one is advertised.
### III. Tier 1 ISPs
1. Definition: A Tier 1 ISP has access to the entire Internet Region solely via its free and reciprocal peering agreements.
2. Definition: An Internet Region is a portion of the Internet network typically bounded by a country's geographical boundaries. Each Internet Region has its own set of "Tier 1 ISPs.

