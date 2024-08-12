#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : _buffer(capacity + 1)
    , _head(0)
    , _tali(capacity)
    , _capacity(capacity)
    , _input_end_flag(false)
    , _byte_w(0)
    , _byte_r(0) {}

size_t ByteStream::write(const string &data) {
    auto ret = min(data.size(), remaining_capacity());
    for (size_t i = 0; i < ret; i++) {
        _tali = (_tali + 1) % (_capacity + 1);
        _buffer[_tali] = data[i];
    }
    _byte_w += ret;
    return ret;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    auto length = min(len, buffer_size());
    string result;
    for (size_t i = 0; i < length; i++) {
        result.push_back(_buffer[(_head + i) % (_capacity + 1)]);
    }

    return result;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    DUMMY_CODE(len);
    // 这里是为了要对比要pop out 的长度和已有长度的对比，如果
    //  已有的长度小于pop out 的长度直接移除已有的长度就可以
    int length = min(len, buffer_size());
    _head = (_head + length) % (_capacity + 1);
    _byte_r += length;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    auto length = min(len, buffer_size());
    string result = peek_output(length);
    pop_output(length);
    return result;
}

void ByteStream::end_input() { _input_end_flag = true; }

bool ByteStream::input_ended() const { return _input_end_flag; }

size_t ByteStream::buffer_size() const { return (_tali - _head + 1 + _capacity + 1) % (1 + _capacity); }

bool ByteStream::buffer_empty() const { return buffer_size() == 0; }

bool ByteStream::eof() const { return _input_end_flag && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _byte_w; }

size_t ByteStream::bytes_read() const { return _byte_r; }

size_t ByteStream::remaining_capacity() const { return _capacity - buffer_size(); }
