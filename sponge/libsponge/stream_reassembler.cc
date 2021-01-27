#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

#define fi first
#define se second
using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), szUnasb(0), unread(0), unasb(0), eof2(-1), t({}) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    unread = _output.bytes_read();
    size_t idx = max(unasb, index);
    size_t endidx = min(index + data.length(), unread + _capacity);
    if (eof) {
        eof2 = index + data.length();
    }
    if (endidx <= idx) {
        if (unasb == eof2) {
            eof2 = -1;
            _output.end_input();
        }
        return;
    }
    vector<pair<pair<size_t, size_t>, string>> toInsert;
    for (auto c = t.begin(); c != t.end(); c++) {
        if (idx < c->fi.fi) {
            size_t start = idx, end = min(c->fi.fi, endidx);
            size_t len = end - start;
            toInsert.push_back({{start, end}, data.substr(idx - index, len)});
        }
        idx = max(idx, c->fi.se);
        if (c->fi.se >= endidx) {
            break;
        }
    }
    if (idx < endidx) {
        toInsert.push_back({{idx, endidx}, data.substr(idx - index, endidx - idx)});
    }
    for (auto i : toInsert) {
        t[i.fi] = i.se;
        szUnasb += i.se.length();
    }
    for (auto c = t.begin(); c != t.end() && c->fi.fi == unasb; t.erase(c++)) {
        _output.write(c->se);
        unasb += c->se.length();
        szUnasb -= c->se.length();
    }
    if (unasb == eof2) {
        eof2 = -1;
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return szUnasb; }

bool StreamReassembler::empty() const { return t.empty(); }

size_t StreamReassembler::firstUnasb() const { return unasb; }