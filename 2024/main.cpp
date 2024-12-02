#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <ctype.h>
#include <algorithm>
#include <optional>
#include <cassert>

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename K, typename V>
const V & idx(const  std::map <K,V> & m, const K & key, const V & defval ) {
   typename std::map<K,V>::const_iterator it = m.find( key );
   if ( it == m.end() ) {
      return defval;
   }
   else {
      return it->second;
   }
}

std::string advent1(std::vector<std::string> lines) {
  std::vector<int> left_nums, right_nums;

  for (const std::string& line : lines) {
    int center = line.find(" ");
    int left = std::stoi(line.substr(0, center));
    int right = std::stoi(line.substr(center));

    left_nums.push_back(left);
    right_nums.push_back(right);
  }

  std::sort(left_nums.begin(), left_nums.end());
  std::sort(right_nums.begin(), right_nums.end());

  assert(left_nums.size() == right_nums.size());

  // P1
  int sum = 0;
  for (size_t i = 0; i < left_nums.size(); i++) {
    sum += std::abs(left_nums[i] - right_nums[i]);
  }

  assert(sum == 1579939);

  // P2
  std::map<int, int> right_freq;
  for (int i : right_nums) {
    right_freq[i] = idx(right_freq, i, 0) + 1;
  }

  sum = 0;
  for (int i : left_nums) {
    sum += i * idx(right_freq, i, 0);
  }

  assert(sum == 20351745);

  return std::to_string(sum);
}

bool advent2_report_is_valid(std::vector<int> report) {
  assert(report.size() != 0 && report.size() != 1);

  int direction = sign(report[1] - report[0]);
  for (size_t i = 0; i < report.size() - 1; i++) {
    int delta = report[i + 1] - report[i];
    if (sign(delta) != direction) {
        return false;
    }

    int mag = std::abs(delta);
    if (mag > 3 || mag < 1) {
      return false;
    }
  }

  return true;
}


std::string advent2(std::vector<std::string> lines) {
  std::vector<std::vector<int>> reports;
  for (const std::string& line : lines) {
    std::vector<int> report;
    
    size_t i = 0;
    while (i < line.size()) {
      size_t end = line.find(" ", i);
      if (end == std::string::npos) {
        end = line.size();
      }

      report.push_back(std::stoi(line.substr(i, end)));
      i = end + 1;
    }

    reports.push_back(report);
  }

  // P1
  int count = 0;
  for (const std::vector<int>& report : reports) {
    if (advent2_report_is_valid(report)) {
      count++;
    }
  }

  assert(count == 585);

  // P2
  count = 0;
  for (const std::vector<int>& report : reports) {
    assert(report.size() != 0 && report.size() != 1);

    for (size_t exclude = 0; exclude <= report.size(); exclude++) {
      std::vector<int> report_copy = report;
      if (exclude != report.size()) {
        report_copy.erase(report_copy.begin() + exclude);
      }

      if (advent2_report_is_valid(report_copy)) {
        count++;
        break;
      }
    }
  }

  return std::to_string(count);
}

const std::string INPUT_FILE_POSTFIX = "_input.txt";
int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: program [advent day]" << std::endl;
  }

  char* dayStr = argv[1];

  std::string inputFileName = dayStr + INPUT_FILE_POSTFIX;
  std::ifstream inputFile(inputFileName);
  if (!inputFile.is_open()) {
      std::cerr << "Error opening file!" << std::endl;
      return 1;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(inputFile, line)) {
    lines.push_back(line);
  }

  int day = std::stoi(dayStr);
  std::string output;
  switch (day) {
    case 1:
      output = advent1(lines);
      break;
    case 2:
      output = advent2(lines);
      break;
    default:
      std::cerr << "Not a valid advent day" << std::endl;
      return 1;
  }

  std::cout << "Result: " << output << std::endl;

  return 0;
}
