//
//  main.cpp
//  CF.STL_Algo_Transform
//
//  Created by Alan Sampson on 10/22/22.
//

#define __STDC_WANT_LIB_EXT1__ 1
// #define XPLUS_

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

#include <cctype>
#include <cstdlib>

#include "identify.h"
#include "version_info.h"

using namespace std::literals::string_literals;

//  MARK: - Reference
//  @see: https://en.cppreference.com/w/cpp/algorithm/transform
//  @see: https://gist.githubusercontent.com/creatorlxd/4e1eea99b0d47a192530585c083d6937/raw/0a8a71a2cd534ee9a5ed621f05214e397c6c2110/string_converter.cpp
//  @see: https://stackoverflow.com/questions/12015571/how-to-print-unicode-character-in-c
//  @see: https://stackoverflow.com/questions/16944750/c-unicode-characters-printing

//  MARK: - Definitions
enum OrdinalFmt {
  DEC, HEX, OCT,
};

static
void transformer(void);
[[deprecated]]
static
void conversions(void);
static
void show0444(void);
[[deprecated]]
static
void show2780(void);
static
void print_ordinals(std::vector<std::size_t> const & ordinals, OrdinalFmt const dxo = DEC);

// --------------------------------------------------------------
//  print a UTF-16 codepoint
static
auto constexpr pc16 = [](char16_t const & wc) {
  std::printf("%04x ", wc);
};

//  print a UTF-32 codepoint
static
auto constexpr pc32 = [](char32_t const & wc) {
  std::printf("%08x ", wc);
};

// typedef unsigned char char8_t;
//  print a UTF-8 codepoint
static
auto constexpr pc08 = [](char8_t const & wc) {
  std::printf("%02x ", wc);
};
// --------------------------------------------------------------

//  MARK: - Implementation
/*
 *  MARK: main()
 */

int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "CF.STL_Algo_Transform\n"s;

  std::cout << std::string(60, '-') << '\n';
  avi::version_details();
  avi::identify();

  // --------------------------------------------------------------
  transformer();
  conversions();
  show0444();
  show2780();

  return 0;
}

/*
 *  MARK: transformer()
 */
void transformer(void) {
  std::cout << std::string(60, '-') << '\n';
  std::cout << "In function "s << __func__ << "()"s << '\n';
  
  std::cout << "Using std::transform\n"s;
  std::string s { "hello"s };
  std::cout << "s = "s << quoted(s) << '\n';

  std::transform(s.cbegin(), s.cend(),
                 s.begin(), /* write to the same location */
                 [](unsigned char c) { return std::toupper(c); });

  std::cout << "s = "s << quoted(s) << '\n';

  std::cout << std::string(60, '-') << '\n';
  // achieving the same with std::for_each (see Notes above)
  std::cout << "Using std::for_each\n"s;
  std::string g { "hello"s };
  std::cout << "g = "s << quoted(g) << '\n';

  std::for_each(g.begin(), g.end(), [](char & c) {
    /* modify in-place */
    c = std::toupper(static_cast<unsigned char>(c));
  });

  std::cout << "g = "s << quoted(g) << '\n';

  std::cout << std::string(60, '-') << '\n';
#ifdef XPLUS_
  std::cout << "Transform ordinals with custom plus function\n"s;
#else
  std::cout << "Transform ordinals with std::plus\n"s;
#endif /* XPLUS_ */
  std::vector<std::size_t> ordinals;
  std::transform(s.cbegin(), s.cend(), std::back_inserter(ordinals),
                 [](unsigned char c) { return c; });

  print_ordinals(ordinals);
  print_ordinals(ordinals, OrdinalFmt::HEX);
  print_ordinals(ordinals, OrdinalFmt::OCT);

#ifdef XPLUS_
  [[maybe_unused]]
  auto xplus = [](auto const lhs, auto const rhs) {
    // std::cout << std::hex;
    auto sum = lhs + rhs;
    std::cout << std::setw(3) << lhs;
    std::cout << std::setw(3) << rhs;
    std::cout << std::setw(4) << sum;
    std::cout.put('\n');
    std::cout << std::dec;
    return sum;
  };
#endif /* XPLUS_ */

  std::transform(ordinals.cbegin(),
                 ordinals.cend(),
                 ordinals.cbegin(),
                 ordinals.begin(),
#ifdef XPLUS_
                 xplus
#else
                 std::plus<>{}
#endif /* XPLUS_ */
  );

  print_ordinals(ordinals);
  print_ordinals(ordinals, OrdinalFmt::HEX);
  print_ordinals(ordinals, OrdinalFmt::OCT);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::transform and std::plus\n"s;
  std::cout << "  'A' - 'a' ASCII offset trick\n"s;

  std::string h { "hello"s };
  auto o = std::string(h.length(), 'A' - 'a');
  std::cout << "h = "s << quoted(h) << '\n';

  std::transform(h.cbegin(), h.cend(), o.cbegin(), h.begin(), std::plus<>{});

  std::cout << "h = "s << quoted(h) << '\n';

  std::cout << std::endl;

  return;
}

/*
 *  MARK: conversions()
 *
 *  Uses deprecated STL functions to convert wiide strings to UTF-8.
 *  Att time of writing the Standards Committee have not approved an
 *  alternative solution.
 */
[[deprecated]]
void conversions(void) {
  std::cout << std::string(60, '-') << '\n';
  std::cout << "In function "s << __func__ << "()"s << '\n';

  auto tnr { 0u };
  std::cout << "Test "s << std::setw(2) << ++tnr << ".\n\n"s;

  //  Convert a UTF-16 string to a UTF-8 string
  // auto lw = L"Prinzregentenstraße";

  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt16_to_8;
  std::u16string lw(u"Prinzregentenstraße"s);
  std::string lw8 = cvt16_to_8.to_bytes(lw);
  std::cout << "Length: "s << std::setw(5) << lw8.size() << ' '
            << lw8 << '\n';
  std::for_each(lw.cbegin(),
                lw.cend(),
                pc16);
  std::cout.put('\n');
  std::for_each(lw8.cbegin(),
                lw8.cend(),
                pc08);
  std::cout.put('\n');
  std::cout.put('\n');

  std::cout << "Test "s << std::setw(2) << ++tnr << ".\n\n"s;

  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt32_to_8;
  std::u32string lww(U"Prinzregentenstraße"s);
  std::string lww8 = cvt32_to_8.to_bytes(lww);
  std::cout << "Length: "s << std::setw(5) << lww8.size() << ' '
            << lww8 << '\n';
  std::for_each(lww.cbegin(),
                lww.cend(),
                pc32);
  std::cout.put('\n');
  std::for_each(lww8.cbegin(),
                lww8.cend(),
                pc08);
  std::cout.put('\n');
  std::cout.put('\n');

  std::cout << "Test "s << std::setw(2) << ++tnr << ".\n\n"s;

  //  TODO: Remove! Wrong! Only takes the last byte of a UTF-16 char!
  std::string ln(lw.begin(), lw.end());
  std::cout << "Length: "s << std::setw(5) << ln.size() << ' '
            << ln << '\n';
  std::for_each(ln.cbegin(),
                ln.cend(),
                pc08);
  std::cout.put('\n');
  std::cout.put('\n');

  std::cout << "Test "s << std::setw(2) << ++tnr << ".\n\n"s;

  std::string mn { "Prinzregentenstraße"s };
  std::cout << "Length: "s << std::setw(5) << mn.size() << ' '
            << mn << '\n';
  std::for_each(mn.cbegin(),
                mn.cend(),
                pc08);
  std::cout.put('\n');
  std::cout.put('\n');

  return;
}

/*
 *  MARK: show0444()
 */
void show0444(void) {
  std::cout << std::string(60, '-') << '\n';
  std::cout << "In function "s << __func__ << "()"s << '\n';

  std::cout << "Hello World\nф\n\u0444\n\n"s;
  char16_t fe = u'\u0444';
  std::string sfe = "\u0444"s;
  std::cout << std::hex
            << std::setfill('0')
            << std::setw(4)
            << static_cast<uint16_t>(fe)
            << std::setfill(' ')
            << std::dec
            << '\n'
            << '\n';
  std::cout << sfe << '\n' << '\n';
  std::printf("%04x\n", (char16_t) fe);

  auto afe = sfe.c_str();
  auto fsz = sfe.size();
  for (size_t f_ { 0ul }; f_ < fsz; ++f_) {
    std::printf("%#02x ", (uint8_t) afe[f_]);
  }
  std::cout.put('\n');

  char xfe[] = "\u0444";
  [[maybe_unused]]
  auto xfesz = sizeof xfe / sizeof *xfe;
  std::cout << '\n' << xfe << '\n' << '\n';
  for (size_t f_ { 0ul }; f_ < xfesz && xfe[f_] != '\0'; ++f_) {
    printf("%#02x ", (char8_t) xfe[f_]);
  }
  std::cout.put('\n');
  std::cout.put('\n');

  return;
}

/*
 *  MARK: show2780()
 *
 *  Uses deprecated STL functions to convert wiide strings to UTF-8.
 *  Att time of writing the Standards Committee have not approved an
 *  alternative solution.
 */
void show2780(void) {
  std::cout << std::string(60, '-') << '\n';
  std::cout << "In function "s << __func__ << "()"s << '\n';

  char32_t base = U'\u2780';

  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
  std::string sbase = convert.to_bytes(base + 5);
  std::cout << std::hex
            << std::setfill('0')
            << std::setw(8)
            << static_cast<uint_fast32_t>(base)
            << std::setfill(' ')
            << std::dec
            << '\n';
  for (size_t f_ { 0ul }; sbase[f_] != '\0'; ++f_) {
    printf("%#02x ", (char8_t) sbase[f_]);
  }
  std::cout.put('\n');
  std::cout << sbase << '\n';

  return;
}

/*
 *  MARK: print_ordinals()
 */
void print_ordinals(std::vector<std::size_t> const & ordinals, OrdinalFmt const dxo) {
  switch (dxo) {
    case OrdinalFmt::HEX:
      std::cout << "ordinals [hex]: "s;
      break;
    case OrdinalFmt::OCT:
      std::cout << "ordinals [oct]: "s;
      break;
    case OrdinalFmt::DEC:
    default:
      std::cout << "ordinals [dec]: "s;
      break;
  }

  for (std::size_t ord : ordinals) {
    switch (dxo) {
      case OrdinalFmt::HEX:
        std::cout << std::hex;
        break;
      case OrdinalFmt::OCT:
        std::cout << std::oct;
        break;
      case OrdinalFmt::DEC:
      default:
        std::cout << std::dec;
        break;
    }
    std::cout << std::setw(3) << ord << ' ';
    std::cout << std::dec;
  }
  std::cout << '\n';

  return;
}
