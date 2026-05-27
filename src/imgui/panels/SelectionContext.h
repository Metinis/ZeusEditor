#pragma once
#include <ZeusEngineCore/engine/Entity.h>
#include <ZeusEngineCore/asset/AssetLibrary.h>

class SelectionContext {
public:
    ZEN::Entity getEntity() const { return m_SelectedEntity; }
    ZEN::Material* getMaterial() const { 
        if(m_SelectedMaterial.has_value())
            return m_SelectedMaterial->get();
        return nullptr;
    }
    ZEN::AssetID getMaterialID() const { return m_SelectedMaterial->id(); }
    void setEntity(ZEN::Entity entity) { m_SelectedEntity = entity; m_SelectedMaterial.reset(); }
    void setMaterial(ZEN::AssetHandle<ZEN::Material> material) {m_SelectedMaterial = material; m_SelectedEntity = ZEN::Entity();}
    std::string selectedFolder{};
private:
    ZEN::Entity m_SelectedEntity;
    std::optional<ZEN::AssetHandle<ZEN::Material>> m_SelectedMaterial{};
};
