#ifndef WODEN_MORPHIC_TABLE_MORPH_HPP
#define WODEN_MORPHIC_TABLE_MORPH_HPP

#include "BorderedMorph.hpp"

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

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;
    virtual void drawRowsWith(const Rendering::GUIRendererPtr &renderer);

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

protected:
    TableContainerMorphPtr container;    
    TableDataSourcePtr dataSource;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_TABLE_MORPH_HPP