#include "UnitTest++/UnitTest++.h"
#include "Woden/Utility/FileSystem.hpp"

SUITE(FileSystemUtilities)
{
    TEST(Dirname)
    {
        CHECK_EQUAL("testDir", Woden::Utility::dirname("testDir/testFile.ext"));
    }

    TEST(Basename)
    {
        CHECK_EQUAL("testFile.ext", Woden::Utility::basename("testDir/testFile.ext"));
    }

    TEST(Extension)
    {
        CHECK_EQUAL(".ext", Woden::Utility::extension("testDir/testFile.ext"));
    }

    TEST(Join)
    {
        CHECK_EQUAL("testDir/testFile.ext", Woden::Utility::join("testDir", "testFile.ext"));
        CHECK_EQUAL("testDir/testFile.ext", Woden::Utility::join("testDir/", "testFile.ext"));
#ifdef _WIN32
        CHECK_EQUAL("c:\\testFile.ext", Woden::Utility::join("testDir", "c:\\testFile.ext"));
#else
        CHECK_EQUAL("/testFile.ext", Woden::Utility::join("testDir", "/testFile.ext"));
#endif
    }
}