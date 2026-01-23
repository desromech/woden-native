#include "Woden/Morphic/TableMorph.hpp"
#include "Woden/Morphic/Layout.hpp"

namespace Woden
{
namespace Morphic
{
// TableDataSourceStringElement
std::string TableDataSourceStringElement::asString() const
{
    return string;
}

// SimpleTableDataSource
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

TableDataSourceElementPtr SimpleTableDataSource::getElementAtIndex(size_t index)
{
    if(index < elements.size())
        return elements[index];
    return nullptr;
}

bool SimpleTableDataSource::isValidIndex(size_t index)
{
    return index < elements.size();
}

size_t SimpleTableDataSource::getNumberOfElements()
{
    return elements.size();
}

// Table container morph
TableContainerMorph::TableContainerMorph()
{
    color = Vector4(1, 1, 1, 1);
}

void TableContainerMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    BorderedMorph::drawWith(renderer);
    drawRowsWith(renderer);
}

void TableContainerMorph::drawRowsWith(const Rendering::GUIRendererPtr &renderer)
{
    const auto &dataSource = getTable()->getDataSource();
    printf("dataSource size: %zu\n", dataSource->getNumberOfElements());
}

TableMorphPtr TableContainerMorph::getTable()
{
    return std::static_pointer_cast<TableMorph> (owner.lock());
}

// Table morph
TableMorph::TableMorph()
{
    color = Vector4(1, 1, 1, 1);
}

void TableMorph::initialize()
{
    auto layout = std::make_shared<HorizontalPackingLayout> ();

    // Create the container.
    container = MakeMorph<TableContainerMorph>();
    addMorph(container);

    // Add the container to the layout.
    layout->addMorph(container, 1, true);
    setLayout(layout);
}

const TableContainerMorphPtr &TableMorph::getContainer() const
{
    return container;
}

const TableDataSourcePtr &TableMorph::getDataSource() const
{
    return dataSource;
}

void TableMorph::setDataSource(const TableDataSourcePtr &newDataSource)
{
    dataSource = newDataSource;
}


} // End of namespace Morphic
} // End of namespace Woden
