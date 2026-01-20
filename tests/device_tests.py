import unittest
import subprocess
from pathlib import Path
import configs.config as config

@unittest.skipUnless(config.RUN_DEVICE_TESTS and config.RUN_MPU6050_TESTS, "Skipping device tests.")
class MPU6050DeviceTests(unittest.TestCase):
    @unittest.skipUnless(config.RUN_MPU6050_COMPLIE_TEST, "Skipping device build test.")
    def test_mpu6050_build(self):
        """Test building the MPU-6050 unittest code."""
        result = subprocess.run("cmake --build build-rpi --target test_mpu6050", capture_output=True, text=True, shell=True, check=False, cwd="../")
        self.assertEqual(result.returncode, 0, "MPU-6050 build output does not match expected output.")

    @unittest.skipUnless(config.RUN_MPU6050_UNITTESTS, "Skipping MPU-6050 unit tests.")
    def test_mpu6050_unittests(self):
        """Run MPU-6050 Unity unit tests."""

        # Build
        build = subprocess.run(
            ["cmake", "--build", "build-rpi", "--target", "test_mpu6050"],
            cwd="../",
            capture_output=True,
            text=True
        )

        self.assertEqual(
            build.returncode,
            0,
            f"MPU-6050 unit test compile failed:\n{build.stdout}\n{build.stderr}"
        )

        # Run Unity tests
        run = subprocess.run(
            ["./test_mpu6050"],
            cwd="../build-rpi/tests/intergration/",
            capture_output=True,
            text=True
        )

        self.assertEqual(
            run.returncode,
            0,
            f"MPU-6050 unit tests failed:\n{run.stdout}\n{run.stderr}"
        )

        if run.returncode != 0:  
            print(run.stdout)


@unittest.skipUnless(config.RUN_DEVICE_TESTS and config.RUN_SX1278_TESTS, "Skipping device tests.")
class SX1278DeviceTests(unittest.TestCase):
    @unittest.skipUnless(config.RUN_SX1278_COMPLIE_TEST, "Skipping device build test.")
    def test_sx1278_build(self):
        """Test building the SX1278 unittest code."""
        result = subprocess.run("cmake --build build-rpi --target test_sx1278", capture_output=True, text=True, shell=True, check=False, cwd="../")
        self.assertEqual(result.returncode, 0, "SX1278 build output does not match expected output.")
    
    @unittest.skipUnless(config.RUN_SX1278_UNITTESTS, "Skipping SX1278 unit tests.")
    def test_sx1278_unittests(self):
        """Run SX1278 Unity unit tests."""

        # Build
        build = subprocess.run(
            ["cmake", "--build", "build-rpi", "--target", "test_sx1278"],
            cwd="../",
            capture_output=True,
            text=True
        )

        self.assertEqual(
            build.returncode,
            0,
            f"SX1278 unit test build failed:\n{build.stdout}\n{build.stderr}"
        )

        # Run Unity tests
        run = subprocess.run(
            ["./test_sx1278"],
            cwd="../build-rpi/tests/intergration/",
            capture_output=True,
            text=True
        )

        self.assertEqual(
            run.returncode,
            0,
            f"SX1278 unit tests failed:\n{run.stdout}\n{run.stderr}"
        )

        if run.returncode != 0:  
            print(run.stdout)


@unittest.skipUnless(config.RUN_DEVICE_TESTS and config.RUN_DUALSHOCK_TESTS, "Skipping device tests.")
class DUALSHOCKDeviceTests(unittest.TestCase):
    @unittest.skipUnless(config.RUN_DUALSHOCK_COMPLIE_TEST, "Skipping device build test.")
    def test_dualshock_build(self):
        """Test building the DualShock unittest code."""
        result = subprocess.run("cmake --build build-rpi --target test_dualshock", capture_output=True, text=True, shell=True, check=False, cwd="../")
        self.assertEqual(result.returncode, 0, "DualShock build output does not match expected output.")

    @unittest.skipUnless(config.RUN_DUALSHOCK_UNITTESTS, "Skipping DualShock unit tests.")
    def test_dualshock_unittests(self):
        """Run DualShock Unity unit tests."""

        # Build
        build = subprocess.run(
            ["cmake", "--build", "build-rpi", "--target", "test_dualshock"],
            cwd="../",
            capture_output=True,
            text=True
        )

        self.assertEqual(
            build.returncode,
            0,
            f"DualShock unit test build failed:\n{build.stdout}\n{build.stderr}"
        )

        # Run Unity tests
        run = subprocess.run(
            ["./test_dualshock"],
            cwd="../build-rpi/tests/intergration/",
            capture_output=True,
            text=True
        )

        self.assertEqual(
            run.returncode,
            0,
            f"DualShock unit tests failed:\n{run.stdout}\n{run.stderr}"
        )

        if run.returncode != 0:  
            print(run.stdout)

