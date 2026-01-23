#include "Woden/Morphic/TableMorph.hpp"
#include "Woden/Morphic/Layout.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

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

float TableContainerMorph::computeVisibleExactRowCount()
{
    return getExtent().y / getTable()->getRowHeight();
}

int TableContainerMorph::computeVisibleRowCount()
{
    return int(ceil(computeVisibleExactRowCount()));
}

int TableContainerMorph::computeStartingRowIndex()
{
    return int(floor(getTable()->rowPositionY / getTable()->getRowHeight()));
}

void TableContainerMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    BorderedMorph::drawWith(renderer);
    drawRowsWith(renderer);
}

void TableContainerMorph::drawRowsWith(const Rendering::GUIRendererPtr &renderer)
{
    const auto &table = getTable();
    int visibleRowCount = computeVisibleRowCount();
    int startingRowIndex = computeStartingRowIndex();

    float positionY = table->rowPositionY;
    auto rowHeight = table->getRowHeight();

    const auto &dataSource = table->getDataSource();
    printf("row height %f\n", rowHeight);
    printf("visible row range %d %d\n", startingRowIndex, startingRowIndex + visibleRowCount);
    printf("dataSource size: %zu\n", dataSource->getNumberOfElements());

    for(int displayIndex = 0; displayIndex < visibleRowCount; ++displayIndex)
    {
        int rowIndex = startingRowIndex + displayIndex;
        float rowPositionY = rowIndex*rowHeight - positionY;

        auto element = dataSource->getElementAtIndex(size_t(rowIndex));
        if(!element)
            continue;

        //Rectangle rowBounds = Rectangle::WithOriginAndExtent();
        printf("%d: %s\n", rowIndex, element->asString().c_str());
    }
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

float TableMorph::getRowHeight()
{
    auto fontFace = Woden::Rendering::RenderingContext::getMainContext()->defaultFontFace;
    return fontFace->getHeight();
}

} // End of namespace Morphic
} // End of namespace Woden
