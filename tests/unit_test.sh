#!/bin/bash

# ============================================
#   Unit Test Module
#   Handles building and running Unity tests
# ============================================

# Source the common test utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/test_utils.sh"

# Test counters (local to this module)
UNIT_PASS=0
UNIT_FAIL=0

# Configuration
UNITTEST_DIR="./devices"

# ---- Unit Test Functions ----

build_unit_tests() {
    local current_dir=$(pwd)
    
    info "Building unit tests in $UNITTEST_DIR"
    
    if [ ! -d "$UNITTEST_DIR" ]; then
        fail "Build unit tests" "Directory $UNITTEST_DIR does not exist"
        return 1
    fi
    
    cd "$UNITTEST_DIR" || {
        fail "Build unit tests" "Could not enter $UNITTEST_DIR"
        return 1
    }
    
    make clean > /dev/null 2>&1
    make all > /dev/null 2>&1
    local build_result=$?
    
    cd "$current_dir"
    
    if [ $build_result -ne 0 ]; then
        fail "Build unit tests" "Makefile reported a build failure"
        UNIT_FAIL=$((UNIT_FAIL+1))
        return 1
    else
        pass "Unit test build successful"
        UNIT_PASS=$((UNIT_PASS+1))
        return 0
    fi
}

run_unity_test() {
    local test_executable="$1"
    local test_name="${test_executable#./}"
    
    if [ ! -f "$test_executable" ]; then
        fail "Run $test_name" "Executable $test_executable does not exist"
        UNIT_FAIL=$((UNIT_FAIL+1))
        return 1
    fi
    
    echo ""  # Add spacing before each test
    info "Running $test_name"
    
    $test_executable
    local test_result=$?
    
    if [ $test_result -ne 0 ]; then
        fail "$test_name" "One or more Unity tests failed"
        UNIT_FAIL=$((UNIT_FAIL+1))
        return 1
    else
        pass "$test_name"
        UNIT_PASS=$((UNIT_PASS+1))
        return 0
    fi
}

cleanup_unit_tests() {
    local current_dir=$(pwd)
    
    if [ -d "$UNITTEST_DIR" ]; then
        cd "$UNITTEST_DIR"
        make clean > /dev/null 2>&1
        cd "$current_dir"
    fi
}

run_all_unit_tests() {
    section "Building and Running Unit Tests"
    
    local current_dir=$(pwd)
    ensure_tests_dir
    
    # Build tests
    if ! build_unit_tests; then
        return 1
    fi
    
    # Run tests
    cd "$UNITTEST_DIR" || {
        fail "Run unit tests" "Could not enter $UNITTEST_DIR"
        return 1
    }
    
    # Find and run all test executables
    local test_found=false
    for test_exe in unittest_*; do
        if [ -x "$test_exe" ]; then
            # Check if specific tests should be skipped
            case "$test_exe" in
                *mpu6050*)
                    if [ "${SKIP_MPU6050_TESTS:-false}" = "true" ]; then
                        info "Skipping $test_exe (SKIP_MPU6050_TESTS=true)"
                        continue
                    fi
                    ;;
                *sx1278*)
                    if [ "${SKIP_SX1278_TESTS:-false}" = "true" ]; then
                        info "Skipping $test_exe (SKIP_SX1278_TESTS=true)"
                        continue
                    fi
                    ;;
            esac
            
            test_found=true
            run_unity_test "./$test_exe"
        fi
    done
    
    if [ "$test_found" = false ]; then
        fail "Run unit tests" "No unit test executables found"
        UNIT_FAIL=$((UNIT_FAIL+1))
    fi
    
    # Cleanup
    cleanup_unit_tests
    cd "$current_dir"
    
    # Output summary for this module
    echo ""
    echo "Unit tests: $UNIT_PASS passed, $UNIT_FAIL failed"
    
    # Return results as the last line for parsing by master script
    echo "$UNIT_PASS,$UNIT_FAIL"
}

# Run tests if this script is executed directly
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    run_all_unit_tests
fi