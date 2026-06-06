#include <iostream>
#include <string>
#include <vector>

class FriendList{
public: 
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
    std::vector<std::string> _friends;
};