from conans import ConanFile, ConfigureEnvironment

class OmrConan(ConanFile):
    name = "omr"
    version = "0.0.1"
    description = "Reusable components for building language runtimes."
    url = "http://www.eclipse.org/omr/"
    license = "Eclipse Public License v1.0", "Apache v2.0 License"
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        self.run("git clone --depth=1 --branch=master https://github.com/eclipse/omr.git")

    def build(self):
        env = ConfigureEnvironment(self)
        self.run("cd omr && {} ./configure".format(env.command_line_env))
        self.run("make -j4 -C omr/jitbuilder")

    def package(self):
        self.copy("*.a", dst="lib", src="omr/", keep_path=False)
        self.copy("*", dst="include/compiler", src="omr/compiler/")
        self.copy("*", dst="include", src="omr/jitbuilder/release/include/compiler")
        self.copy("Jit.hpp", dst="include", src="omr/jitbuilder/release/include")

    def package_info(self):
        self.cpp_info.libs = ["libjitbuilder.a"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.cppflags = ["-fno-rtti", "-fno-exceptions"]

    def test(self):
        pass
