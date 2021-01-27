#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader h = seg.header();
    if (h.syn) {
        ISN = h.seqno;
        status = _SYN_RECV;
    } else {
        cp = unwrap(h.seqno, ISN, cp) - 1;
    }
    if (status == _LISTEN) {
        return;
    }
    bool eof = false;
    if (h.fin) {
        eof = true;
        status = _FIN_RECV;
    }
    string str = static_cast<string>(seg.payload().str());
    _reassembler.push_substring(str, cp, eof);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (status == _LISTEN) {
        return {};
    } else {
        WrappingInt32 ack(0);
        size_t unasb = _reassembler.firstUnasb();
        if (_reassembler.unassembled_bytes() == 0) {
            ack = WrappingInt32(wrap(unasb + status, ISN));
        } else {
            ack = WrappingInt32(wrap(unasb + _SYN_RECV, ISN));
        }
        return {ack};
    }
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }