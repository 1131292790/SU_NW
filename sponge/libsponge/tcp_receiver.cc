#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader h = seg.header();
    if(h.syn) {
        ISN = h.seqno;
        issetACK = true;
        return;
    }
    cp = unwrap(h.seqno, ISN, cp); 
    const Buffer b = seg.payload();
    string_view bStr = b.str();
    bool eof = h.fin;
    _reassembler.push_substring(bStr.data(), cp, eof);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if(!issetACK) {
        return {};
    } else {
        size_t unasb = _reassembler.firstUnasb();
        WrappingInt32 ack(wrap(unasb+1, ISN));
        return {ack};
    }
}

size_t TCPReceiver::window_size() const {
    return _capacity - _reassembler.stream_out().buffer_size();
}