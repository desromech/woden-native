#ifndef WODEN_EDITOR_LEVEl_MODEL_HPP
#define WODEN_EDITOR_LEVEl_MODEL_HPP

#include "Woden/Morphic/TableMorph.hpp"
#include <memory>
#include <vector>

namespace Woden
{
namespace LevelEditor
{

typedef std::shared_ptr<class LevelModel> LevelModelPtr;
typedef std::shared_ptr<class LevelElement> LevelElementPtr;

class LevelElement : public Morphic::TableDataSourceElement
{
public:
    virtual std::string asString() const override;

    std::string name;
};

class LevelModel : public Morphic::TableDataSource
{
public:
    virtual Morphic::TableDataSourceElementPtr getElementAtIndex(size_t index) override;
    virtual bool isValidIndex(size_t index) override;

    virtual size_t getNumberOfElements() override;
    
    std::vector<LevelElementPtr> elements;
};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_EDITOR_LEVEl_MODEL_HPP