from conans import ConanFile, CMake, tools
import os

class OmrTestConan(ConanFile):
    generators = "cmake"

    def requirements(self):
        self.requires("omr/0.0.1@{}/{}".format(self.user, self.channel))

    def build(self):
        cmake = CMake(self.settings)
        self.run("cmake \"%s\" %s".format(self.conanfile_directory, cmake_command))
        self.run("cmake --build . %s".format(cmake.build_config))

    def test(self):
        self.run("ctest")
