#include "user.h"
#include<algorithm>

/**
 * Creates a new User with the given name and no friends.
 */
User::User(const std::string& name)
  : _name(name)
  , _friends()
{
}

/**
 * Adds a friend to this User's list of friends.
 * @param name The name of the friend to add.
 */
void
User::add_friend(const std::string& name)
{
  _friends.add_friend(name);
}

/**
 * Returns the name of this User.
 */
std::string
User::get_name() const
{
  return _name;
}

/**
 * Returns the number of friends this User has.
 */
size_t
User::size() const
{
  return _friends.size();
}

/**
 * Sets the friend at the given index to the given name.
 * @param index The index of the friend to set.
 * @param name The name to set the friend to.
 */
void User::set_friend(size_t index, const std::string& name)
{
  _friends.set_friend(index,name);
}

/** 
 * STUDENT TODO:
 * The definitions for your custom operators and special member functions will go here!
 */

std::ostream& operator<<(std::ostream& os, const User& u){
  os << " User(name=" << u._name << ", friends=";

  os << u._friends << ")";
  return os;
}

User& User::operator+=(User& rhs){
  if(this == &rhs) return *this;

  _friends.add_friend(rhs.get_name());
  rhs._friends.add_friend(_name);

  return *this;
}

bool User::operator<(const User& rhs) const{
  return _name < rhs._name;
}