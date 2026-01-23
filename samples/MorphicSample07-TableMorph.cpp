#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/TableMorph.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;

    auto dataSource = std::make_shared<SimpleTableDataSource> ();
    for(int i = 0; i < 20; ++i)
    {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%d", i);
        dataSource->addString(buffer);
    }

    auto tableMorph = MakeMorph<TableMorph>();
    tableMorph->setExtent(Vector2(200, 200));
    tableMorph->setDataSource(dataSource);

    tableMorph->openInSystemWindow();


    return 0;
}
