//! @file
//! comparison.h
//!
//! @brief
//! Contains functions to compare verctor registers.


#pragma once

#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/definitions.h"

#include <concepts>

// === DECLARATION ====================================================================================================

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Return `true` only if all elements of two registers are equal.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are greater than the ones of `rhs`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_greater(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are greater or equal than the ones of `rhs`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_greater_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are less than the corresponding ones in `rhs`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_less(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are less equal than the corresponding ones in `rhs`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_less_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if the element-wise comparisons of `lhs` and `rhs` yields `true` for all elements.
//!
//! @tparam T_RegisterType:
//! The register type
//! @tparam T_CompFunc:
//! The Type of a callable object that performs the register element comparison.
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//! @param[in] comp_func:
//! A comparison function or functor that provides the comparison of the register elements. Functors should be preferred
//! to assure inlining. See  https://stackoverflow.com/a/12718449/6700329
//!
//! @return
//! `true` or `false`
template <FloatVectorRegister T_RegisterType, std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto compare_all_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept
        -> bool;


//! @brief
//! Return `true` only if all elements of both registers are equal inside of a specified sequence of indices.
//!
//! @tparam t_idx_first:
//! The index of the first register element that is part of the sequence
//! @tparam t_length:
//! The number of elements in the sequence
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are greater than in `rhs` inside of a specified sequence.
//!
//! @tparam t_idx_first:
//! The index of the first register element that is part of the sequence
//! @tparam t_length:
//! The number of elements in the sequence
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_greater(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are greater or equal than in `rhs` inside of a specified sequence.
//!
//! @tparam t_idx_first:
//! The index of the first register element that is part of the sequence
//! @tparam t_length:
//! The number of elements in the sequence
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_greater_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are less than in `rhs` inside of a specified sequence of indices.
//!
//! @tparam t_idx_first:
//! The index of the first register element that is part of the sequence
//! @tparam t_length:
//! The number of elements in the sequence
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_less(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all elements of `lhs` are less equal than in `rhs` inside of a specified sequence of indices.
//!
//! @tparam t_idx_first:
//! The index of the first register element that is part of the sequence
//! @tparam t_length:
//! The number of elements in the sequence
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_less_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if the comparisons of all register elements are true inside of a specified sequence.
//!
//! @tparam t_idx_first:
//! The index of the first register element that is part of the sequence
//! @tparam t_length:
//! The number of elements in the sequence
//! @tparam T_RegisterType:
//! The register type
//! @tparam T_CompFunc:
//! The Type of a callable object that performs the register element comparison.
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//! @param[in] comp_func:
//! A comparison function or functor that provides the comparison of the register elements. Functors should be preferred
//! to assure inlining. See  https://stackoverflow.com/a/12718449/6700329
//!
//! @return
//! `true` or `false`
template <UST                                            t_idx_first,
          UST                                            t_length,
          FloatVectorRegister                            T_RegisterType,
          std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto
compare_in_sequence_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept -> bool;


//! @brief
//! Return `true` only if all selected register elements are equal.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all selected elements of `lhs` are greater than the corresponding ones in `rhs`.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_greater(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all selected elements of `lhs` are greater or equal than the corresponding ones in `rhs`.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_greater_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all selected elements of `lhs` are less than the corresponding ones in `rhs`.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_less(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if all selected elements of `lhs` are less equal than the corresponding ones in `rhs`.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_less_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;


//! @brief
//! Return `true` only if the comparisons of all selected register elements yields `true`.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored
//! @tparam T_RegisterType:
//! The register type
//! @tparam T_CompFunc:
//! The Type of a callable object that performs the register element comparison.
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//! @param[in] comp_func:
//! A comparison function or functor that provides the comparison of the register elements. Functors should be preferred
//! to assure inlining. See  https://stackoverflow.com/a/12718449/6700329
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType, std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto compare_selected_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept
        -> bool;


//! @brief
//! Return `true` if the whole memory of the passed register is zero and `false` otherwise
//!
//! @tparam T_RegisterType
//! The register type
//!
//! @param[in] a
//! The register that should be checked
//!
//! @return
//! `true` or `false`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto is_memory_zero(T_RegisterType a) noexcept -> bool;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================


#include "mjolnir/core/math/math.h"
#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/utility/parameter_pack.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/x86.h"

#include <cstring>
#include <limits>

namespace mjolnir::x86
{
//! \cond DO_NOT_DOCUMENT
namespace internal
{
//! The following structs are just wrappers around comparison functions from `mjolnir/core/x86/intrinsics.h`. They
//! are used in conjunction with the different generalized comparison functions of this header and define the performed
//! comparison operation. Using a callable wrapper struct as template parameter instead of the function itself makes
//! optimizing the code easier for the compiler. See the following links for more info:
//! https://stackoverflow.com/a/12718449/6700329
//! https://stackoverflow.com/a/356993/6700329

template <FloatVectorRegister T_RegisterType>
struct CompareEqual
{
    [[nodiscard]] inline auto operator()(T_RegisterType lhs, T_RegisterType rhs) const noexcept -> T_RegisterType
    {
        return mm_cmp_eq<T_RegisterType>(lhs, rhs);
    }
};


// --------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
struct CompareGreater
{
    [[nodiscard]] inline auto operator()(T_RegisterType lhs, T_RegisterType rhs) const noexcept -> T_RegisterType
    {
        return mm_cmp_gt<T_RegisterType>(lhs, rhs);
    }
};


// --------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
struct CompareGreaterEqual
{
    [[nodiscard]] inline auto operator()(T_RegisterType lhs, T_RegisterType rhs) const noexcept -> T_RegisterType
    {
        return mm_cmp_ge<T_RegisterType>(lhs, rhs);
    }
};


// --------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
struct CompareLess
{
    [[nodiscard]] inline auto operator()(T_RegisterType lhs, T_RegisterType rhs) const noexcept -> T_RegisterType
    {
        return mm_cmp_lt<T_RegisterType>(lhs, rhs);
    }
};


// --------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
struct CompareLessEqual
{
    [[nodiscard]] inline auto operator()(T_RegisterType lhs, T_RegisterType rhs) const noexcept -> T_RegisterType
    {
        return mm_cmp_le<T_RegisterType>(lhs, rhs);
    }
};


} // namespace internal
//! \endcond


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_all_true(lhs, rhs, internal::CompareEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_greater(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_all_true(lhs, rhs, internal::CompareGreater<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_greater_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_all_true(lhs, rhs, internal::CompareGreaterEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_less(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_all_true(lhs, rhs, internal::CompareLess<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_less_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_all_true(lhs, rhs, internal::CompareLessEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType, std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto compare_all_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept
        -> bool
{
    constexpr UST n_e = num_elements<T_RegisterType>;
    return compare_in_sequence_true<0, n_e>(lhs, rhs, comp_func);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_in_sequence_true<t_idx_start, t_idx_end>(lhs, rhs, internal::CompareEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_greater(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_in_sequence_true<t_idx_start, t_idx_end>(lhs, rhs, internal::CompareGreater<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_greater_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_in_sequence_true<t_idx_start, t_idx_end>(lhs, rhs, internal::CompareGreaterEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_less(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_in_sequence_true<t_idx_start, t_idx_end>(lhs, rhs, internal::CompareLess<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_idx_start, UST t_idx_end, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_in_sequence_less_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_in_sequence_true<t_idx_start, t_idx_end>(lhs, rhs, internal::CompareLessEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <UST                                            t_idx_first,
          UST                                            t_length,
          FloatVectorRegister                            T_RegisterType,
          std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto
compare_in_sequence_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept -> bool
{
    constexpr UST n_e = num_elements<T_RegisterType>;

    static_assert(t_length > 0, "At least 1 element must be compared.");
    static_assert(t_idx_first + t_length <= n_e, "Sequence exceeds data length.");

    constexpr auto get_boolean_array = []() constexpr->std::array<bool, n_e>
    {
        std::array<bool, n_e> arr = {{{0}}};
        for (UST i = t_idx_first; i < t_idx_first + t_length; ++i)
            arr.at(i) = true;
        return arr;
    };
    constexpr auto b = get_boolean_array();


    if constexpr (is_m128d<T_RegisterType>)
        return compare_selected_true<b[0], b[1]>(lhs, rhs, comp_func);
    else if constexpr (is_m128<T_RegisterType> || is_m256d<T_RegisterType>)
        return compare_selected_true<b[0], b[1], b[2], b[3]>(lhs, rhs, comp_func);
    else
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return compare_selected_true<b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]>(lhs, rhs, comp_func);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_selected_true<t_cmp...>(lhs, rhs, internal::CompareEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_greater(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_selected_true<t_cmp...>(lhs, rhs, internal::CompareGreater<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_greater_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_selected_true<t_cmp...>(lhs, rhs, internal::CompareGreaterEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_less(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_selected_true<t_cmp...>(lhs, rhs, internal::CompareLess<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_selected_less_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_selected_true<t_cmp...>(lhs, rhs, internal::CompareLessEqual<T_RegisterType>());
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_cmp, FloatVectorRegister T_RegisterType, std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto compare_selected_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept
        -> bool
{
    constexpr UST n_e    = num_elements<T_RegisterType>;
    constexpr UST n_bits = sizeof(ElementType<T_RegisterType>);
    constexpr UST val    = power_of_2(n_bits) - 1;

    static_assert(sizeof...(t_cmp) == n_e, "Number of template parameters must be equal to the number of elements.");
    static_assert(! pp_all_false<t_cmp...>(), "At least one template parameter must be `true`.");


    auto           result = mm_movemask_epi8(mm_cast_fi(comp_func(lhs, rhs)));
    constexpr auto ref    = bit_construct_from_ints<n_bits, decltype(result), (static_cast<UST>(t_cmp) * val)...>(true);

    if constexpr (! pp_all_true<t_cmp...>())
        result &= ref; // Set bits of elements that shouldn't be compared to zero

    return result == ref;
}


// --------------------------------------------------------------------------------------------------------------------


template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto is_memory_zero(T_RegisterType a) noexcept -> bool
{
    constexpr UST n_bytes   = sizeof(T_RegisterType);
    constexpr UST alignment = alignment_bytes<T_RegisterType>;

    alignas(alignment) constexpr std::array<U8, n_bytes> ref = {{{0}}};

    return ! static_cast<bool>(std::memcmp(&a, &ref, n_bytes));
}


} // namespace mjolnir::x86
