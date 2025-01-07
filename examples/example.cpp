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

#include <vector>

#include "confer.hpp"

/**
 *  @brief  Test basic assertion.
 *  @return  Number of errors.
 */
[[nodiscard]] CT_TESTER_FN(test_basic_assertion) { // Test function
    // Test function should begin with CT_BEGIN (even before local variable
    // declaration)
    CT_BEGIN;

    // Test assertion
    CT_ASSERT(1 + 1, 2, "1 + 1 should equal 2");

    CT_ASSERT(2 * 2, 4, "2 * 2 should equal 4");

    // Customized assertion
    CT_ASSERT_CODE_FMT(3 * 3, 8, {
        // Assertion failed, run the code
        int value = 8;
        value++;
        // We are now good... except no?
    }, "3 * 3 should equal 8... should it?");

    // Test function should end with CT_END (after everything)
    CT_END;
}

/**
 *  @brief  Test container assertion.
 *  @return  Number of errors.
 */
[[nodiscard]] CT_TESTER_FN(test_container_assertion) {
    CT_BEGIN;

    std::vector value    = { 1, 2, 3, 4, 5 };
    std::vector expected = { 1, 2, 3, 4, 5 };

    // Test the container
    CT_ASSERT_CTR(value, expected);

    std::vector<std::vector<int>> nested_value = {
        { 1,  2,  3,  4,  5  },
        { 6,  7,  8,  9,  10 },
        { 11, 12, 13, 14, 15 }
    };

    std::vector<std::vector<int>> nested_expected = {
        { 1,  2,  3,  4,  5  },
        { 6,  7,  8,  9,  10 },
        { 11, 12, 13, 14, 15 }
    };

    // Test nested container
    CT_ASSERT_NEST_CTR(nested_value, nested_expected);

    CT_END;
}

/**
 *  @brief  Run all the assertions.
 *  @return  Zero on success.
 */
auto main() -> int
{
    // Redirect the logs to a file
    log_file.open("assertion_test.log");

    test_suite suite = {
        .pre_run = default_pre_runner('=', 3),
        .post_run = default_post_runner('=', 3)
    };

    // Test for each test function
    test basic_assertion_test = {
        .title = "Test basic assertion",
        .function_name = "test_basic_assertion",
        .function = test_basic_assertion
    };

    test container_assertion_test = {
        .title = "Test container assertion",
        .function_name = "test_container_assertion",
        .function = test_container_assertion
    };

    suite.tests.emplace_back(&basic_assertion_test);
    suite.tests.emplace_back(&container_assertion_test);

    // Pointers of all the failed tests
    auto failed_tests = suite.run();

    // Redirect any subsequent logs to std::cout
    log_file.close();

    // Report all the failed tests
    print_failed_tests(failed_tests);

    std::println("See file assertion_test.log for logs");

    // Return the sum of number of errors in each tests that are failed
    return sum_failed_tests_errors(failed_tests);
}
