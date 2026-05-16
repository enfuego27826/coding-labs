/*
 * Assignment 1: Marriage Pact
 * Adapted by Tinkercademy from Stanford CS106L
 * (originally by Haven Whitney, with modifications by Fabio Ibanez
 * & Jacob Roberts-Baca).
 *
 * Complete each STUDENT TODO below. Read the README carefully — the
 * requirements there (ranges, projections, sample, reserve, no raw
 * for-loops in find_matches, iterator-safe erase in run_mixer) are
 * part of the assignment, not optional polish.
 */

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <random>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/**
 * Reads `filename` line by line and returns the applicants.
 *
 * Requirements:
 *   - Take `filename` as `const std::string&`.
 *   - Call `reserve()` before populating, with a sensible capacity.
 *     Justify your choice in short_answer.txt.
 */
std::vector<std::string> get_applicants(const std::string& filename) {
  // STUDENT TODO: Implement this function.
  std::ifstream filein(filename);

  if(!filein.is_open()){
    throw std::runtime_error("Unable to open file!");
  }

  int count = std::count(std::istreambuf_iterator<char>(filein),std::istreambuf_iterator<char>(),'\n');

  filein.clear();
  filein.seekg(0);

  std::vector<std::string> applicants;
  applicants.reserve(count);

  std::string line;
  while(getline(filein,line)){
    applicants.push_back(line);
  }

  return applicants;
}

/**
 * Returns the initials of `name`, uppercased.
 *   e.g. initials("Marceline McMillan") == "MM"
 *
 * Requirements:
 *   - Parameter must be `std::string_view` (no allocation).
 */
std::string initials(std::string_view name) {
  // STUDENT TODO: Implement this function.
  std::stringstream ss{std::string(name)};
  std::string word;
  std::string initials;

  while(ss >> word){
    initials += std::toupper(word[0]);
  }

  return initials;
}

/**
 * Returns every applicant in `students` who shares initials with `name`.
 *
 * Requirements:
 *   - No raw `for` loops. Use std::ranges::copy_if (or views::filter
 *     piped into a vector). Use a projection where it makes the call
 *     clearer.
 *   - Take `students` as `const std::vector<std::string>&`.
 */
std::vector<std::string> find_matches(std::string_view name,
                                      const std::vector<std::string>& students) {
  // STUDENT TODO: Implement this function.
  std::string target = initials(name);
  std::vector<std::string> matches;

  std::ranges::copy_if(students,std::back_inserter(matches),
                      [&](std::string student){
                        return initials(student) == target;
                      });

  return matches;
}

/**
 * Returns one randomly-chosen match, or "NO MATCHES FOUND." if empty.
 *
 * Requirements:
 *   - Use std::sample with a seeded std::mt19937.
 *   - Do NOT use pop_back() or rand() % size.
 */
std::string get_match(const std::vector<std::string>& matches) {
  // STUDENT TODO: Implement this function.
  if(matches.empty()){
    return "NO MATCHES FOUND.";
  }

  std::mt19937 rng(std::random_device{}());
  std::vector<std::string> result;
  
  std::sample(matches.begin(),matches.end(),std::back_inserter(result),1,rng);

  return result.front();
}

/**
 * Runs a multi-round mixer. In each round, scan the remaining
 * applicants left-to-right; for each applicant, look for another
 * applicant with the same initials still in the pool. If found,
 * pair them, remove both from `applicants`, and record the pair.
 * Continue rounds until a full pass yields no new pairs.
 *
 * `applicants` is mutated: paired names are removed. Whatever is
 * left over at the end is unpaired.
 *
 * Requirements:
 *   - The naive "iterate and erase as you go" approach WILL invalidate
 *     your iterator. You must handle this — see the README for the
 *     three acceptable strategies — and document your choice in
 *     short_answer.txt.
 */
std::vector<std::pair<std::string, std::string>>
run_mixer(std::vector<std::string>& applicants) {
  // STUDENT TODO: Implement this function.

  std::vector<std::pair<std::string,std::string>> result;
  auto it = applicants.begin();

  while(it != applicants.end()){
    std::string target = initials(*it);

    auto match = std::find_if(
      std::next(it),
      applicants.end(),
      [&](std::string student){
        return initials(student) == target;
      }
    );

    if(match == applicants.end()){
      it = next(it);
      continue;
    }

    result.push_back({*it,*match});

    applicants.erase(match);
    it = applicants.erase(it);
  }

  return result;
}

// /* #### Please don't remove this line! #### */
#include "tests/utils.hpp"

