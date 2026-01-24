#include "Woden/Morphic/FileBrowser.hpp"
#include "Woden/Morphic/Layout.hpp"
#include "Woden/Morphic/RootMorph.hpp"
#include "Woden/Utility/FileSystem.hpp"

namespace Woden
{
namespace Morphic
{

std::string FileBrowserDataSourceStringElement::asString() const
{
    if(isDirectory)
        return "[D]" + name;
    else
        return "[F]" + name;
}

FileBrowser::FileBrowser()
{
    color = Vector4();
    setExtent(Vector2(400, 300));
}

void FileBrowser::initialize()
{
    BorderedMorph::initialize();

    auto browserLayout = std::make_shared<VerticalPackingLayout> ();

    // Directory label.
    directoryLabel = MakeMorph<StringMorph> ();
    directoryLabel->label = ".";
    directoryLabel->fitSize();
    addMorph(directoryLabel);

    // Create the table.
    fileTable = MakeMorph<TableMorph>();
    fileTable->onElementActivate = [=](TableDataSourceElementPtr element) {
        onTableElementActivate(element);
    };
    addMorph(fileTable);

    // Create the text input
    fileNameInput = MakeMorph<TextInputMorph> ();
    addMorph(fileNameInput);

    // Create the buttons.
    auto cancelButton = MakeMorph<SimpleButtonMorph> ();
    cancelButton->setLabel("Cancel");
    cancelButton->onClickAction = [=](MorphPtr) {
        onCancelButton();
    };
    addMorph(cancelButton);

    auto acceptButton = MakeMorph<SimpleButtonMorph> ();
    acceptButton->setLabel("Accept");
    acceptButton->onClickAction = [=](MorphPtr) {
        onAcceptButton();
    };
    addMorph(acceptButton);

    auto buttonLayout = std::make_shared<HorizontalPackingLayout> ();
    buttonLayout->addMorph(fileNameInput, 1, true);
    buttonLayout->addMorph(cancelButton, 0, true);
    buttonLayout->addMorph(acceptButton, 0, true);
    
    // Add the container to the layout.
    browserLayout->addMorph(directoryLabel, 0, true);
    browserLayout->addMorph(fileTable, 1, true);
    browserLayout->addLayout(buttonLayout, 0, true);
    setLayout(browserLayout);
}

void FileBrowser::setDirectory(const std::string &directory)
{
    currentDirectory = directory;
    directoryLabel->label = directory;

    fileTableDataSource = std::make_shared<SimpleTableDataSource>();
    auto directoryElements = Utility::readWholeDirectorySorted(directory);
    for(auto &element : directoryElements)
    {
        auto entry = std::make_shared<FileBrowserDataSourceStringElement> ();
        entry->isDirectory = element.isDirectory;
        entry->name = element.name;
        fileTableDataSource->addElement(entry);
    }

    fileTable->setDataSource(fileTableDataSource);

}

void FileBrowser::onCancelButton()
{
    if(onCancelAction)
        onCancelAction();
    getRootMorph()->close();
}

void FileBrowser::onAcceptButton()
{
    if(fileNameInput->text.empty())
        return;

    std::string acceptedFileName = Utility::join(currentDirectory, fileNameInput->text);
    if(onAcceptedFile)
        onAcceptedFile(acceptedFileName);
    getRootMorph()->close();
}

void FileBrowser::onTableElementActivate(TableDataSourceElementPtr element)
{
    auto browserElement = std::static_pointer_cast<FileBrowserDataSourceStringElement> (element);
    std::string selectedPath = Utility::join(currentDirectory, browserElement->name);
    if(browserElement->isDirectory)
    {
        setDirectory(selectedPath);
        return;
    }

    if(onAcceptedFile)
        onAcceptedFile(selectedPath);
    getRootMorph()->close();
}


} // End of namespace Morphic
} // End of namespace Woden