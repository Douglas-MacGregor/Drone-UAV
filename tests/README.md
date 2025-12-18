# Test System Usage Guide

## Basic Usage

```bash
# Run all tests (default skips integration)
./run_tests.sh

# Run specific test types
./run_tests.sh --cmdline-only
./run_tests.sh --unit-only
./run_tests.sh --integration-only
./run_tests.sh --all  # includes integration tests
```

## Toggling Tests On/Off

### Method 1: Environment Variables

```bash
# Skip specific test categories
SKIP_SPI_TESTS=true ./run_tests.sh
SKIP_MPU6050_TESTS=true ./run_tests.sh

# Skip multiple categories
SKIP_SPI_TESTS=true SKIP_PIGPIO_TESTS=true ./run_tests.sh
```

### Method 2: Command Line Options

```bash
# Skip hardware tests
./run_tests.sh --skip-hardware

# Skip specific devices
./run_tests.sh --skip-mpu6050
./run_tests.sh --skip-sx1278

# Development mode (skip all hardware)
./run_tests.sh --dev-mode
```

### Method 3: Configuration Files

```bash
# Use preset configurations
./run_tests.sh --config configs/dev_mode.config      # No hardware
./run_tests.sh --config configs/hardware.config      # Full hardware
./run_tests.sh --config configs/ci.config            # CI/CD pipeline
```

### Method 4: Edit test_config.sh

Edit the `test_config.sh` file to set your default preferences.

## Available Test Controls

| Environment Variable | Description               |
| -------------------- | ------------------------- |
| `SKIP_SPI_TESTS`     | Skip SPI interface tests  |
| `SKIP_PIGPIO_TESTS`  | Skip PIGPIO library tests |
| `SKIP_MPU6050_TESTS` | Skip MPU6050 unit tests   |
| `SKIP_SX1278_TESTS`  | Skip SX1278 unit tests    |

## Common Scenarios

### Development on Mac (no hardware)

```bash
./run_tests.sh --dev-mode
# or
./run_tests.sh --config configs/dev_mode.config
```

### Testing on Raspberry Pi (with hardware)

```bash
./run_tests.sh --config configs/hardware.config
```

### CI/CD Pipeline

```bash
./run_tests.sh --config configs/ci.config
```

### Testing specific component only

```bash
# Only test MPU6050
SKIP_SX1278_TESTS=true ./run_tests.sh --unit-only

# Only test command-line setup
./run_tests.sh --cmdline-only
```

## Adding New Tests

### Command-line tests:

Edit `commandline_tests.sh` and add calls to `add_cmd_test()` in `setup_cmdline_tests()`

### Unit tests:

Add new `.c` files in `devices/` directory and update the Makefile

### Integration tests:

Edit `integration_tests.sh` and add new test functions
