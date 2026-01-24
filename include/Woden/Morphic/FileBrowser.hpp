#ifndef WODEN_MORPHIC_FILE_BROWSER_HPP
#define WODEN_MORPHIC_FILE_BROWSER_HPP

#include "TableMorph.hpp"
#include "StringMorph.hpp"
#include "SimpleButtonMorph.hpp"
#include "TextInputMorph.hpp"

namespace Woden
{
namespace Morphic
{

/**
 *  I am a file or directory in the file browser
 */
class FileBrowserDataSourceStringElement : public TableDataSourceElement
{
public:
    virtual std::string asString() const;

    bool isDirectory = false;
    std::string name;
};

/**
 * I am a file browser.
 */
class FileBrowser : public BorderedMorph
{
public:
    FileBrowser();

    virtual void initialize() override;
    void setDirectory(const std::string &directory);

    std::string currentDirectory;

    StringMorphPtr directoryLabel;
    TableMorphPtr fileTable;
    SimpleTableDataSourcePtr fileTableDataSource;
    TextInputMorphPtr fileNameInput;
};

/**
 * I am an open file browser.
 */
class OpenFileBrowser : public FileBrowser
{
public:
};

/**
 * I am a save file browser.
 */
class SaveFileBrowser : public FileBrowser
{
public:
};



} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_FILE_BROWSER_HPP
