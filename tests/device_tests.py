import unittest
import subprocess
from pathlib import Path
import config

@unittest.skipUnless(config.RUN_DEVICE_TESTS, "Skipping device tests.")
class DeviceTests(unittest.TestCase):
    @unittest.skipUnless(config.RUN_MPU6050_BUILD_TEST, "Skipping device build test.")
    def test_mpu6050_build(self):
        """Test building the MPU-6050 device code."""
        result = subprocess.run("make unittest_mpu6050", capture_output=True, text=True, shell=True, check=False, cwd="devices")
        self.assertEqual(result.returncode, 0, "MPU-6050 build output does not match expected output.")
        result = subprocess.run("clean", capture_output=True, text=True, shell=True, check=False, cwd="devices")

