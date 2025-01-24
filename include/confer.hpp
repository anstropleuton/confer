/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   The main header file for the testing framework.
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

#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "confer_config.hpp"

/**
 *  @brief  Confer version string.
 */
inline constinit std::string_view confer_version = CONFER_VERSION;

#ifndef CT_ERRORS
/**
 *  @brief  Errors counter variable name.
 */
#define CT_ERRORS errors
#endif // ifndef CT_ERRORS

#ifndef CT_ERRORS_TYPE
/**
 *  @brief  Errors counter variable type (must provide suffix ++ operator).
 */
#define CT_ERRORS_TYPE std::size_t
#endif // ifndef CT_ERRORS_TYPE

#ifndef CT_ERRORS_PARAMS
/**
 *  @brief  Errors counter default argument.
 */
#define CT_ERRORS_PARAMS 0
#endif // ifndef CT_ERRORS_PARAMS

#ifndef CT_INCREMENT_ERRORS
/**
 *  @brief  Increment error counter.
 */
#define CT_INCREMENT_ERRORS(errors) errors++; do {} while (false)
#endif // ifndef CT_INCREMENT_ERRORS

#ifndef CT_ADD_TO_ERRORS
/**
 *  @brief  Add number to errors counter.
 */
#define CT_ADD_TO_ERRORS(errors, number) \
errors += number;                        \
do {} while (false)
#endif // ifndef CT_ADD_TO_ERRORS

#ifndef CT_HAS_ERRORS
/**
 *  @brief  Return true if errors counter accumulated any errors.
 */
#define CT_HAS_ERRORS(errors) errors != 0
#endif

#ifndef CT_BEGIN
/**
 *  @brief  Begin testing.  Write this just after the function call.
 */
#define CT_BEGIN                            \
CT_ERRORS_TYPE CT_ERRORS(CT_ERRORS_PARAMS); \
do {} while (false)
#endif // ifndef CT_BEGIN

#ifndef CT_END
/**
 *  @brief  End testing.  Write this just before ending the function.
 */
#define CT_END return CT_ERRORS; do {} while (false)
#endif // ifndef CT_END

#ifndef CT_TESTER_FN
/**
 *  @brief  Define a tester function.
 */
#define CT_TESTER_FN(name) \
auto name() -> CT_ERRORS_TYPE
#endif // ifndef CT_TESTER_FN

#ifndef CT_ASSERT_CODE_FMT
/**
 *  @brief  Assert the condition with customized code if asserted and log
 *          message format parameters.
 */
#define CT_ASSERT_CODE_FMT(value, expected, code, ...) \
if (value != expected)                                 \
{                                                      \
    logln(__VA_ARGS__);                                \
    CT_INCREMENT_ERRORS(CT_ERRORS);                    \
    code;                                              \
}                                                      \
do {} while (false)
#endif // ifndef CT_ASSERT_CODE_FMT

#ifndef CT_ASSERT_FMT
/**
 *  @brief  Assert the condition with customized log message format parameters.
 */
#define CT_ASSERT_FMT(value, expected, ...) \
CT_ASSERT_CODE_FMT(value, expected, {}, __VA_ARGS__)
#endif // ifndef CT_ASSERT_FMT

#ifndef CT_ASSERT_CODE
/**
 *  @brief  Assert the condition with customized code if asserted.
 */
#define CT_ASSERT_CODE(value, expected, message, code)                        \
CT_ASSERT_CODE_FMT(value, expected, code, "{}: {} != {} ({} != {})", message, \
    #value, #expected, value, expected)
#endif // ifndef CT_ASSERT_CODE

#ifndef CT_ASSERT
/**
 *  @brief  Assert the condition.
 */
#define CT_ASSERT(value, expected, message)                                 \
CT_ASSERT_CODE_FMT(value, expected, {}, "{}: {} != {} ({} != {})", message, \
    #value, #expected, value, expected)
#endif // ifndef CT_ASSERT

#ifndef CT_ASSERT_END_FMT
/**
 *  @brief  Assert the condition with customized log message format parameter,
 *          and end the function if assertion failed.
 */
#define CT_ASSERT_END_FMT(value, expected, ...) \
CT_ASSERT_CODE_FMT(value, expected, CT_END, __VA_ARGS__)
#endif // ifndef CT_ASSERT_END_FMT

#ifndef CT_ASSERT_END
/**
 *  @brief  Assert the condition, end the function if assertion failed.
 */
#define CT_ASSERT_END(value, expected, message)                        \
CT_ASSERT_CODE_FMT(value, expected, CT_END, "{}: {} != {} ({} != {})", \
    message, #value, #expected, value, expected)
#endif // ifndef CT_ASSERT_END

#ifndef CT_ASSERT_SIZE
/**
 *  @brief  Assert container size.
 */
#define CT_ASSERT_SIZE(value, expected)                \
CT_ASSERT_END_FMT(value.size(), expected.size(),       \
    "Invalid size: {}.size() != {}.size() ({} != {})", \
    #value, #expected, value.size(), expected.size())
#endif // ifndef CT_ASSERT_SIZE

#ifndef CT_ASSERT_ELM
/**
 *  @brief  Assert the container element.
 */
#define CT_ASSERT_ELM(value, expected, i)                                    \
CT_ASSERT_FMT(value[i], expected[i],                                         \
    "Invalid element: {}[{}] != {}[{}] ({} != {})", #value, i, #expected, i, \
    value[i], expected[i])
#endif // ifndef CT_ASSERT_ELM

#ifndef CT_ASSERT_CTR
/**
 *  @brief  Assert the container.
 */
#define CT_ASSERT_CTR(value, expected)            \
CT_ASSERT_SIZE(value, expected);                  \
for (std::size_t i = 0; i < expected.size(); i++) \
{                                                 \
    CT_ASSERT_ELM(value, expected, i);            \
}                                                 \
do {} while (false)
#endif // ifndef CT_ASSERT_CTR

#ifndef CT_ASSERT_SUB_SIZE
/**
 *  @brief  Assert the nested container size.
 */
#define CT_ASSERT_SUB_SIZE(value, expected, i)                 \
CT_ASSERT_END_FMT(value[i].size(), expected[i].size(),         \
    "Invalid size: {}[{}].size() != {}[{}].size() ({} != {})", \
    #value, i, #expected, i, value[i].size(), expected[i].size())
#endif // ifndef CT_ASSERT_SUB_SIZE

#ifndef CT_ASSERT_SUB_ELM
/**
 *  @brief  Assert the nested container element.
 */
#define CT_ASSERT_SUB_ELM(value, expected, i1, i2)                          \
CT_ASSERT_FMT(value[i1][i2], expected[i1][i2],                              \
    "Invalid element: {}[{}][{}] != {}[{}][{}] ({} != {})", #value, i1, i2, \
    #expected, i1, i2, value[i1][i2], expected[i2][i2])
#endif // ifndef CT_ASSERT_SUB_ELM

#ifndef CT_ASSERT_SUB_CTR
/**
 *  @brief  Assert the nested container.
 */
#define CT_ASSERT_SUB_CTR(value, expected, i)        \
CT_ASSERT_SUB_SIZE(value, expected, i);              \
for (std::size_t j = 0; j < expected[i].size(); j++) \
{                                                    \
    CT_ASSERT_SUB_ELM(value, expected, i, j);        \
}                                                    \
do {} while (false)
#endif // ifndef CT_ASSERT_SUB_CTR

#ifndef CT_ASSERT_NEST_CTR
/**
 *  @brief  Assert both container and nested container.
 */
#define CT_ASSERT_NEST_CTR(value, expected)       \
CT_ASSERT_SIZE(value, expected);                  \
for (std::size_t i = 0; i < expected.size(); i++) \
{                                                 \
    CT_ASSERT_SUB_CTR(value, expected, i);        \
}                                                 \
do {} while (false)
#endif // ifndef CT_ASSERT_NEST_CTR

/**
 *  @brief  Open this file to redirect logging to a file.
 */
extern std::ofstream log_file;

/**
 *  @brief   Log to `std::cout` or provided file.
 *
 *  @tparam  Args    Type of arguments.
 *  @param   format  Format specifier.
 *  @param   args    Arguments.
 */
template<typename ... Args>
[[nodiscard]] inline auto log(
    std::format_string<Args ...> format,
    Args &&...                   args
)
{
    // Print to stdout if no file is specified
    std::ostream &stream = log_file.is_open() ? log_file : std::cout;
    std::print(stream, format, std::forward<Args>(args)...);
}

/**
 *  @brief   Log with newline to std::cout or provided file.
 *
 *  @tparam  Args    Type of arguments.
 *  @param   format  Format specifier.
 *  @param   args    Arguments.
 */
template<typename ... Args>
[[nodiscard]] inline auto logln(
    std::format_string<Args ...> format,
    Args &&...                   args
)
{
    // Print to stdout if no file is specified
    std::ostream &stream = log_file.is_open() ? log_file : std::cout;
    std::println(stream, format, std::forward<Args>(args)...);
}

/**
 *  @brief  Test the function.
 */
struct test_case {

    /**
     *  @brief  Test title, useful to identify failed tests.
     */
    std::string title;

    /**
     *  @brief  Testing function's name.
     */
    std::string function_name;

    /**
     *  @brief  The function to test which returns the number of errors.
     */
    std::function<CT_ERRORS_TYPE()> function;

    /**
     *  @brief   Run the test.
     *  @return  The number of errors within the test.
     */
    [[nodiscard]] inline constexpr auto run() const { return function(); }
};

/**
 *  @brief  A category of tests.
 */
struct test_suite {

    /**
     *  @brief  All the tests.
     */
    std::vector<const test_case *> tests;

    /**
     *  @brief  Function to execute before a test.
     */
    std::function<void (const test_case *)> pre_run;

    /**
     *  @brief  Function to execute after a test.
     */
    std::function<void (const test_case *, CT_ERRORS_TYPE)> post_run;

    /**
     *  @brief  Function to execute after a failed test. Return true to stop
     *          further tests.
     *  @note   Also executes @c post_run
     */
    std::function<bool (const test_case *, CT_ERRORS_TYPE)> run_failed;

    /**
     *  @brief   Run all tests.
     *  @return  The titles and errors count of each failed test.
     */
    [[nodiscard]] inline constexpr auto run()
    {
        std::vector<std::pair<const test_case *,
            CT_ERRORS_TYPE>> failed_tests = {};
        for (auto &test : tests)
        {
            if (pre_run) pre_run(test);
            CT_ERRORS_TYPE errors   = test->run();
            bool           end_test = false;

            if (CT_HAS_ERRORS(errors))
            {
                failed_tests.emplace_back(test, errors);
                if (run_failed && run_failed(test, errors))
                {
                    end_test = true;
                }
            }

            if (post_run) post_run(test, errors);
            if (end_test) break;
        }
        return failed_tests;
    };
};

/**
 *  @brief   Get fancy default pre-run function for decorated title output.
 *
 *  @param   decor_char   Decoration character for before and after title.
 *  @param   decor_count  Decoration character count.
 *  @return  A decorator function.
 */
[[nodiscard]] inline auto default_pre_runner(
    char        decor_char,
    std::size_t decor_count = 1
)
{
    std::string decor = std::ranges::views::repeat(decor_char, decor_count)
                      | std::ranges::to<std::string>();
    return [=](const test_case *test) {
        logln("{} {} {}", decor, test->title, decor);
    };
}

/**
 *  @brief   Get fancy default post-run function for decorated title output.
 *
 *  @param   decor_char   Decoration character for before and after title.
 *  @param   decor_count  Decoration character count.
 *  @return  A decorator function.
 */
[[nodiscard]] inline auto default_post_runner(
    char        decor_char,
    std::size_t decor_count = 1
)
{
    std::string decor = std::views::repeat(decor_char, decor_count)
                        | std::ranges::to<std::string>();
    return [=](const test_case *test, CT_ERRORS_TYPE errors) {
        logln("{} End of {}, {} errors {}\n", decor, test->title, errors,
            decor);
    };
}

/**
 *  @brief   Terminate further tests after printing failed test title.
 *  @return  A quitter function.
 */
[[nodiscard]] inline auto default_run_failed_quitter()
{
    return [=](const test_case *test, CT_ERRORS_TYPE errors) {
        logln("{} failed, cannot conduct further tests.", test->title);
        return true;
    };
}

/**
 *  @brief  Print all failed test's title and errors count.
 *  @param  failed_tests  Failed test results.
 */
inline auto print_failed_tests(
    const std::vector<std::pair<const test_case *,
        CT_ERRORS_TYPE>> &failed_tests
)
{
    if (failed_tests.empty()) return;

    logln("Failed tests:");
    for (auto &failed_test : failed_tests)
    {
        logln("  {}: {} errors", failed_test.first->title, failed_test.second);
    }
}

/**
 *  @brief   Get sum of number of errors in all failed tests.
 *
 *  @param   failed_tests  Failed test results.
 *  @return  The number of total errors.
 */
[[nodiscard]] inline constexpr auto sum_failed_tests_errors(
    const std::vector<std::pair<const test_case *,
        CT_ERRORS_TYPE>> &failed_tests
)
{
    CT_ERRORS_TYPE errors(CT_ERRORS_PARAMS);
    std::ranges::for_each(failed_tests, [&](const auto &failed_test) {
        CT_ADD_TO_ERRORS(errors, failed_test.second);
    });
    return errors;
}
