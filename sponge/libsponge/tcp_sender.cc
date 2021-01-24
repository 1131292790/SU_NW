#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>
#include <map>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity)
    , now(0)
    , window(-1)
    , acked(0)
    , outstanding({}) {}

uint64_t TCPSender::bytes_in_flight() const { 
    uint64_t res = 0;
    for(auto c = outstanding.begin(); c != outstanding.end(); c++) {
        res += c->second.payload().size();
    }
    return res;
}

void TCPSender::fill_window() {
    if(window == -1) {
        TCPSegment seg;
        seg.header().seqno = _isn;
        seg.header().syn = true;
        if(_stream.eof()) {
            seg.header().fin = true;
            _segments_out.push(seg);
            outstanding.insert({now, seg});
            _next_seqno = 2;
            return;
        } else {
            _segments_out.push(seg);
            _next_seqno = 1;
            return;
        }
    }
    while (_next_seqno < acked + window) {
        TCPSegment seg;
        size_t len = min(TCPConfig::MAX_PAYLOAD_SIZE, min(_stream.buffer_size(), acked + window - _next_seqno));
        if(len == 0) {
            if(_stream.eof()) {
                seg.header().seqno = wrap(_next_seqno++, _isn);
                seg.header().fin = true;
                _segments_out.push(seg);
                return;
            } else {
                return;
            }
        }
        string data = _stream.read(len);
        seg.header().seqno = wrap(_next_seqno, _isn);
        seg.payload() = string(data);
        _segments_out.push(seg);
        _next_seqno += len;
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) { 
    size_t ackno64 = unwrap(ackno, _isn, acked);
    if(ackno64 > acked) {
        acked = ackno64;
        window = static_cast<int>(window_size);
        for(auto c = outstanding.begin(); c != outstanding.end(); ) {
            size_t seq64 = unwrap(c->second.header().seqno, _isn, acked); 
            if(seq64 + c->second.payload().size() <= acked) {
                outstanding.erase(c++);
            } else {
                break;
            }
        }
    }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    now += ms_since_last_tick;
    queue<pair<size_t,TCPSegment>> toTransmit;
    for(auto c = outstanding.begin(); c != outstanding.end(); ) {
        if(now - c->first >= _initial_retransmission_timeout) {
            toTransmit.push({now,c->second});
        }
    }
    // toTransmit
}

unsigned int TCPSender::consecutive_retransmissions() const { return {}; }

void TCPSender::send_empty_segment() {
    TCPSegment seg;
    seg.header().seqno = wrap(_next_seqno, _isn);
    _next_seqno++;
    _segments_out.push(seg);
}
