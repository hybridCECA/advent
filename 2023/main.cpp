#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <ctype.h>
#include <algorithm>
#include <optional>
#include <cassert>

std::optional<int> getDigitName(std::string line, size_t pos) {
  const std::map<std::string, int> NAME_TO_VALUE = {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
  };

  size_t maxLen = line.size() - pos;

  for (size_t len = 3; len <= std::min(5UL, maxLen); len++) {
    std::string sub = line.substr(pos, len);
    if (NAME_TO_VALUE.count(sub)) {
      return NAME_TO_VALUE.at(sub);
    }
  }

  return std::nullopt;
}

std::string advent1(std::vector<std::string> lines) {
  uint64_t sum = 0;

  for (const std::string& line : lines) {
    std::optional<int> firstDigit = std::nullopt;
    int lastDigit;
    for (size_t i = 0; i < line.size(); i++) {
      std::optional<int> digitFromName = getDigitName(line, i);
      const char c = line.at(i);
      int digit;
      if (isdigit(c)) {
        digit = c - '0';
      } else if (digitFromName.has_value()) {
        digit = digitFromName.value();
      } else {
        continue;
      }

      if (!firstDigit.has_value()) {
        firstDigit = digit;
      }

      lastDigit = digit;
    }

    sum += firstDigit.value() * 10 + lastDigit;
  }

  assert(sum == 56017);

  return std::to_string(sum);
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
    default:
      std::cerr << "Not a valid advent day" << std::endl;
      return 1;
  }

  std::cout << "Result: " << output << std::endl;

  return 0;
}
