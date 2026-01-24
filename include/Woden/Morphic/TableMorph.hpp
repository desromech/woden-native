#ifndef WODEN_MORPHIC_TABLE_MORPH_HPP
#define WODEN_MORPHIC_TABLE_MORPH_HPP

#include "BorderedMorph.hpp"
#include <functional>

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class TableDataSourceElement> TableDataSourceElementPtr;
typedef std::shared_ptr<class TableDataSourceStringElement> TableDataSourceStringElementPtr;
typedef std::shared_ptr<class TableDataSource> TableDataSourcePtr;
typedef std::shared_ptr<class SimpleTableDataSource> SimpleTableDataSourcePtr;
typedef std::shared_ptr<class TableContainerMorph> TableContainerMorphPtr;
typedef std::shared_ptr<class TableMorph> TableMorphPtr;

/**
 * I am a single table data source element.
 */
class TableDataSourceElement
{
public:
    virtual std::string asString() const = 0;
};

/**
 *  I am string data source element.
 */
class TableDataSourceStringElement : public TableDataSourceElement
{
public:
    virtual std::string asString() const;

    std::string string;
};

/**
 * I am a table data source.
 */
class TableDataSource
{
public:
    virtual TableDataSourceElementPtr getElementAtIndex(size_t index) = 0;
    virtual bool isValidIndex(size_t index) = 0;

    virtual size_t getNumberOfElements() = 0;
};

/**
 * I am a table data source which holds a list of elements.
 */
class SimpleTableDataSource : public TableDataSource
{
public:
    void addString(const std::string &stringElement);
    void addElement(const TableDataSourceElementPtr &element);

    virtual TableDataSourceElementPtr getElementAtIndex(size_t index) override;
    virtual bool isValidIndex(size_t index) override;

    virtual size_t getNumberOfElements() override;

    std::vector<TableDataSourceElementPtr> elements;
};

/**
 * I am a table morph container.
 */
class TableContainerMorph : public BorderedMorph
{
public:
    TableContainerMorph();

    float computeVisibleExactRowCount();
    int computeVisibleRowCount();
    int computeStartingRowIndex();
    int computeRowIndexAtPosition(const Vector2 &aPosition);

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;
    virtual void drawRowsWith(const Rendering::GUIRendererPtr &renderer);

    virtual void handleMouseClickEvent(const MouseClickEventPtr &event) override;
    virtual void handleMouseDoubleClickEvent(const MouseDoubleClickEventPtr &event) override;
    virtual void handleMouseWheelEvent(const MouseWheelEventPtr &event) override;

    TableMorphPtr getTable();
};

/**
 * I am a table morph.
 */
class TableMorph : public BorderedMorph
{
public:
    TableMorph();
    virtual void initialize() override;

    const TableContainerMorphPtr &getContainer() const;

    const TableDataSourcePtr &getDataSource() const;
    void setDataSource(const TableDataSourcePtr &newDataSource);

    float getRowHeight();

    void clearSelection();
    bool isSelectedRowIndex(size_t index);
    void selectSingleRow(size_t rowIndex);

    float rowPositionY = 0;

    std::function<void ()> onSelectionChange;
    std::function<void (TableDataSourceElementPtr)> onElementActivate;

protected:
    TableContainerMorphPtr container;    
    TableDataSourcePtr dataSource;
    std::vector<size_t> selectedIndices;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_TABLE_MORPH_HPP