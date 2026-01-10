import unittest
import subprocess
from pathlib import Path
import config

@unittest.skipUnless(config.RUN_DEVICE_TESTS, "Skipping device tests.")
class DeviceTests(unittest.TestCase):
    @unittest.skipUnless(config.RUN_DEVICE_BUILD_TEST, "Skipping device build test.")
    def test_building_device_executables(self):
        """Test MPU-6050 self-test functionality."""
        subdir = Path("devices")  # change this to your subdirectory path
        if not subdir.exists():
            self.skipTest(f"{subdir} does not exist")
        result = subprocess.run(["make"], capture_output=True, text=True, shell=False, check=False, cwd=subdir)
        self.assertEqual(result.returncode, 0, f"Building device executables failed with error: {result.stderr}")
