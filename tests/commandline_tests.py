import unittest
import config
import subprocess

@unittest.skipUnless(config.RUN_COMMANDLINE_TESTS, "Skipping commandline tests.")
class CommandLineTests(unittest.TestCase):

    @unittest.skipUnless(config.RUN_PIGPIO_TEST, "Skipping pigpio installation test.")
    def test_pigpio_installed(self):
        """Test if pigpio is installed."""
        result = subprocess.run("dpkg -l | grep pigpio", capture_output=True, text=True, shell=True, check=False)
        with open('expected/pigpio_installed.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "pigpio installation does not match expected output.")

    @unittest.skipUnless(config.RUN_I2C_TOOLS_TEST, "Skipping i2c-tools installation test.")
    def test_i2c_tools_installed(self):
        """Test if i2c-tools is installed."""
        result = subprocess.run("dpkg -l | grep i2c-tools", capture_output=True, text=True, shell=True, check=False)
        with open('expected/i2c_tools_installed.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "i2c-tools installation does not match expected output.")

    @unittest.skipUnless(config.RUN_I2C_TEST, "Skipping I2C enabled test.")
    def test_i2c_enabled(self):
        """Test if I2C is enabled."""
        result = subprocess.run("ls /dev/i2c-*", capture_output=True, text=True, shell=True, check=False)
        with open('expected/i2c_enabled.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "I2C interface is not enabled as expected.")

    @unittest.skipUnless(config.RUN_SPI_TEST, "Skipping SPI enabled test.")
    def test_spi_enabled(self):
        """Test if SPI is enabled."""
        result = subprocess.run("ls /dev/spi*", capture_output=True, text=True, shell=True, check=False)
        with open('expected/spi_enabled.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "SPI interface is not enabled as expected.")
