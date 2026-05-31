#include "spellcheck.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>
#include "utils.cpp"

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(std::string& source) {
  auto vec = find_all(source.begin(),
                      source.end(),
                      [](auto curr){
                        return std::isspace(curr);
                      });

  Corpus result;

  std::transform(vec.begin(),
                std::prev(vec.end()),
                std::next(vec.begin()),
                std::inserter(result,result.end()),
                [&](auto it1, auto it2){
                  return Token(source,it1,it2);
                });
  
  std::erase_if(result, 
                [](auto curr){
                  return curr.content.empty();
                });

  return result;
}

std::set<Misspelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  /* TODO: Implement this method */
  auto view = source
            | std::ranges::views::filter(
                                      [&dictionary](auto curr){
                                        return (dictionary.count(curr.content) == 0);
                                      })
            | std::ranges::views::transform(
              [&dictionary](auto curr){
                auto suggestions = dictionary
                                | std::ranges::views::filter(
                                  [curr](auto word){
                                    return (levenshtein(curr.content,word) <= 1);
                                  }
                                  );

                std::set<std::string> sugg(suggestions.begin(),suggestions.end());

                Misspelling res{
                  .token = curr,
                  .suggestions = sugg
                };

                return res;
              })
            | std::ranges::views::filter(
              [](auto miss){
                return !(miss.suggestions.empty());
              }
            );

  
  return std::set<Misspelling>(view.begin(),view.end());
};
