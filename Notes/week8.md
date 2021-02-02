# Week 8
## Abstractions
1. IP forwarding abstraction
2. firewall abstraction
3. VLAN abstraction
4. VPN abstraction
5. NAT abstraction
## Virtual Network: The abstraction
abstration: The user, application (and possibly the network administrator too) cannot tell if the network is physical or virtual. \
A set of VMs operating as if connected to the same physical network.
1. Typically belonging to the same tenant.
2. VMs communicate with each other using their own address space.
3. Virtual networks are isolated from each other: They cannot communicate, except through a gateway
4. VMs can migrate to a different server without changing IP address
5. A virtual network has a SLO expressed as a desired quality of service (e.g. data rate, reliability, latency)
6. A VM can operate as if on the tenant’s home network.
7. Used for containers too
## Mechanism: Tags, Tunnels or Translation
### I. Tags
Switches contain a forwarding table per tenant.
- Tag in every packet indicates the tenant and therefore the forwarding table to use
- But: We need to change the switches to recognize the tag and forward based on it.
### II. Translation
Use NAT, with port numbers identifying VMs
- But: Both ends behind NATs, therefore need NAT traversal everywhere – complicated.
- But: With thousands of VMs per server, quickly run out of port numbers for mapping.
### III. Tunnel
Create tunnel between every pair of servers. Forward traffic between VMs through the tunnel.
- But: We need to change switches to create tunnels.
- But: Server will receive packets for all addresses used by its VMs.
## How it is done in Virtualized Data Centers?
1. Use the software “vSwitch” in every server
2. Forward packets in tunnels between vSwitches
## Network Function Virtualization (NFV)
### I. Benefits of NFV
- Moves expensive hardware to software
- Easier to scale out on demand
- Easier to add new functions over time