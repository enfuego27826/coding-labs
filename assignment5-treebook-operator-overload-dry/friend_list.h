#include <iostream>
#include <string>

class FriendList{
public: 
  FriendList();

  ~FriendList();
  FriendList(const FriendList& FriendList);
  FriendList& operator=(FriendList FriendList);
  FriendList(FriendList&& FriendList) = delete;
  FriendList& operator=(FriendList&& FriendList) = delete;


  void add_friend(const std::string& name);
  std::string get_name() const;
  size_t size() const;
  void set_friend(size_t index, const std::string& name);

  /** 
   * STUDENT TODO:
   * Your custom operators and special member functions will go here!
   */

   friend std::ostream& operator<<(std::ostream& os, const FriendList& u);
   friend void swap(FriendList& first, FriendList& second) noexcept;

private:
    std::string* _friends;
    size_t _size;
    size_t _capacity;
};