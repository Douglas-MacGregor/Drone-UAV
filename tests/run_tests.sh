#!/bin/bash

# ============================================
#   Master Test Runner
#   Orchestrates all test modules in the project
#   Master Test Runner
#   Orchestrates all test modules in the project
# ============================================

# Get script directory for relative imports
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/test_utils.sh"

# Global test counters
TOTAL_PASS=0
TOTAL_FAIL=0
# Get script directory for relative imports
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/test_utils.sh"

# Global test counters
TOTAL_PASS=0
TOTAL_FAIL=0

# Test module flags
RUN_CMDLINE=true
RUN_UNIT=true
RUN_INTEGRATION=false  # Default disabled for integration tests

# ---- Helper Functions ----

show_usage() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -c, --cmdline-only      Run only command-line tests"
    echo "  -u, --unit-only         Run only unit tests"
    echo "  -i, --integration-only  Run only integration tests"
    echo "  -a, --all               Run all tests including integration"
    echo "  -h, --help             Show this help message"
}

parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -c|--cmdline-only)
                RUN_CMDLINE=true
                RUN_UNIT=false
                RUN_INTEGRATION=false
                shift
                ;;
            -u|--unit-only)
                RUN_CMDLINE=false
                RUN_UNIT=true
                RUN_INTEGRATION=false
                shift
                ;;
            -i|--integration-only)
                RUN_CMDLINE=false
                RUN_UNIT=false
                RUN_INTEGRATION=true
                shift
                ;;
            -a|--all)
                RUN_CMDLINE=true
                RUN_UNIT=true
                RUN_INTEGRATION=true
                shift
                ;;
            -h|--help)
                show_usage
                exit 0
                ;;
            *)
                echo "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
}

run_test_module() {
    local module_name="$1"
    local module_script="$2"
    
    if [ ! -f "$SCRIPT_DIR/$module_script" ]; then
        fail "Run $module_name" "Module script $module_script not found"
        TOTAL_FAIL=$((TOTAL_FAIL+1))
        return 1
    fi
    
    # Execute module and capture all output
    local temp_file=$(mktemp)
    "$SCRIPT_DIR/$module_script" > "$temp_file" 2>&1
    local module_status=$?
    
    # Show all output except the last line (which contains results)
    head -n -1 "$temp_file"
    
    # Get the last line which should contain the results
    local results
    results=$(tail -n 1 "$temp_file")
    rm -f "$temp_file"
    
    # Parse results (format: "pass_count,fail_count")
    if [[ $results =~ ^[0-9]+,[0-9]+$ ]]; then
        IFS=',' read -r pass_count fail_count <<< "$results"
        TOTAL_PASS=$((TOTAL_PASS + pass_count))
        TOTAL_FAIL=$((TOTAL_FAIL + fail_count))
    else
        # Module didn't return proper format, assume failure
        if [ $module_status -eq 0 ]; then
            TOTAL_PASS=$((TOTAL_PASS + 1))
        else
            TOTAL_FAIL=$((TOTAL_FAIL + 1))
        fi
    fi
    
    return $module_status
}

main() {
    # Parse command line arguments
    parse_arguments "$@"
    
    # Ensure we're in the correct directory
    ensure_tests_dir
    
    section "Drone UAV Test Suite"
    info "Running modular test suite..."
    
    # Run selected test modules
    if [ "$RUN_CMDLINE" = true ]; then
        run_test_module "Command-line Tests" "commandline_tests.sh"
    fi
    
    if [ "$RUN_UNIT" = true ]; then
        run_test_module "Unit Tests" "unit_test.sh"
    fi
    
    if [ "$RUN_INTEGRATION" = true ]; then
        run_test_module "Integration Tests" "integration_tests.sh"
    fi
    
    # ============================================
    #   Summary
    # ============================================
    section "Test Summary"
    
    echo -e "Total Passed: ${GREEN}$TOTAL_PASS${NC}"
    echo -e "Total Failed: ${RED}$TOTAL_FAIL${NC}"
    echo ""
    
    if [ $TOTAL_FAIL -ne 0 ]; then
        echo -e "${RED}Some tests failed!${NC}"
        exit 1
    else
        echo -e "${GREEN}All tests passed!${NC}"
        exit 0
    fi
}

# Run main function with all arguments
main "$@"
