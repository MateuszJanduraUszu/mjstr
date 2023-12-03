// main.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <unit/string.hpp>
#include <unit/string_view.hpp>
#include <unit/string_view_iterator.hpp>

int main() {
    ::testing::InitGoogleTest();
    return ::RUN_ALL_TESTS();
}