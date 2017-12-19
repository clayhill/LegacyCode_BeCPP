#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <fstream>
#include <experimental/filesystem>

using namespace ::testing;
using namespace std;
namespace fs = std::experimental::filesystem;

namespace {
const fs::path modern_code_binary {fs::current_path() / "ModernCode"};
const fs::path golden_master_file {fs::current_path() / ".." / "tests" / "gm_output.txt"};

bool readFromFile(fs::path file_path, string &capture)
{
    if (std::ifstream is{file_path, ios::binary | ios::ate}) {
        auto size = is.tellg();
        capture.resize(size, '\0');
        is.seekg(0);
        if (is.read(&capture[0], size)) {
            return true;
        }
    }

    return false;
}
} // namespace

TEST(MyLegacyCode, HasGoldenMaster)
{
    EXPECT_TRUE(fs::exists(golden_master_file));
}

TEST(MyLegacyCode, GeneratesOutputIdenticalToGoldenMaster)
{
    fs::path output {fs::current_path() / "output.txt"};
    string command {modern_code_binary.string() + " > " + output.string()};
    std::system(command.c_str());

    string gm;
    string capture;

    if (!readFromFile(golden_master_file, gm)) {
        EXPECT_TRUE(false) << "Could not read golden master";
    }
    else if (!readFromFile(output, capture)) {
        EXPECT_TRUE(false) << "Could not read captured output";
    }
    else {
        EXPECT_THAT(capture, StrEq(gm));
    }
}
