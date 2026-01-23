#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/TableMorph.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;

    auto dataSource = std::make_shared<SimpleTableDataSource> ();
    dataSource->addString("first");
    dataSource->addString("second");
    dataSource->addString("third");

    auto tableMorph = MakeMorph<TableMorph>();
    tableMorph->setExtent(Vector2(200, 200));
    tableMorph->setDataSource(dataSource);

    tableMorph->openInSystemWindow();


    return 0;
}
