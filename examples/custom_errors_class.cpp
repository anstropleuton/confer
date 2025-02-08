/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   How to define a custom error counter class for Confer.
 *
 *  @copyright  Copyright (c) 2024 Anstro Pleuton
 *
 *    ____             __
 *   / ___|___  _ __  / _| ___ _ __
 *  | |   / _ \| '_ \| |_ / _ \ '__|
 *  | |__| (_) | | | |  _|  __/ |
 *   \____\___/|_| |_|_|  \___|_|
 *
 *  Confer is a testing framework for Anstro Pleuton's libraries and
 *  programs.
 *
 *  This software is licensed under the terms of MIT License.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  Credits where credit's due:
 *  - ASCII Art generated using https://www.patorjk.com/software/taag with font
 *    "Standard".
 */

#include <cmath>
#include <format>

/**
 *  @brief  Custom error counter class.
 */
struct my_error_t {

    /**
     *  @brief  Internal error counter.
     */
    float errors_count = 0;

    /**
     *  @brief  Constructor taking two arguments.
     */
    inline constexpr my_error_t(float base, float power)
    {
        errors_count = std::pow(base, power);
    }

    /**
     *  @brief   An @c operator() that would increment errors.
     *  @return  Reference to self.
     */
    inline constexpr auto operator() () -> my_error_t &
    {
        errors_count += 0.5f;
        return *this;
    }

    /**
     *  @brief   An @c operator[] that would add to errors.
     *  @return  Reference to self.
     */
    inline constexpr auto operator[] (
        my_error_t addition
    ) -> my_error_t &
    {
        errors_count += addition.errors_count;
        return *this;
    }

    /**
     *  @brief   Get the number of errors.
     *  @return  Number of errors.
     */
    [[nodiscard]] inline constexpr auto get_errors() const
    {
        return (int)(errors_count * 2.0f);
    }

    /**
     *  @brief  Return true if there are any errors accumulated.
     */
    [[nodiscard]] inline constexpr auto has_errors() const
    {
        return errors_count != 0.0f;
    }
};

/**
 *  @brief  Custom variable for errors counter.
 */
#define CT_ERRORS my_errors

/**
 *  @brief  Use custom type for errors counter.
 */
#define CT_ERRORS_TYPE my_error_t

/**
 *  @brief  Pass two parameters during construction of the variable.
 */
#define CT_ERRORS_PARAMS 0.0f, 1.0f

/**
 *  @brief  Increment errors counter.
 */
#define CT_INCREMENT_ERRORS(errors) errors() // Calls operator() of my_error_t

/**
 *  @brief  Add number to errors counter.
 */
#define CT_ADD_TO_ERRORS(errors, number) errors[number]

/**
 *  @brief  Return true if errors counter accumulated any errors.
 */
#define CT_HAS_ERRORS(errors) errors.has_errors()

namespace std {

/**
 *  @brief   Formatter for @c my_error_t .
 *  @tparam  char_type   Character type.
 */
template<typename char_type>
struct formatter<my_error_t, char_type> {

    /**
     *  @brief   Parse the format specifiers.
     *
     *  @tparam  parse_context  Parse context type.
     *  @param   ctx            Parse context.
     *  @return  Iterator to end of format specifier.
     */
    template<typename parse_context>
    [[nodiscard]] inline constexpr auto parse(parse_context &ctx)
    -> parse_context::iterator
    {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}')
        {
            throw std::format_error("No format specifiers for my_error_t");
        }
        return it;
    }

    /**
     *  @brief   Format to string using parsed specifiers.
     *
     *  @tparam  format_context  Format context type.
     *  @param   error           Custom errors class.
     *  @param   ctx             Format context.
     *  @return  Iterator to end of format context.
     */
    template<typename format_context>
    [[nodiscard]] inline constexpr auto format(
        const my_error_t &error,
        format_context   &ctx
    ) const
    {
        return std::ranges::copy(std::format("{}", error.get_errors()),
            ctx.out()).out;
    }
};

} // namespace std

#include "confer.hpp"

/**
 *  @brief  Test the new custom @c my_error_t class.
 */
CT_TESTER_FN(test_custom_errors_class) {
    CT_BEGIN;

    CT_INCREMENT_ERRORS(CT_ERRORS);
    CT_INCREMENT_ERRORS(CT_ERRORS);
    CT_INCREMENT_ERRORS(CT_ERRORS);
    CT_INCREMENT_ERRORS(CT_ERRORS);

    CT_ASSERT(CT_ERRORS.get_errors(), 4,
        "4 error increments should result in 4 errors");

    CT_END; // Must return 4
}

// This example is meant to be a kick-starter to start using my library, but
// they do not teach everything.  Refer to the documentation for more details
auto main() -> int try
{
    test_case custom_errors_class_test_case = {
        .title         = "Test the new custom my_error_t class",
        .function_name = "test_custom_errors_class",
        .function      = test_custom_errors_class
    };

    test_suite suite = {
        .tests       = { &custom_errors_class_test_case },
        .pre_run     = default_pre_runner('='),
        .post_run    = default_post_runner('=')
    };

    auto failed_runs = suite.run();
    print_failed_tests(failed_runs);

    std::println("If it prints \"4 errors\", it is normal");

    return sum_failed_tests_errors(failed_runs).has_errors();
}
catch (const std::exception &e)
{
    logln("Exception occurred: {}", e.what());
}
catch (...)
{
    logln("Unknown exception occurred");
}
