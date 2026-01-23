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

int TableContainerMorph::computeRowIndexAtPosition(const Vector2 &aPosition)
{
    auto table = getTable();
    return (int)floor(((aPosition.y + table->rowPositionY) / table->getRowHeight()));
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

    auto rowWidth = getExtent().x; 

    for(int displayIndex = 0; displayIndex < visibleRowCount; ++displayIndex)
    {
        int rowIndex = startingRowIndex + displayIndex;
        float rowPositionY = rowIndex*rowHeight - positionY;

        auto element = dataSource->getElementAtIndex(size_t(rowIndex));
        if(!element)
            continue;

        Rectangle rowBounds = Rectangle::WithOriginAndExtent(Vector2(0, rowPositionY), Vector2(rowWidth, rowHeight));

        // Selected display
        if(table->isSelectedRowIndex((size_t)rowIndex))
            renderer->fillRectangleWithColor(rowBounds, Vector4(0.2f, 0.4f, 1.0, 1.0));

        Rectangle rowTextBounds = Rectangle::WithOriginAndExtent(Vector2(5, rowPositionY), Vector2(rowWidth, rowHeight));
        renderer->drawTextInRectangleWithColor(rowTextBounds, element->asString(), Vector4(0, 0, 0, 1));
    }
}

void TableContainerMorph::handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    int rowIndex = computeRowIndexAtPosition(event->position);

    auto table = getTable();
    auto datasource = table->getDataSource();
    if(rowIndex >= 0 && datasource->isValidIndex((size_t) rowIndex))
        table->selectSingleRow(rowIndex);
    else
        table->clearSelection();
}

void TableContainerMorph::handleMouseWheelEvent(const MouseWheelEventPtr &event)
{
    auto table = getTable();
    table->rowPositionY = std::max(0.0f, table->rowPositionY - event->scrollAmount.y*10);
    event->wasHandled = true;
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
    auto tableLayout = std::make_shared<HorizontalPackingLayout> ();

    // Create the container.
    container = MakeMorph<TableContainerMorph>();
    addMorph(container);

    // Add the container to the layout.
    tableLayout->addMorph(container, 1, true);
    setLayout(tableLayout);
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

void TableMorph::clearSelection()
{
    selectedIndices.clear();
}

bool TableMorph::isSelectedRowIndex(size_t index)
{
    for(auto selectedIndex : selectedIndices)
    {
        if(selectedIndex == index)
            return true;
    }

    return false;
}

void TableMorph::selectSingleRow(size_t rowIndex)
{
    selectedIndices.clear();
    selectedIndices.push_back(rowIndex);
}

} // End of namespace Morphic
} // End of namespace Woden
