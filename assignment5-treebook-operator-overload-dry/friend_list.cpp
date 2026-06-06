#include "friend_list.h"
#include<algorithm>
#include<utility>

FriendList::FriendList()
    : _friends(nullptr)
    , _size(0)
    , _capacity(0)
{
}

FriendList::~FriendList()
{
    delete[] _friends;
}

FriendList::FriendList(const FriendList& other)
    : _friends(nullptr)
    , _size(other._size)
    , _capacity(other._capacity)
{
    if(_capacity == 0) return;

    _friends = new std::string[_capacity];
    
    for(size_t i = 0; i < _size; ++i){
        _friends[i] = other._friends[i];
    }
}

void swap(FriendList& first, FriendList& second) noexcept{
    using std::swap;

    swap(first._friends, second._friends);
    swap(first._size, second._size);
    swap(first._capacity, second._capacity);
}


FriendList& FriendList::operator=(FriendList other){
    swap(*this, other);
    return *this;
}

void FriendList::add_friend(const std::string& name){
    if (_size == _capacity) {
        _capacity = 2 * _capacity + 1;

        std::string* newFriends = new std::string[_capacity];

        for (size_t i = 0; i < _size; ++i){
            newFriends[i] = _friends[i];
        }

        delete[] _friends;
        _friends = newFriends;
    }

    _friends[_size++] = name;
}

size_t FriendList::size() const{
    return _size;
}

void FriendList::set_friend(size_t index, const std::string& name){
    _friends[index] = name;
}

std::ostream& operator<<(std::ostream& os, const FriendList& list){
    os << "[";

    for(size_t i = 0; i < list._size; ++i){
        os << list._friends[i] << ((i != list._size-1) ? "," : "");
    }

    os << "]";

    return os;
}