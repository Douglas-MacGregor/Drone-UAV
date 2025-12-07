#!/bin/bash

# ============================================
#   Combined Test Runner
#   1. Command-line tests
#   2. Build unit tests (Makefile)
#   3. Run Unity unit tests (./unittest)
# ============================================

# ---- Colors ----
GREEN="\e[32m"
RED="\e[31m"
YELLOW="\e[33m"
NC="\e[0m"

PASS=0
FAIL=0

# ---- Pretty printing ----
pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
    PASS=$((PASS+1))
}

fail() {
    echo -e "${RED}[FAIL]${NC} $1"
    echo -e "${YELLOW}Reason:${NC} $2"
    FAIL=$((FAIL+1))
}

section() {
    echo ""
    echo "============================================"
    echo "$1"
    echo "============================================"
}

# ============================================
#   1. Command-line tests
# ============================================
section "Running Command-Line Tests"

run_cmd_test() {
    local name="$1"
    local command="$2"
    local expected_file="$3"
    local fail_msg="$4"

    local actual
    actual=$(eval "$command")

    local expected
    expected=$(cat "$expected_file")

    if [ "$actual" = "$expected" ]; then
        pass "$name"
    else
        fail "$name" "$fail_msg"
        echo "---- Expected ----"
        echo "$expected"
        echo "---- Actual ----"
        echo "$actual"
    fi
}

# === YOUR COMMAND-LINE TESTS HERE ===
# Example:
run_cmd_test \
    "Check SPI is enabled" \
    "ls /dev/spi*" \
    "expected/spi_enabled.out" \
    "Version text did not match expected output."

run_cmd_test \
    "Checking that PIGPIO is installed" \
    "dpkg -l | grep pigpio" \
    "expected/pigpio_installed.out" \
    "PIGPIO library is not installed."

# Add more tests below…
# run_cmd_test "Name" "cmd" "expected/file" "Fail message"


# ============================================
#   2. Build unit tests (via Makefile)
# ============================================
section "Building Unit Tests with Makefile"

make unittest > /dev/null 2>&1
if [ $? -ne 0 ]; then
    fail "Building unittest" "Makefile reported a build failure."
else
    pass "Build successful"
fi


# ============================================
#   3. Run Unity unit tests (executable: ./unittest)
# ============================================
section "Running Unity Unit Tests"

if [ ! -f "./unittest" ]; then
    fail "Run Unity Tests" "Executable ./unittest does not exist."
else
    ./unittest
    UT_STATUS=$?

    if [ $UT_STATUS -ne 0 ]; then
        fail "Unity Tests" "One or more Unity tests failed."
    else
        pass "Unity Tests"
    fi
fi


# ============================================
#   Summary
# ============================================
section "Test Summary"

echo -e "Passed: ${GREEN}$PASS${NC}"
echo -e "Failed: ${RED}$FAIL${NC}"
echo ""

if [ $FAIL -ne 0 ]; then
    exit 1
else
    exit 0
fi
