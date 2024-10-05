/*
 * Forward declarations of the important APyTypes types, and their relationship
 */

// Scalar types
class APyFixed;
class APyFloat;

// Array types
class APyFixedArray;
class APyFloatArray;

/*
 * APyTypes relationships
 */
template <typename T> struct scalar_variant;

template <typename T> struct array_variant;

template <> struct scalar_variant<APyFixedArray> {
    using type = APyFixed;
};
template <> struct scalar_variant<APyFloatArray> {
    using type = APyFloat;
};
template <> struct array_variant<APyFixed> {
    using type = APyFixedArray;
};
template <> struct array_variant<APyFloat> {
    using type = APyFloatArray;
};

template <typename T> using scalar_variant_t = typename scalar_variant<T>::type;

template <typename T> using array_variant_t = typename array_variant<T>::type;
