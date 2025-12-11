#include "ProjectPanel.h"

#include <tinyfiledialogs.h>

ProjectPanel::ProjectPanel(ZEN::ZEngine* engine, SelectionContext& selection)
    : m_Engine(engine), m_SelectionContext(selection)  {
    //m_Engine->getDispatcher().attach<ZEN::ToggleEditorEvent, ProjectPanel, &ProjectPanel::onToggleEditor>(this);

}

/*void ProjectPanel::onToggleEditor(ZEN::ToggleEditorEvent &e) {
    ZEN::Application::get().popOverlay(this);
}*/
static std::string getFileName(const std::string& path) {
    size_t pos = std::max(path.find_last_of('/'), path.find_last_of('\\'));
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
}

static std::string getNameWithoutExtension(const std::string& path) {
    std::filesystem::path p(path);
    return p.stem().string();
}

static void checkWindowFocus(ZEN::EventDispatcher& dispatcher) {
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        ImGui::SetWindowFocus();

    //if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
        //dispatcher.trigger<ZEN::PanelFocusEvent>({ .panel = "Project" });
}

static void drawSearchBar() {
    static char search[128] = "";
    ImGui::InputTextWithHint("##search", "Search assets...", search, sizeof(search));
    ImGui::Separator();
}

static auto processThumbnail = [](
    const std::string& name,
    std::vector<std::string>& toRemove,
    const std::unordered_set<std::string>& defaultNames,
    const std::string& payloadType,
    void* texHandle = nullptr,
    const std::function<void()>& onClick = nullptr
) {
    constexpr float size = 64.0f;
    ImGui::PushID(name.c_str());

    if (ImGui::ImageButton("##thumbnail", texHandle, { size, size }, { 0,1 }, { 1,0 }) && onClick)
        onClick();

    if (ImGui::BeginDragDropSource()) {
        const char* payload = name.c_str();
        ImGui::SetDragDropPayload(payloadType.c_str(), payload, strlen(payload) + 1);
        ImGui::Image(texHandle, { 16, 16 });
        ImGui::Text("%s", payload);
        ImGui::EndDragDropSource();
    }

    if (!defaultNames.contains(name) &&
        ImGui::BeginPopupContextItem(("ThumbnailContext_" + name).c_str())) {
        if (ImGui::MenuItem("Delete")) toRemove.push_back(name);
        if (ImGui::MenuItem("Rename")) { /* TODO */ }
        ImGui::EndPopup();
    }

    ImGui::TextWrapped("%s", name.c_str());
    ImGui::NextColumn();
    ImGui::PopID();
};

void ProjectPanel::drawFolderTree() {
    ImGui::BeginChild("LeftPane", ImVec2(200, 0), true);

    if (ImGui::TreeNode("Assets")) {
        const char* folders[] = { "Meshes", "Materials", "Textures", "Scenes" };
        for (const char* folder : folders)
            if (ImGui::Selectable(folder, m_SelectionContext.selectedFolder == folder))
                m_SelectionContext.selectedFolder = folder;

        ImGui::TreePop();
    }
    ImGui::EndChild();
}

void ProjectPanel::drawContextMenu() {
    if (ImGui::BeginPopupContextWindow("ProjectPanelContext",
            ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        if (ImGui::MenuItem("Add Model from Disk")) {
            constexpr std::array filters = { "*.obj", "*.fbx", "*.glb", "*.gltf" };
            const char* path = tinyfd_openFileDialog("Choose a model", "", filters.size(), filters.data(), "3D Model Files", 0);
            if (path) m_Engine->getModelImporter().loadModel(getNameWithoutExtension(path), path);
        }

        if (ImGui::MenuItem("Add Texture from Disk")) {
            constexpr std::array filters = { "*.png", "*.jpg", "*.tga" };
            const char* path = tinyfd_openFileDialog("Choose a texture", "", filters.size(), filters.data(), "Image Files", 0);
            if (path) m_Engine->getModelImporter().loadTexture(getNameWithoutExtension(path), path);
        }

        ImGui::EndPopup();
    }
}

void ProjectPanel::drawMeshesGrid() {
    std::vector<std::string> toRemove;
    for (auto& [name, mesh] : m_Engine->getModelLibrary().getAllMeshData())
        processThumbnail(name, toRemove, ZEN::defaultMeshes, "MESH_NAME");

    for (auto& name : toRemove) {
        m_Engine->getModelLibrary().removeMeshData(name);
        m_Engine->getModelLibrary().removeMeshDrawable(name);
    }
}

void ProjectPanel::drawMaterialsGrid() {
    std::vector<std::string> toRemove;
    for (auto& [name, material] : m_Engine->getModelLibrary().getAllMaterials()) {
        auto texID = m_Engine->getModelLibrary().getTexture(material->texture);
        auto tex = m_Engine->getRenderer().getResourceManager()->getTexture(texID->id);
        void* texHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(tex));

        processThumbnail(
            name,
            toRemove,
            ZEN::defaultMaterials,
            "MATERIAL_NAME",
            texHandle,
            [&, name]() {
                m_SelectionContext.setMaterial(m_Engine->getModelLibrary().getMaterial(name));
                //m_Engine->getDispatcher().trigger<ZEN::SelectMaterialEvent>(
                    //ZEN::SelectMaterialEvent{ .materialName = name });
            }
        );
    }

    for (auto& name : toRemove) {
        m_Engine->getModelLibrary().removeMaterial(name);
    }
}

void ProjectPanel::drawTexturesGrid() {
    std::vector<std::string> toRemove;
    for (auto& [name, tex] : m_Engine->getModelLibrary().getAllTextures()) {
        void* texHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(
            m_Engine->getRenderer().getResourceManager()->getTexture(tex->id)));

        processThumbnail(
            name, toRemove, {}, "TEXTURE_NAME", texHandle,
            [&, name]() {
                //m_Engine->getDispatcher().trigger<ZEN::SelectMaterialEvent>(
                    //ZEN::SelectMaterialEvent{ .materialName = name });
                m_SelectionContext.setMaterial(m_Engine->getModelLibrary().getMaterial(name));

            }
        );
    }
    for (auto& name : toRemove)
        m_Engine->getModelLibrary().removeTexture(name);
}

void ProjectPanel::drawAssetGrid() {
    ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
    drawContextMenu();

    constexpr float thumbSize = 64.0f, padding = 16.0f;
    const float cellSize = thumbSize + padding;
    const float panelWidth = ImGui::GetContentRegionAvail().x;
    int columns = std::max(1, (int)(panelWidth / cellSize));
    ImGui::Columns(columns, 0, false);

    if (m_SelectionContext.selectedFolder == "Meshes")       drawMeshesGrid();
    else if (m_SelectionContext.selectedFolder == "Materials") drawMaterialsGrid();
    else if (m_SelectionContext.selectedFolder == "Textures")  drawTexturesGrid();

    ImGui::EndChild();
}

void ProjectPanel::onUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 size = io.DisplaySize;

    ImGui::SetNextWindowPos({ 0, size.y * 0.7f });
    ImGui::SetNextWindowSize({ size.x, size.y * 0.3f });

    ImGui::Begin("Project Panel", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    //checkWindowFocus(m_Engine->getDispatcher());
    drawSearchBar();
    drawFolderTree();
    ImGui::SameLine();
    drawAssetGrid();

    ImGui::End();
}

void ProjectPanel::onEvent(ZEN::Event &event) {
    ZEN::EventDispatcher dispatcher(event);

    dispatcher.dispatch<ZEN::RunPlayModeEvent>([this](ZEN::RunPlayModeEvent& e) {return onPlayModeEvent(e); });
}

bool ProjectPanel::onPlayModeEvent(ZEN::RunPlayModeEvent &e) {
    if(e.getPlaying()) {
        ZEN::Application::get().popOverlay(this);
    }
    return false;
}
