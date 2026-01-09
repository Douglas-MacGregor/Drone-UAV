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

@unittest.skipUnless(RUN_COMMANDLINE_TESTS, "Skipping commandline tests.")
class CommandLineTests(unittest.TestCase):

    @unittest.skipUnless(RUN_PIGPIO_TEST, "Skipping pigpio installation test.")
    def test_pigpio_installed(self):
        """Test if pigpio is installed."""
        result = subprocess.run("dpkg -l | grep pigpio", capture_output=True, text=True, shell=True, check=False)
        with open('tests/expected/pigpio_installed.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "pigpio installation does not match expected output.")

    @unittest.skipUnless(RUN_I2C_TOOLS_TEST, "Skipping i2c-tools installation test.")
    def test_i2c_tools_installed(self):
        """Test if i2c-tools is installed."""
        result = subprocess.run("dpkg -l | grep i2c-tools", capture_output=True, text=True, shell=True, check=False)
        with open('tests/expected/i2c_tools_installed.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "i2c-tools installation does not match expected output.")

    @unittest.skipUnless(RUN_I2C_TEST, "Skipping I2C enabled test.")
    def test_i2c_enabled(self):
        """Test if I2C is enabled."""
        result = subprocess.run("ls /dev/i2c-*", capture_output=True, text=True, shell=True, check=False)
        with open('tests/expected/i2c_enabled.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "I2C interface is not enabled as expected.")

    @unittest.skipUnless(RUN_SPI_TEST, "Skipping SPI enabled test.")
    def test_spi_enabled(self):
        """Test if SPI is enabled."""
        result = subprocess.run("ls /dev/spi-*", capture_output=True, text=True, shell=True, check=False)
        with open('tests/expected/spi_enabled.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "SPI interface is not enabled as expected.")

@unittest.skipUnless(RUN_INTEGRATION_TESTS, "Skipping integration tests.")
class IntegrationTests(unittest.TestCase):
    @unittest.skipUnless(RUN_MPU6050_I2C_TEST, "Skipping MPU-6050 I2C connection test.")
    def test_mpu6050_i2c_connection(self):
        """Test MPU-6050 I2C connection."""
        result = subprocess.run(["i2cdetect", "-y", "1"], capture_output=True, text=True, shell=False, check=False)
        with open('tests/expected/mpu6050_i2c_connection.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "MPU-6050 I2C connection does not match expected output.")


@unittest.skipUnless(RUN_DEVICE_TESTS, "Skipping device tests.")
class DeviceTests(unittest.TestCase):
    @unittest.skipUnless(RUN_DEVICE_BUILD_TEST, "Skipping device build test.")
    def test_building_device_executables(self):
        """Test MPU-6050 self-test functionality."""
        subdir = Path("devices")  # change this to your subdirectory path
        if not subdir.exists():
            self.skipTest(f"{subdir} does not exist")
        result = subprocess.run(["make"], capture_output=True, text=True, shell=False, check=False, cwd=subdir)
        self.assertEqual(result.returncode, 0, f"Building device executables failed with error: {result.stderr}")

if __name__ == "__main__":
    unittest.main()
