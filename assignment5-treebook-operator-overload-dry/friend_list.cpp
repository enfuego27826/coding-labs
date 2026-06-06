#include "friend_list.h"
#include<algorithm>
#include<utility>

void FriendList::add_friend(const std::string& name){
    _friends.push_back(name);
}

size_t FriendList::size() const{
    return _friends.size();
}

void FriendList::set_friend(size_t index, const std::string& name){
    _friends[index] = name;
}

std::ostream& operator<<(std::ostream& os, const FriendList& list){
    os << "[";

    for(size_t i = 0; i < list._friends.size(); ++i){
        os << list._friends[i] << ((i != list._friends.size()-1) ? "," : "");
    }

    os << "]";

    return os;
}