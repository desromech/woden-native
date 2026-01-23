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

};

/**
 *  I am string data source element.
 */
class TableDataSourceStringElement : public TableDataSourceElement
{
public:

    std::string string;
};

/**
 * I am a table data source.
 */
class TableDataSource
{
public:
};

/**
 * I am a table data source which holds a list of elements.
 */
class SimpleTableDataSource : public TableDataSource
{
public:
    void addString(const std::string &stringElement);
    void addElement(const TableDataSourceElementPtr &element);

    std::vector<TableDataSourceElementPtr> elements;
};

/**
 * I am a table morph container.
 */
class TableContainerMorph : public BorderedMorph
{
public:
    TableContainerMorph();
};


} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_TABLE_MORPH_HPP