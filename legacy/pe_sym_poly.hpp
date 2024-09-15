#ifndef PE_SYM_POLY_
#define PE_SYM_POLY_

#include "pe_base"

namespace pe {
// [+- ]*
SL std::vector<int> ParseSgnList(const std::string& s, int& i) {
  const int size = static_cast<int>(s.size());
  std::vector<int> sgns;
  for (;;) {
    while (i < size && std::isspace(s[i])) ++i;
    if (i < size && (s[i] == '+' || s[i] == '-')) {
      sgns.push_back(s[i] == '+' ? 1 : -1);
      ++i;
    } else {
      return sgns;
    }
  }
}

SL std::string ParseDigList(const std::string& s, int& i) {
  const int size = static_cast<int>(s.size());
  while (i < size && std::isspace(s[i])) ++i;
  if (i < size && std::isdigit(s[i])) {
    int start = i;
    while (i < size && std::isdigit(s[i])) {
      ++i;
    }
    return s.substr(start, i - start);
  }
  return "";
}

SL std::string ParseIdentifier(const std::string& s, int& i) {
  const int size = static_cast<int>(s.size());
  while (i < size && std::isspace(s[i])) ++i;
  if (i < size && (s[i] == '_' || std::isalpha(s[i]))) {
    int start = i;
    while (i < size &&
           (s[i] == '_' || std::isdigit(s[i]) || std::isalpha(s[i]))) {
      ++i;
    }
    return s.substr(start, i - start);
  }
  return "";
}

SL std::vector<std::pair<std::string, std::string>> ParseSingleTermList(
    const std::string& s, int& i) {
  std::vector<std::pair<std::string, std::string>> result;
  const int size = static_cast<int>(s.size());
  for (;;) {
    while (i < size && std::isspace(s[i])) ++i;
    if (s[i] == '*') {
      ++i;
      while (i < size && std::isspace(s[i])) ++i;
    }
    std::string variable = ParseIdentifier(s, i);
    if (variable.empty()) variable = ParseDigList(s, i);
    if (variable.empty()) return result;
    while (i < size && std::isspace(s[i])) ++i;
    if (i < size && (s[i] == '^' && !std::isdigit(variable[0]))) {
      ++i;
      std::string e = ParseDigList(s, i);
      result.emplace_back(variable, e);
    } else {
      result.emplace_back(variable, "");
    }
  }
}

template <typename CT>
SL CT EvaluateDigs(const std::vector<int>& sgns, const std::string& digs,
                   CT defaultVal = 0) {
  int s = 1;
  for (const auto& iter : sgns) {
    if (iter == -1) s = -s;
  }
  CT v = 0;
  for (const auto& iter : digs) v = v * 10 + iter - '0';
  if (digs.empty()) v = defaultVal;
  return s == 1 ? v : -v;
}

template <typename CT>
SL std::pair<TermKey, CT> EvaluateSingleTermList(
    const std::vector<std::pair<std::string, std::string>>& singleTerms) {
  std::map<std::string, int> t;
  CT c = 1;
  for (const auto& iter : singleTerms) {
    if (std::isdigit(iter.first[0])) {
      c *= EvaluateDigs<int>({}, iter.first, 1);
    } else {
      t[iter.first] += EvaluateDigs<int>({}, iter.second, 1);
    }
  }
  TermKey key;
  for (const auto& iter : t) key.emplace_back(iter.first, iter.second);
  return {key, c};
}

template <typename CT>
SL std::pair<TermKey, CT> ParseTerm(const std::string& s, int& i) {
  auto sgns = ParseSgnList(s, i);
  auto singleTerms = ParseSingleTermList(s, i);
  auto t = EvaluateSingleTermList<CT>(singleTerms);
  auto sgn = EvaluateDigs<CT>(sgns, "", 1);
  return {t.first,
          singleTerms.empty() ? CT(0) : (sgn == 1 ? t.second : -t.second)};
}

template <typename CT>
SL std::pair<TermKey, CT> ParseTerm(const std::string& s) {
  int i = 0;
  return ParseTerm<CT>(s, i);
}

template <typename CT>
SL std::map<TermKey, CT> ParseSymPolyTerms(const std::string& s) {
  const int size = sz(s);
  int i = 0;
  std::map<TermKey, CT> terms;
  for (;;) {
    while (i < size && std::isspace(s[i])) ++i;
    int j = i;
    auto t = ParseTerm<CT>(s, i);
    if (j == i) {
      if (i < size) {
        std::cerr << "Unknown: " << s.substr(i) << std::endl;
      }
      break;
    }
    terms[t.first] += t.second;
  }
  return terms;
}

template <typename CT>
SL SymPoly<CT> ParseSymPoly(const std::string& s) {
  return SymPoly<CT>(ParseSymPolyTerms<CT>(s));
}

SL TermKey ToTermKey(const std::string& s) {
  int i = 0;
  return ParseTerm<int64>(s, i).first;
}
}