# System Test Runner

Comprehensive test suite for the Drone UAV project with support for multiple configurations.

## Quick Start

```bash
# Run all tests (default configuration)
python3 runner.py

# Run receiver-specific tests (IMU + LoRa, no controller)
python3 runner.py --config reciver

# Run controller-specific tests (LoRa + DualShock, no IMU)
python3 runner.py --config controller

# Run base configuration tests
python3 runner.py --config base
```

## Test Categories

- **Command Line Tests**: System setup validation (SPI, I2C, pigpio)
- **Device Tests**: Hardware component unit tests (MPU-6050, SX1278, DualShock)
- **Integration Tests**: Hardware connectivity and communication tests

## Configurations

| Config       | Command Line | Integration | MPU-6050 | SX1278 | DualShock |
| ------------ | ------------ | ----------- | -------- | ------ | --------- |
| `base`       | ✅           | ✅          | ✅       | ✅     | ✅        |
| `reciver`    | ✅           | ✅          | ✅       | ✅     | ❌        |
| `controller` | Partial      | ❌          | ❌       | ✅     | ✅        |

## Requirements

- Python 3.x with unittest
- Unity C test framework (included)
- Hardware devices for integration tests
- Root access for some hardware tests (`sudo python3 runner.py`)

## Test Structure

```
tests/
├── runner.py              # Main test runner
├── configs/               # Test configurations
│   ├── config_base.py     # All tests enabled
│   ├── config_reciver.py  # Receiver/drone tests
│   └── config_controller.py # Controller/pilot tests
├── devices/               # C unit tests (Unity framework)
├── commandline_tests.py   # System setup tests
├── device_tests.py        # Python device test wrappers
└── integration_tests.py   # Hardware integration tests
```

## Adding Tests

1. Add test methods to appropriate `*_tests.py` files
2. Update config files to enable/disable new tests
3. For C tests: Add to `devices/` and update Makefile

## Common Issues

- **Permission errors**: Run with `sudo` for hardware access
- **Device not found**: Check hardware connections and `/dev/` paths
- **Import errors**: Ensure you're running from the `tests/` directory
