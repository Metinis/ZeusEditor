#pragma once
#include <ZeusEngineCore/engine/Entity.h>
#include <ZeusEngineCore/asset/AssetLibrary.h>

class SelectionContext {
public:
    ZEN::Entity getEntity() const { return m_SelectedEntity; }
    ZEN::Material* getMaterial() const { return m_SelectedMaterial; }
    void setEntity(ZEN::Entity entity) { m_SelectedEntity = entity; m_SelectedMaterial = nullptr; }
    void setMaterial(ZEN::Material* material) {m_SelectedMaterial = material; m_SelectedEntity = ZEN::Entity();}
    std::string selectedFolder{};
private:
    ZEN::Entity m_SelectedEntity;
    ZEN::Material* m_SelectedMaterial{};
};
