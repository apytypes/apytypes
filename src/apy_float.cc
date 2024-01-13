#include <iostream>
#include <limits>
#include <math.h>

#include "apy_float.h"
#include "apy_util.h"

#include "ieee754.h"

constexpr bool PRINT_WARNINGS = false;

void print_warning(const std::string msg) {
  if constexpr (PRINT_WARNINGS) {
    std::cerr << "Warning: " << msg;
  }
}

/* **********************************************************************************
 * * Constructors                                          *
 * **********************************************************************************
 */

APyFloat::APyFloat(bool sign, exp_t exp, man_t man, std::uint8_t exp_bits,
                   std::uint8_t man_bits, std::optional<exp_t> bias)
    : exp_bits(exp_bits), man_bits(man_bits), bias(bias.value_or(ieee_bias())),
      sign(sign), exp(exp), man(man) {

  if (bias.has_value() && bias.value() != ieee_bias()) {
    print_warning("non 'ieee-like' biases is not sure to work yet.\n");
  }
}

APyFloat::APyFloat(std::uint8_t exp_bits, std::uint8_t man_bits,
                   std::optional<exp_t> bias)
    : exp_bits(exp_bits), man_bits(man_bits), bias(bias.value_or(ieee_bias())),
      sign(false), exp(0), man(0) {}

/* **********************************************************************************
 * * Methods for conversions                               *
 * **********************************************************************************
 */

APyFloat APyFloat::from_double(double value, std::uint8_t exp_bits,
                               std::uint8_t man_bits, std::optional<exp_t> bias,
                               RoundingMode rounding_mode) {
  APyFloat f(exp_bits, man_bits);
  return f.update_from_double(value, rounding_mode);
}

APyFloat &APyFloat::update_from_double(double value,
                                       RoundingMode rounding_mode) {

  // Initialize an APyFloat from the double
  APyFloat apy_double(sign_of_double(value), exp_of_double(value),
                      man_of_double(value), 11, 52);

  // Cast it to the correct format
  *this = apy_double.cast_to(exp_bits, man_bits, bias, rounding_mode);

  return *this;
}

APyFloat APyFloat::cast_to(std::uint8_t new_exp_bits, std::uint8_t new_man_bits,
                           std::optional<exp_t> new_bias,
                           RoundingMode rounding_mode) const {
  APyFloat res(new_exp_bits, new_man_bits, new_bias);
  res.sign = sign;

  // Handle special values first
  if (is_nan()) {
    return res.construct_nan(sign);
  } else if (is_inf()) {
    return res.construct_inf(sign);
  } else if (is_zero()) {
    return res.construct_zero(sign);
  }

  // TODO: Handle cast *from* subnormal

  // Initial value for exponent
  std::int64_t new_exp = (std::int64_t)exp - (std::int64_t)bias + 1 -
                         is_normal() + (std::int64_t)res.bias;

  if (new_exp <=
      -static_cast<std::int64_t>(res.man_bits)) { // Exponent too small
    return res.construct_zero();
  }

  const auto man_bits_delta = res.man_bits - man_bits;

  // Initial value for mantissa
  man_t new_man =
      (man_bits_delta > 0) ? (man << man_bits_delta) : (man >> -man_bits_delta);

  if (new_exp <=
      0) { // The number will be converted to a subnormal in the new format
    new_man |= res.leading_one(); // Add leading one
    new_man <<= (res.man_bits + new_exp -
                 1);              // Shift the difference between E_min and exp
    new_man /= 1 << res.man_bits; // Divide by the minimum subnorm (i.e. E_min)
    new_man &= res.man_mask();    // Mask away the leading ones
    new_exp = 0;
  } else if (man_bits_delta < 0) { // Normal case, exponent is positive
    // Calculate rounding bit
    man_t G, // Guard
        T,   // Sticky bits after guard
        B;   // Rounding bit to add to LSB

    G = (man >> (std::abs(man_bits_delta) - 1)) & 1;
    T = (man & ((1ULL << (std::abs(man_bits_delta - 1))) - 1)) != 0;

    switch (rounding_mode) {
    case RoundingMode::TO_POSITIVE:
      B = sign ? 0 : G;
      break;
    case RoundingMode::TO_NEGATIVE:
      B = sign ? G : 0;
      break;
    case RoundingMode::TO_ZERO:
      B = 0;
      break;
    case RoundingMode::TIES_TO_EVEN:
      B = G & (new_man | T);
      break;
    case RoundingMode::TIES_TO_AWAY:
      throw NotImplementedException(
          "APyFloat: rounding to away has not been implemented yet.");
    case RoundingMode::JAMMING:
      throw NotImplementedException(
          "APyFloat: rounding mode jamming has not been implemented.");
    default:
      throw NotImplementedException("APyFloat: Unknown rounding mode.");
    }

    new_man += B;
    if (static_cast<std::uint64_t>(new_man) > res.man_mask()) {
      ++new_exp;
      new_man = 0;
    }
  }

  if (new_exp >= res.max_exponent()) {
    return res.construct_inf();
  }

  res.man = new_man;
  res.exp = new_exp;
  return res;
}

double APyFloat::to_double() const {
  const auto apy_d = cast_to(11, 52);
  double d{};
  set_sign_of_double(d, apy_d.sign);
  set_exp_of_double(d, apy_d.exp);
  set_man_of_double(d, apy_d.man);
  return d;
}

APyFloat::operator double() const { return to_double(); }

APyFloat APyFloat::from_bits(unsigned long long bits, std::uint8_t exp_bits,
                             std::uint8_t man_bits) {
  APyFloat f(exp_bits, man_bits);
  return f.update_from_bits(bits);
}

APyFloat &APyFloat::update_from_bits(unsigned long long bits) {
  man = bits & man_mask();
  bits >>= man_bits;

  exp = bits & exp_mask();
  bits >>= exp_bits;

  sign = bits != 0;

  return *this;
}

unsigned long long APyFloat::to_bits() const {
  return (sign << (exp_bits + man_bits)) | (exp << man_bits) | man;
}

std::string APyFloat::str() const { return std::to_string(to_double()); }

std::string APyFloat::repr() const {
  std::string str =
      "APyFloat(sign=" + std::to_string(static_cast<int>(sign)) +
      ", exp=" + std::to_string(exp) + ", man=" + std::to_string(man) +
      ", exp_bits=" + std::to_string(exp_bits) +
      ", man_bits=" + std::to_string(man_bits) +
      (bias == ieee_bias() ? std::string() : ", bias=" + std::to_string(bias)) +
      ")";
  return str;
}

std::string APyFloat::pretty_string() const {
  std::string str = "fp<" + std::to_string(exp_bits) + "," +
                    std::to_string(man_bits) + (sign ? ">(-" : ">(");

  if (is_nan()) {
    return str + "NaN)";
  } else if (is_inf()) {
    return str + "inf)";
  }

  str += "2**" +
         std::to_string(static_cast<std::int64_t>(exp) - bias - man_bits + 1 -
                        is_normal()) +
         "*" + std::to_string((is_normal() << man_bits) | man) + ")";

  return str;
}

/* ******************************************************************************
 * * Arithmetic operators                                   *
 * ******************************************************************************
 */

APyFloat APyFloat::operator+(APyFloat y) const {
  if (get_rounding_mode() != RoundingMode::TIES_TO_EVEN) {
    throw NotImplementedException(
        "APyFloat: Only round-ties-to-even is supported currently.");
  }

  APyFloat x = *this;

  if (!(x.is_finite() && y.is_finite())) {
    throw NotImplementedException();
  }

  APyFloat res(std::max(x.exp_bits, y.exp_bits),
               std::max(x.man_bits, y.man_bits));

  if ((res.man_bits + 5) >
      (sizeof(man_t) *
       CHAR_BIT)) { // +1 (leading one) +3 (guard bits) +1 (addition)
    throw std::domain_error("The intermediate mantissa can potentially exceed "
                            "its underlaying data type.");
  }

  // Compute sign and swap operands if need to make sure |x| >= |y|
  const APyFloat xabs = APyFloat::abs(x);
  const APyFloat yabs = APyFloat::abs(y);

  if (xabs > yabs) {
    res.sign = x.sign;
  } else if (xabs < yabs) {
    res.sign = y.sign;
    std::swap(x, y);
  } else {
    if (x.sign != y.sign) {
      return res.construct_zero(true);
    }
    res.sign = x.sign | y.sign;
  }

  std::int64_t new_exp = x.exp - x.bias + res.bias;

  // Conditionally add leading one's
  const man_t nx = x.is_normal();
  const man_t ny = y.is_normal();
  man_t mx = (nx << x.man_bits) | x.man;
  man_t my = (ny << y.man_bits) | y.man;

  // Add room for guard bits
  mx <<= 3;
  my <<= 3;

  // Align mantissas based on mixed formats
  const auto man_bits_delta = x.man_bits - y.man_bits;

  if (man_bits_delta < 0) {
    mx <<= -man_bits_delta;
  } else {
    my <<= man_bits_delta;
  }

  // Align mantissas based on exponent difference
  const std::int64_t delta = (x.exp - x.bias - nx) - (y.exp - y.bias - ny);

  const std::int64_t max_shift =
      res.man_bits + 4UL; // +4 to account for leading one and 3 guard bits
  man_t highY = my >> std::min(max_shift, delta);

  man_t lowY; // Used to update the sticky bit position T
  if (delta <= 3) {
    lowY = 0;
  } else if (delta >= max_shift) {
    lowY = 1;
  } else {
    lowY = (my & ((1 << (max_shift - delta)) - 1)) != 0;
  }

  // Perform addition/subtraction
  man_t highR;
  if (x.sign != y.sign) {
    highR = mx - (highY | lowY);
  } else {
    highR = mx + (highY | lowY);
  }

  // Perform rounding

  if (highR & (1ULL << (res.man_bits + 4))) { // Carry
    ++new_exp;
    highR &= (1ULL << (res.man_bits + 4)) - 1;
    return APyFloat(res.sign, new_exp, highR, res.exp_bits, (res.man_bits + 4),
                    res.bias)
        .cast_to(res.exp_bits, res.man_bits, res.bias, get_rounding_mode());

  } else if (highR & (1ULL << (res.man_bits + 3))) { // No carry
    highR &= (1ULL << (res.man_bits + 3)) - 1;
    return APyFloat(res.sign, new_exp, highR, res.exp_bits, (res.man_bits + 3),
                    res.bias)
        .cast_to(res.exp_bits, res.man_bits, res.bias, get_rounding_mode());
  }

  // Cancellation occured
  while (!(highR & (1 << (res.man_bits + 3)))) {
    highR <<= 1;
    --new_exp;
    if (new_exp == 0) {
      break;
    }
  }

  res.man = (highR >> 3) & res.man_mask();
  res.exp = static_cast<exp_t>(new_exp);

  return res;
}

APyFloat APyFloat::operator-(const APyFloat &y) const { return *this + (-y); }

APyFloat APyFloat::operator-() const {
  auto res = *this;
  res.sign = !sign;
  return res;
}

APyFloat APyFloat::operator*(const APyFloat &y) const {
  APyFloat res(std::max(exp_bits, y.exp_bits), std::max(man_bits, y.man_bits));

  if ((2 * res.man_bits + 1) > (sizeof(man_t) * CHAR_BIT)) {
    throw std::domain_error("The intermediate mantissa can potentially exceed "
                            "its underlaying data type.");
  }

  // Calculate sign
  res.sign = sign ^ y.sign;

  // Handle special operands
  if ((is_nan() || y.is_nan()) || (is_inf() && y.is_zero()) ||
      (is_zero() && y.is_inf())) {
    return res.construct_nan();
  }

  if ((is_inf() || y.is_inf())) {
    return res.construct_inf();
  }

  if ((is_subnormal() && !is_zero()) || (y.is_subnormal() && !y.is_zero())) {
    print_warning("multiplication with subnormals is not sure to work yet.");
  }

  if (is_zero() || y.is_zero()) {
    return res.construct_zero();
  }

  // Conditionally add leading one's
  man_t mx = (is_normal() << man_bits) | man;
  man_t my = (y.is_normal() << y.man_bits) | y.man;

  // One of the operands should be scaled but since (a*scale)*b == (a*b)*scale
  // we can just scale the result.
  const exp_t man_bits_delta = std::abs(man_bits - y.man_bits);
  man_t highR = mx * my;
  highR <<= man_bits_delta;

  std::int64_t new_exp = (exp - bias) + (y.exp - y.bias) + res.bias;

  // Perform rounding

  if (highR & static_cast<man_t>((1ULL << (2 * res.man_bits + 1)))) { // Carry
    ++new_exp;
    highR &= (1ULL << (2 * res.man_bits + 1)) - 1;
    return APyFloat(res.sign, new_exp, highR, res.exp_bits,
                    (2 * res.man_bits + 1), res.bias)
        .cast_to(res.exp_bits, res.man_bits, res.bias, get_rounding_mode());
  } else {
    highR &= (1ULL << (2 * res.man_bits)) - 1;
    return APyFloat(res.sign, new_exp, highR, res.exp_bits, (2 * res.man_bits),
                    res.bias)
        .cast_to(res.exp_bits, res.man_bits, res.bias, get_rounding_mode());
  }
}

APyFloat APyFloat::operator/(const APyFloat &y) const {
  throw NotImplementedException(
      "APyFloat: Division has not yet been implemented.");
}

/* ******************************************************************************
 * * Mathematical functions                                 *
 * ******************************************************************************
 */

APyFloat APyFloat::abs(const APyFloat &x) { return x.is_sign_neg() ? -x : x; }

APyFloat APyFloat::pow(const APyFloat &x, const APyFloat &y) {
  throw NotImplementedException(
      "APyFloat: Power to another APyFloat has not yet been implemented.");
}

APyFloat APyFloat::pown(const APyFloat &x, int n) {
  throw NotImplementedException(
      "APyFloat: Power to an integer has not yet been implemented.");
}

/* ******************************************************************************
 * * Binary comparison operators                           *
 * ******************************************************************************
 */

bool APyFloat::operator==(const APyFloat &rhs) const {
  if (sign != rhs.sign) {
    return false;
  }

  if (is_nan() || rhs.is_nan()) {
    return false;
  }

  if (is_inf() && rhs.is_inf()) {
    return true;
  }

  if (is_zero() && rhs.is_zero()) {
    return true;
  }

  // Operands are (sub)normals
  std::int64_t ex = exp - bias - man_bits + 1 - is_normal();
  std::int64_t ey = rhs.exp - rhs.bias - rhs.man_bits + 1 - rhs.is_normal();

  man_t mx = (is_normal() << man_bits) | man;
  man_t my = (rhs.is_normal() << rhs.man_bits) | rhs.man;

  // Align mantissas
  const auto man_bits_delta = man_bits - rhs.man_bits;

  if (man_bits_delta < 0) {
    mx <<= -man_bits_delta;
    ex += man_bits_delta;
  } else {
    my <<= man_bits_delta;
    ey -= man_bits_delta;
  }

  return (ex == ey) && (mx == my);
}

bool APyFloat::operator!=(const APyFloat &rhs) const {
  if (is_nan() || rhs.is_nan()) {
    return false;
  } else {
    return !(*this == rhs);
  }
}

bool APyFloat::operator<=(const APyFloat &rhs) const {
  return (*this < rhs) || (*this == rhs);
}

bool APyFloat::operator<(const APyFloat &rhs) const {
  if (is_nan() || rhs.is_nan() || (*this == rhs)) {
    return false;
  }

  if (sign != rhs.sign) {
    return sign;
  }

  bool ret;
  if ((static_cast<std::int64_t>(exp) - bias) <
      (static_cast<std::int64_t>(rhs.exp) - rhs.bias)) {
    ret = true;
  } else if ((exp - bias) > (rhs.exp - rhs.bias)) {
    ret = false;
  } else {
    const auto man_bits_diff = man_bits - rhs.man_bits;
    if (man_bits_diff < 0) {
      ret = (man << -man_bits_diff) < rhs.man;
    } else {
      ret = man < (rhs.man << man_bits_diff);
    }
  }
  return ret ^ sign;
}

bool APyFloat::operator>=(const APyFloat &rhs) const {
  return (*this > rhs) || (*this == rhs);
}

bool APyFloat::operator>(const APyFloat &rhs) const {
  if (is_nan() || rhs.is_nan() || (*this == rhs)) {
    return false;
  } else {
    return !(*this < rhs);
  }
}

/* ******************************************************************************
 * * Non-computational functions                           *
 * ******************************************************************************
 */

// True if and only if x is normal (not zero, subnormal, infinite, or NaN).
bool APyFloat::is_normal() const { return is_finite() && !is_subnormal(); }

// True if and only if x is zero, subnormal, or normal.
bool APyFloat::is_finite() const {
  return is_subnormal() || exp != max_exponent();
}

// True if and only if x is subnormal. Zero is also considered a subnormal
// number.
bool APyFloat::is_subnormal() const { return exp == 0; }

// True if and only if x is zero.
bool APyFloat::is_zero() const { return exp == 0 && man == 0; }

// True if and only if x is NaN.
bool APyFloat::is_nan() const { return exp == max_exponent() && man != 0; }

// True if and only if x is infinite.
bool APyFloat::is_inf() const { return exp == max_exponent() && man == 0; }

// True if and only if x has a negative sign. Applies to zeros and NaNs as well.
bool APyFloat::is_sign_neg() const { return sign; }

/* ******************************************************************************
 * * Helper functions                                         *
 * ******************************************************************************
 */

APyFloat APyFloat::construct_zero(std::optional<bool> new_sign) const {
  return APyFloat(new_sign.value_or(sign), 0, 0, exp_bits, man_bits);
}

APyFloat APyFloat::construct_inf(std::optional<bool> new_sign) const {
  return construct_nan(new_sign, 0);
}

APyFloat APyFloat::construct_nan(std::optional<bool> new_sign,
                                 man_t payload /*= 1*/) const {
  return APyFloat(new_sign.value_or(sign), max_exponent(), payload, exp_bits,
                  man_bits);
}
