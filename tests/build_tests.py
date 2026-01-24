import unittest
import configs.config as config
import subprocess

@unittest.skipUnless(config.RUN_BUILD_TESTS, "Skipping build tests.")
class BuildTests(unittest.TestCase):
    @unittest.skipUnless(config.RUN_RASPBERRUY_PI_BUILD_TEST, "Skipping Raspberry Pi build test.")
    def test_build_rasp_process(self):
        """Test the Raspberry Pi build process."""
        result = subprocess.run( "cmake -B build-rpi -DCMAKE_TOOLCHAIN_FILE=toolchains/raspberry_pi.cmake", capture_output=True, text=True, shell=True, check=False, cwd="../")
        self.assertEqual(result.returncode, 0, "Raspberry Pi build process failed.")