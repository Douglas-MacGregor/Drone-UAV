import unittest
import subprocess
import config

@unittest.skipUnless(config.RUN_INTEGRATION_TESTS, "Skipping integration tests.")
class IntegrationTests(unittest.TestCase):
    @unittest.skipUnless(config.RUN_MPU6050_I2C_TEST, "Skipping MPU-6050 I2C connection test.")
    def test_mpu6050_i2c_connection(self):
        """Test MPU-6050 I2C connection."""
        result = subprocess.run(["i2cdetect", "-y", "1"], capture_output=True, text=True, shell=False, check=False)
        with open('expected/mpu6050_i2c_connection.out', 'r') as f:
            expected_output = f.read()
        self.assertEqual(result.stdout.strip(), expected_output.strip(), "MPU-6050 I2C connection does not match expected output.")