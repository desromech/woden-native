#ifndef WODEN_RENDERING_MATERIAL_HPP
#define WODEN_RENDERING_MATERIAL_HPP

#include "AGPU/agpu.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class ResourceCache> ResourceCachePtr;
class ResourceLoadingContext;
}

namespace Rendering
{
typedef std::shared_ptr<class Material> MaterialPtr;
typedef std::weak_ptr<class Material> MaterialWeakPtr;
class SceneRenderer;

/**
 * I am a material.
 */
class Material
{
public:
    static MaterialPtr getDefaultMaterial();

    virtual bool activateDepthOnlyWithRenderer(SceneRenderer *sceneRenderer) = 0;
    virtual bool activateShadowMapWithRenderer(SceneRenderer *sceneRenderer) = 0;
    virtual bool activateOpaqueWithRenderer(SceneRenderer *sceneRenderer) = 0;
    virtual bool activateTranslucentWithRenderer(SceneRenderer *sceneRenderer) = 0;

    virtual void loadWithContext(Assets::ResourceLoadingContext &loadingContext) = 0;

    agpu_shader_resource_binding_ref materialResourceBinding;
};

class AbstractMaterialFactory
{
public:
    virtual const char *getTypeName() const = 0;
    virtual MaterialPtr makeInstance() const = 0;
};

class MaterialFactoryRegistry
{
public:
    static MaterialFactoryRegistry *Get();

    AbstractMaterialFactory *findFactory(const std::string &typeName)
    {
        auto it = factories.find(typeName);
        if(it != factories.end())
            return it->second;
        return nullptr;
    }

    void registerFactory(AbstractMaterialFactory *factory)
    {
        factories.insert(std::make_pair(factory->getTypeName(), factory));
    }


private:
    std::unordered_map<std::string, AbstractMaterialFactory*> factories;

};

template<typename MT>
class MaterialFactory : public AbstractMaterialFactory
{
public:
    MaterialFactory(const char *initTypeName)
     : typeName(initTypeName)
    {
        MaterialFactoryRegistry::Get()->registerFactory(this);
    }

    virtual const char *getTypeName() const
    {
        return typeName;
    }

    virtual MaterialPtr makeInstance() const
    {
        return std::make_shared<MT> ();
    }

private:
    const char *typeName;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_MATERIAL_HPP
