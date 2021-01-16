#include "byte_stream.hh"

#include <algorithm>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity): cap(capacity), rem(capacity), totRead(0), totWrite(0), eof2(false), buf("") {}

size_t ByteStream::write(const string &data) {
    size_t len = data.length();
    size_t writable = min(len, rem);
    buf += data.substr(0,writable);
    rem -= writable;
    totWrite += writable;
    return writable;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const { return buf.substr(0, len); }

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    buf.erase(0, len);
    rem += len;
    totRead += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string res = buf.substr(0, len);
    buf.erase(0, len);
    rem += len;
    totRead += len;
    return res;
}

void ByteStream::end_input() { eof2 = true; }

bool ByteStream::input_ended() const { return eof2; }

size_t ByteStream::buffer_size() const { return cap - rem; }

bool ByteStream::buffer_empty() const { return rem == cap; }

bool ByteStream::eof() const { return rem == cap && eof2; }

size_t ByteStream::bytes_written() const { return totWrite; }

size_t ByteStream::bytes_read() const { return totRead; }

size_t ByteStream::remaining_capacity() const { return rem; }
