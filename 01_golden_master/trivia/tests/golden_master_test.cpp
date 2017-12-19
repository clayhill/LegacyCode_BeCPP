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
    const fs::path binary_path {fs::current_path() / "TriviaGame"};
    const fs::path golden_master_path {fs::current_path() / ".." / "tests" / "gm_output.txt"};

    bool readFromFile(const fs::path &file_path, string &capture)
    {
        if (ifstream is{file_path, ios::binary | ios::ate}) {
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

TEST(MyTriviaGame, HasGoldenMaster)
{
    EXPECT_TRUE(fs::exists(golden_master_path));
}

TEST(MyTriviaGame, GeneratesOutputIdenticalToGoldenMaster)
{
    fs::path output_path {fs::current_path() / "output.txt"};
    string command {binary_path.string() + " > " + output_path.string()};
    system(command.c_str());

    string gm;
    string capture;

    if (!readFromFile(golden_master_path, gm)) {
        EXPECT_TRUE(false) << "Could not read golden master";
    }
    else if (!readFromFile(output_path, capture)) {
        EXPECT_TRUE(false) << "Could not read captured output";
    }
    else {
        EXPECT_THAT(capture, StrEq(gm));
    }
}
