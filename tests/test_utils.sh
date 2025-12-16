#!/bin/bash

# ============================================
#   Test Utilities - Shared Functions
#   Common functions used across test modules
# ============================================

# ---- Colors ----
GREEN="\e[32m"
RED="\e[31m"
YELLOW="\e[33m"
BLUE="\e[34m"
NC="\e[0m"

# ---- Pretty printing ----
pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
}

fail() {
    local test_name="$1"
    local reason="$2"
    echo -e "${RED}[FAIL]${NC} $test_name"
    if [ -n "$reason" ]; then
        echo -e "${YELLOW}Reason:${NC} $reason"
    fi
}

info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

section() {
    echo ""
    echo "============================================"
    echo "$1"
    echo "============================================"
}

# Check if we're in the tests directory
ensure_tests_dir() {
    if [[ ! $(basename $(pwd)) == "tests" ]]; then
        if [ -d "tests" ]; then
            cd tests
        else
            echo "Error: Cannot find tests directory"
            return 1
        fi
    fi
}