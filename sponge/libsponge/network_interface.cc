#include "network_interface.hh"

#include "arp_message.hh"
#include "ethernet_frame.hh"

#include <iostream>

// Dummy implementation of a network interface
// Translates from {IP datagram, next hop address} to link-layer frame, and from link-layer frame to IP datagram

// For Lab 5, please replace with a real implementation that passes the
// automated checks run by `make check_lab5`.

// You will need to add private members to the class declaration in `network_interface.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of the interface
//! \param[in] ip_address IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface(const EthernetAddress &ethernet_address, const Address &ip_address)
    : _ethernet_address(ethernet_address), _ip_address(ip_address) {
    cerr << "DEBUG: Network interface has Ethernet address " << to_string(_ethernet_address) << " and IP address "
         << ip_address.ip() << "\n";
}

//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically a router or default gateway, but may also be another host if directly connected to the same network as the destination)
//! (Note: the Address type can be converted to a uint32_t (raw 32-bit IP address) with the Address::ipv4_numeric() method.)
void NetworkInterface::send_datagram(const InternetDatagram &dgram, const Address &next_hop) {
    // convert IP address of next hop to raw 32-bit representation (used in ARP header)
    const uint32_t next_hop_ip = next_hop.ipv4_numeric();
    
    EthernetFrame ef;
    ef.header().src = _ethernet_address;
    auto iter = cache.find(next_hop_ip);
    if(iter != cache.end()) {
        ef.header().type = EthernetHeader::TYPE_IPv4;
        ef.header().dst = iter->second.first;
        ef.payload() = dgram.serialize();
        _frames_out.push(ef);
    }
    if(cooldown.find(next_hop_ip) != cooldown.end()) {
        return;
    }
    ef.header().type = EthernetHeader::TYPE_ARP;
    ef.header().dst = ETHERNET_BROADCAST;
    ARPMessage am;
    am.opcode = ARPMessage::OPCODE_REQUEST;
    am.sender_ethernet_address = _ethernet_address;
    am.sender_ip_address = _ip_address.ipv4_numeric();
    am.target_ip_address = next_hop_ip;
    ef.payload() = am.serialize();
    pending.insert({next_hop_ip, dgram});
    cooldown.insert({next_hop_ip, now});
    _frames_out.push(ef);
}

//! \param[in] frame the incoming Ethernet frame
optional<InternetDatagram> NetworkInterface::recv_frame(const EthernetFrame &ef) {
    if((ef.header().type != EthernetHeader::TYPE_ARP && 
        ef.header().type != EthernetHeader::TYPE_IPv4) || 
        (ef.header().dst != ETHERNET_BROADCAST &&
        ef.header().dst != _ethernet_address)) {
        return {};
    }
    if(ef.header().type == EthernetHeader::TYPE_IPv4) {
        InternetDatagram id;
        ParseResult pr = id.parse(ef.payload().buffers().front());
        if(pr != ParseResult::NoError) {
            return {};
        }
        return {id};
    }
    ARPMessage am;
    ParseResult pr = am.parse(ef.payload().buffers().front());
    if(pr != ParseResult::NoError) {
        return {};
    }
    auto iter = cache.find(am.sender_ip_address);
    if(iter != cache.end()) {
        cache.erase(iter);
    }
    for(auto c = pending.lower_bound(am.sender_ip_address); c != pending.end() && c->first == am.sender_ip_address; ) {
        EthernetFrame ef3;
        ef3.header().src = _ethernet_address;
        ef3.header().type = EthernetHeader::TYPE_IPv4;
        ef3.header().dst = am.sender_ethernet_address;
        ef3.payload() = c->second.serialize();
        _frames_out.push(ef3);
        pending.erase(c++);
    }
    cache.insert({am.sender_ip_address, {am.sender_ethernet_address, now}});
    if(am.opcode != ARPMessage::OPCODE_REQUEST) {
        return {};
    }
    EthernetFrame ef2;
    ef2.header().src = _ethernet_address;
    ef2.header().type = EthernetHeader::TYPE_ARP;
    ef2.header().dst = ETHERNET_BROADCAST;
    ARPMessage am2;
    am2.opcode = ARPMessage::OPCODE_REPLY;
    am2.sender_ethernet_address = _ethernet_address;
    am2.sender_ip_address = _ip_address.ipv4_numeric();
    am2.target_ethernet_address = am.sender_ethernet_address;
    am2.target_ip_address = am.sender_ip_address;
    ef2.payload() = am2.serialize();
    _frames_out.push(ef2);
    return {};
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void NetworkInterface::tick(const size_t ms_since_last_tick) {
    now += ms_since_last_tick;
    for(auto c = cache.begin(); c != cache.end(); ) {
        if(c->second.second + 30000 >= now) {
            cache.erase(c++);
            continue;
        }
        c++;
    }
    for(auto c = cooldown.begin(); c != cooldown.end(); ) {
        if(c->second + 5000 >= now) {
            cooldown.erase(c++);
            continue;
        }
        c++;
    }
}
