#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <ctype.h>
#include <algorithm>
#include <optional>
#include <cassert>
#include <regex>

typedef std::string (*AdventFunction)(std::vector<std::string>);

template <typename T> std::map<T, int> indexify(const std::vector<T>& vec) {
  std::map<T, int> indexes;
  for (size_t i = 0; i < vec.size(); i++) {
    indexes[vec[i]] = i;
  }

  return indexes;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename K, typename V>
const bool has(const  std::map <K,V> & m, const K & key) {
   return m.find(key) != m.end();
}

template <typename K, typename V>
const bool has(const  std::unordered_set <K,V> & m, const K & key) {
   return m.find(key) != m.end();
}

bool contains(const std::string& str, const std::string& substr) {
  return str.find(substr) != std::string::npos;
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

bool starts_with(const std::string& str, const std::string& prefix) {
  return str.rfind(prefix, 0) == 0;
}

template <typename V>
const V & pop_back(std::vector<V> & vec) {
  assert(!vec.empty());
  V& val = vec.back();
  vec.pop_back();
  return val;
}

std::vector<std::string> regex_search_all(const std::regex& rgx, const std::string& str) {
    std::smatch match;
    std::string::const_iterator start = str.begin();
    std::vector<std::string> output;
    while (std::regex_search(start, str.end(), match, rgx)) {
        for (auto sub_match : match) {
          std::string str = sub_match.str();
          if (!starts_with(str, "mul")) {
            output.push_back(str);
          }
        }

        start = match.suffix().first;
    }

    return output;
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
    for (const std::string& token : split(line, " ")) {
      report.push_back(std::stoi(token));
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

  assert(count == 626);

  return std::to_string(count);
}

int advent3_get_sum_of_mul(std::vector<int> nums) {
  assert(nums.size() % 2 == 0);

  int total = 0;
  while (!nums.empty()) {
    total += pop_back(nums) * pop_back(nums);
  }

  return total;
}


std::string advent3(std::vector<std::string> lines) {
  // P1
  std::vector<int> nums;

  for (const std::string& line : lines) {
    std::regex rgx("mul\\((\\d+),(\\d+)\\)");
    for (const std::string& str : regex_search_all(rgx, line)) {
      if (!starts_with(str, "mul")) {
        nums.push_back(std::stoi(str));
      }
    }
  }

  int total = advent3_get_sum_of_mul(nums);
  assert(total == 173529487);

  // P2
  nums.clear();

  bool enabled = true;
  for (const std::string& line : lines) {
    std::regex rgx("(mul\\((\\d+),(\\d+)\\))|(do\\(\\))|(don't\\(\\))");
    for (const std::string& str : regex_search_all(rgx, line)) {
      if (str.empty()) {
        continue;
      } else if (str == "do()") {
        enabled = true;
      } else if (str == "don't()") {
        enabled = false;
      } else if (!starts_with(str, "mul") && enabled) {
        nums.push_back(std::stoi(str));
      }
    }
  }

  total = advent3_get_sum_of_mul(nums);
  assert(total == 99532691);

  return std::to_string(total);
}


std::optional<char> advent4_get(std::vector<std::string> lines, size_t r, size_t c) {
  if (r < 0 || r >= lines.size() || c < 0 || c >= lines[0].size()) {
    return std::nullopt;
  }

  return std::make_optional(lines[r][c]);
}

std::string advent4_get_word(std::vector<std::string> lines, size_t r, size_t c, int direction, size_t len) {
  int dr, dc;
  switch (direction) {
    case 0:
      dr = 1;
      dc = 0;
      break;
    case 1:
      dr = 1;
      dc = 1;
      break;
    case 2:
      dr = 0;
      dc = 1;
      break;
    case 3:
      dr = -1;
      dc = 1;
      break;
    case 4:
      dr = -1;
      dc = 0;
      break;
    case 5:
      dr = -1;
      dc = -1;
      break;
    case 6:
      dr = 0;
      dc = -1;
      break;
    case 7:
      dr = 1;
      dc = -1;
      break;
    default:
      assert(false);
  }

  std::string word = "";
  for (size_t i = 0; i < len; i++) {
    std::optional<char> ch = advent4_get(lines, r + dr * i, c + dc * i);
    if (ch.has_value()) {
      word += ch.value();
    }
  }

  return word;
}

std::string advent4(std::vector<std::string> lines) {
  // P1
  int total = 0;
  for (size_t r = 0; r < lines.size(); r++) {
    for (size_t c = 0; c < lines[0].size(); c++) {
      for (int direction = 0; direction < 8; direction++) {
        std::string word = advent4_get_word(lines, r, c, direction, 4);
        if (word == "XMAS") {
          total++;
        }
      }
    }
  }

  assert(total == 2514);

  // P2
  total = 0;
  for (size_t r = 0; r < lines.size(); r++) {
    for (size_t c = 0; c < lines[0].size(); c++) {
      std::string pos_diag = advent4_get_word(lines, r, c, 1, 2) + advent4_get_word(lines, r, c, 5, 2);
      if (pos_diag != "AMAS" && pos_diag != "ASAM") {
        continue;
      }

      std::string neg_diag = advent4_get_word(lines, r, c, 3, 2) + advent4_get_word(lines, r, c, 7, 2);
      if (neg_diag != "AMAS" && neg_diag != "ASAM") {
        continue;
      }

      total++;
    }
  }

  assert(total == 1888);

  return std::to_string(total);
}

std::string advent5(std::vector<std::string> lines) {
  struct TIntPair {
    int first, second;
  };

  std::vector<TIntPair> rules;
  std::vector<std::vector<int>> orders;
  for (const std::string& line : lines) {
    if (contains(line, "|")) {
      std::vector<std::string> parts = split(line, "|");
      assert(parts.size() == 2);

      int before = std::stoi(parts[0]);
      int after = std::stoi(parts[1]);
      rules.push_back({before, after});
    } else if (contains(line, ",")) {
      std::vector<int> order;
      for (const std::string& token : split(line, ",")) {
        order.push_back(std::stoi(token));
      }

      orders.push_back(order);
    }
  }

  // P1
  int sum = 0;
  std::vector<std::vector<int>> wrong_orders;
  for (const std::vector<int>& order : orders) {
    std::map<int, int> indexes = indexify(order);
    bool valid = true;
    for (const TIntPair& pair : rules) {
      int before = pair.first;
      int after = pair.second;

      if (!has(indexes, before) || !has(indexes, after)) {
        continue;
      }
      
      valid = valid && indexes[before] < indexes[after];

      if (!(indexes[before] < indexes[after]) && false) {
        std::cout << "Invalid: " << before << " " << after << " " << valid << std::endl;

        // print order
        for (int i : order) {
          std::cout << i << " ";
        }
        std::cout << std::endl;
      }
    }

    if (valid) {
      sum += order[order.size() / 2];
    } else {
      wrong_orders.push_back(order);
    }
  }

  assert(sum == 4689);

  // P2
  sum = 0;
  for (const std::vector<int>& order : wrong_orders) {
    std::unordered_set<int> vals;
    for (int i : order) {
      vals.insert(i);
    }

    std::map<int, int> out_degree;
    std::vector<TIntPair> local_rules;
    for (const TIntPair& pair : rules) {
      if (has(vals, pair.first) && has(vals, pair.second)) {
        local_rules.push_back(pair);
        out_degree[pair.first] = idx(out_degree, pair.first, 0) + 1;
      }
    }

    std::vector<int> sorted;
    while (vals.size() > 0) {
      std::optional<int> endOpt = std::nullopt;
      for (int i : vals) {
        if (idx(out_degree, i, 0) == 0) {
          endOpt = i;
        }
      }

      assert(endOpt.has_value());
      int end = endOpt.value();

      sorted.push_back(end);
      vals.erase(end);

      for (const TIntPair& pair : local_rules) {
        if (pair.second == end) {
          out_degree[pair.first] = idx(out_degree, pair.first, 0) - 1;
        }
      }
    }

    sum += sorted[sorted.size() / 2];
  }

  assert(sum == 6336);

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

  std::map<int, AdventFunction> functions = {
    {1, &advent1},
    {2, &advent2},
    {3, &advent3},
    {4, &advent4},
    {5, &advent5},
  };

  int day = std::stoi(dayStr);
  if (!has(functions, day)) {
    std::cerr << "Not a valid advent day" << std::endl;
    return 1;
  }

  std::string output = functions[day](lines);
  std::cout << "Result: " << output << std::endl;

  return 0;
}
