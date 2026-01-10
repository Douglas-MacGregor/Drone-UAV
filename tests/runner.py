import subprocess
import unittest
from pathlib import Path

RUN_COMMANDLINE_TESTS = True
RUN_SPI_TEST = True
RUN_I2C_TEST = True
RUN_I2C_TOOLS_TEST = True
RUN_PIGPIO_TEST = True 

RUN_INTEGRATION_TESTS = True
RUN_MPU6050_I2C_TEST = True

RUN_DEVICE_TESTS = True
RUN_DEVICE_BUILD_TEST = True

import unittest
import sys

class CleanTestResult(unittest.TextTestResult):
    def addSuccess(self, test):
        print(f"[ OK ] {test.shortDescription()}")

    def addFailure(self, test, err):
        print(f"[FAIL] {test.shortDescription()}")
        super().addFailure(test, err)

    def addError(self, test, err):
        print(f"[ERR ] {test.shortDescription()}")
        super().addError(test, err)

    def addSkip(self, test, reason):
        print(f"[SKIP] {test.shortDescription()} ({reason})")

def run_all_tests():
    loader = unittest.TestLoader()
    suite = unittest.TestSuite()

    from test_commandline import CommandLineTests
    from test_device import DeviceTests
    from test_integration import IntegrationTests

    suite.addTests(loader.loadTestsFromTestCase(CommandLineTests))
    suite.addTests(loader.loadTestsFromTestCase(DeviceTests))
    suite.addTests(loader.loadTestsFromTestCase(IntegrationTests))

    runner = unittest.TextTestRunner(
        verbosity=0,
        resultclass=CleanTestResult
    )
    result = runner.run(suite)
    sys.exit(not result.wasSuccessful())

if __name__ == "__main__":
    run_all_tests()
