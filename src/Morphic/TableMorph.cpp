#include "Woden/Morphic/TableMorph.hpp"

namespace Woden
{
namespace Morphic
{

void SimpleTableDataSource::addString(const std::string &stringElement)
{
    auto element = std::make_shared<TableDataSourceStringElement> ();
    element->string = stringElement;
    addElement(element);
}

void SimpleTableDataSource::addElement(const TableDataSourceElementPtr &element)
{
    elements.push_back(element);
}

TableContainerMorph::TableContainerMorph()
{
}

} // End of namespace Morphic
} // End of namespace Woden
