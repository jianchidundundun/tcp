#include "stream_reassembler.hh"
#include <limits>
#include <iomanip>
#include <iostream>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity),_eof_index(std::numeric_limits<size_t>::max()),_unassemble_byte_count(0), _current_idx(0),_stream(capacity){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {

    
    //整个data 放进out 的流程是   data -> unassembled string (_strea) -> _output

    size_t start_index = max(_current_idx, index);
    // 结束index 是在eof 文件结尾index， 
    //_current_idx + _capacity - _output.buffer_size()  这个数表明了_stream 最多能从现在index 还能装多少bytes
    size_t end_index = min(_eof_index, min(index + data.size(), _current_idx + _capacity - _output.buffer_size()));

    if (eof) {
        _eof_index = min(_eof_index, index + data.size());
    }
    //data_index 的设置： 如果start_index = index ，就表明当前index 大于current_index ，则 _current_index 和 当前
    //数据没有重叠，可以直接开始,
    // 如果 start_index = _current_index， 就表明_curretn_index 大于传入数据的index
    //表明当前传入数据的有一部分已经放进_unassembled里边了，所以从_current - index 
    //也就是没有重叠的第一个byte 开始
    for (size_t stream_index = start_index, data_index = start_index - index; stream_index < end_index ; ++stream_index, ++data_index){
        // 注意下边的_stream_pointer 是指针
        auto &_stream_pointer = _stream[stream_index % _capacity];
        if (_stream_pointer.second == true) {
            // if (_stream_pointer.first != data[data_index])
            //     throw runtime_error("StreamReassembler::push_substring: Inconsistent substrings!");
            continue;
        }else{
            _stream_pointer = make_pair(data[data_index], true);
            
            
            ++_unassemble_byte_count;
        }
    }

    // 下一个步骤就是把_unassembled 里边从从_current_idnex 开始所有连续的char 
    // 全部装配进_output
    string temp ;
    //_stream 是一个环形vector 为了防止_current_index > capacity 所以就需要在index 上mod 一下capacity

    //print_stream(_stream, _capacity);
    while (_stream[_current_idx % _capacity].second == true && _current_idx < _eof_index)
    {
        /* code */
        temp.push_back(_stream[_current_idx % _capacity].first);

        // 在写入完之后还需要清理当前的_unassembled 因为之后如果有存入_unassembled 的时候会有混淆，capacity 
        // 肯定是远远小于所有传入数据的total data size的




        //0 代表空的char
        _stream[_current_idx % _capacity] = {0,false};
        ++_current_idx;
        --_unassemble_byte_count;
    }
    _output.write(temp);

    // 不需要清空temp ，因为下个调用就又会自动声明了
    // _current_index 就是当前数据在整个data 的里边的index，而不是在_stream 里边的index
    if (_current_idx == _eof_index) {
        _output.end_input();
    }


    
    


    
    
    
    
}

size_t StreamReassembler::unassembled_bytes() const { return _unassemble_byte_count; }

bool StreamReassembler::empty() const { return _unassemble_byte_count == 0; }




void print_stream(const std::vector<std::pair<char, bool>>& stream, size_t capacity) {
    std::cout << "Stream contents: ";
    for (size_t i = 0; i < capacity; ++i) {
        if (stream[i].second) {
            std::cout << stream[i].first;
        } else {
            continue;
        }
    }
    std::cout << std::endl;
}