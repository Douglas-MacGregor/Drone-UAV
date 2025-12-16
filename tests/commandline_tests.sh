#!/bin/bash

# ============================================
#   Command-Line Tests Module
#   Handles system-level tests and validation
# ============================================

# Source the common test utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/test_utils.sh"

# Test counters (local to this module)
CMD_PASS=0
CMD_FAIL=0

# ---- Command Line Test Functions ----

run_cmd_test() {
    local name="$1"
    local command="$2"
    local expected_file="$3"
    local fail_msg="$4"

    local actual
    actual=$(eval "$command" 2>/dev/null)
    local command_exit_code=$?

    if [ ! -f "$expected_file" ]; then
        fail "$name" "Expected output file '$expected_file' does not exist"
        CMD_FAIL=$((CMD_FAIL+1))
        return 1
    fi

    local expected
    expected=$(cat "$expected_file")

    if [ "$actual" = "$expected" ]; then
        pass "$name"
        CMD_PASS=$((CMD_PASS+1))
    else
        fail "$name" "$fail_msg"
        echo "---- Expected ----"
        echo "'$expected'"
        echo "---- Actual ----"
        echo "'$actual'"
        echo "---- Command Exit Code: $command_exit_code ----"
        CMD_FAIL=$((CMD_FAIL+1))
    fi
}

add_cmd_test() {
    local name="$1"
    local command="$2"
    local expected_file="$3"
    local fail_msg="$4"
    
    # Store test info for later execution
    CMDLINE_TESTS+=("$name|$command|$expected_file|$fail_msg")
}

run_all_cmdline_tests() {
    section "Running Command-Line Tests"
    
    # Initialize test array if not already done
    if [ ${#CMDLINE_TESTS[@]} -eq 0 ]; then
        setup_cmdline_tests
    fi
    
    info "Found ${#CMDLINE_TESTS[@]} command-line tests"
    
    # Execute all registered tests
    for test_info in "${CMDLINE_TESTS[@]}"; do
        IFS='|' read -r name command expected_file fail_msg <<< "$test_info"
        run_cmd_test "$name" "$command" "$expected_file" "$fail_msg"
    done
    
    # Return results
    echo "$CMD_PASS,$CMD_FAIL"
}

# ---- Test Definitions ----
# Add your command-line tests here
setup_cmdline_tests() {
    CMDLINE_TESTS=()
    
    add_cmd_test "Check SPI is enabled" "ls /dev/spi*" "expected/spi_enabled.out" "SPI interface not available."
    add_cmd_test "Checking that PIGPIO is installed" "dpkg -l | grep pigpio" "expected/pigpio_installed.out" "PIGPIO library is not installed."
    
    # Add more tests by calling add_cmd_test here...
}

# Run tests if this script is executed directly
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    run_all_cmdline_tests
fi