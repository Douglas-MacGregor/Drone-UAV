import subprocess
import unittest
from pathlib import Path
import unittest
import sys
import argparse
import importlib
import os

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
    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Run drone test suite')
    parser.add_argument('--config', choices=['base', 'controller', 'receiver'], 
                       default='base', help='Configuration to use (default: base)')
    args = parser.parse_args()
    
    # Load the appropriate config and inject it into sys.modules
    config_name = f'config_{args.config}'
    
    # Import from configs directory directly
    configs_dir = os.path.join(os.path.dirname(__file__), 'configs')
    sys.path.insert(0, configs_dir)
    
    config_module = importlib.import_module(config_name)
    
    # Inject the config globally so test modules can import it as configs.config
    sys.modules['configs.config'] = config_module
    
    # Also create the configs module if it doesn't exist
    if 'configs' not in sys.modules:
        configs_module = type(sys)('configs')
        configs_module.config = config_module
        sys.modules['configs'] = configs_module
    
    print(f"Running tests with {args.config} configuration...")
    
    loader = unittest.TestLoader()
    suite = unittest.TestSuite()

    from commandline_tests import CommandLineTests
    from device_tests import MPU6050DeviceTests, SX1278DeviceTests, DualShockDeviceTests
    from intergration_tests import IntegrationTests

    suite.addTests(loader.loadTestsFromTestCase(CommandLineTests))
    suite.addTests(loader.loadTestsFromTestCase(MPU6050DeviceTests))
    suite.addTests(loader.loadTestsFromTestCase(SX1278DeviceTests))
    suite.addTests(loader.loadTestsFromTestCase(DualShockDeviceTests))
    suite.addTests(loader.loadTestsFromTestCase(IntegrationTests))

    runner = unittest.TextTestRunner(
        verbosity=0,
        resultclass=CleanTestResult
    )
    result = runner.run(suite)
    sys.exit(not result.wasSuccessful())

if __name__ == "__main__":
    run_all_tests()
