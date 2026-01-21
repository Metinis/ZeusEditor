#include "ProjectPanel.h"

//#include <tinyfiledialogs.h>

ProjectPanel::ProjectPanel(ZEN::ZEngine* engine, SelectionContext& selection)
    : m_Engine(engine), m_SelectionContext(selection)  {
    //m_Engine->getDispatcher().attach<ZEN::ToggleEditorEvent, ProjectPanel, &ProjectPanel::onToggleEditor>(this);
    m_AssetLibrary = ZEN::Project::getActive()->getAssetLibrary();
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
    const ZEN::AssetID& assetID,
    const std::string& assetName,
    std::vector<ZEN::AssetID>& toRemove,
    const std::unordered_set<std::string>& defaultNames,
    const std::string& payloadType,
    void* texHandle = nullptr,
    const std::function<void()>& onClick = nullptr
) {
    constexpr float size = 64.0f;
    ImGui::PushID(assetID);

    if (ImGui::ImageButton("##thumbnail", texHandle, { size, size }, { 0,1 }, { 1,0 }) && onClick)
        onClick();

    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(payloadType.c_str(), &assetID, sizeof(assetID));
        ImGui::Image(texHandle, { 16, 16 });
        ImGui::Text("%s", assetName.c_str());
        ImGui::EndDragDropSource();
    }

    auto popupName = "ThumbnailContext_" + assetName;
    if (ImGui::BeginPopupContextItem(assetName.c_str())) {
        if (ImGui::MenuItem("Delete")) toRemove.push_back(assetID);
        if (ImGui::MenuItem("Rename")) {  }
        ImGui::EndPopup();
    }

    ImGui::TextWrapped("%s", assetName.c_str());
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
            /*const char* path = tinyfd_openFileDialog("Choose a model", "",
                filters.size(), filters.data(), "3D Model Files", 1);
            if (path) m_Engine->getModelImporter().loadModel(getNameWithoutExtension(path), path);*/
        }

        if (ImGui::MenuItem("Add Texture from Disk")) {
            constexpr std::array filters = { "*.png", "*.jpg", "*.tga" };
            /*
            const char* paths = tinyfd_openFileDialog("Choose a texture", "",
                filters.size(), filters.data(), "Image Files", 1);
            if (paths) {
                std::string allPaths = paths;
                size_t start = 0;
                size_t end = 0;

                while ((end = allPaths.find('|', start)) != std::string::npos) {
                    std::string path = allPaths.substr(start, end - start);
                    m_Engine->getModelImporter().loadTexture(
                        getNameWithoutExtension(path.c_str()),
                        path.c_str()
                    );
                    start = end + 1;
                }

                std::string path = allPaths.substr(start);
                m_Engine->getModelImporter().loadTexture(
                    getNameWithoutExtension(path.c_str()),
                    path.c_str()
                );
            }
            */
        }
        if (ImGui::MenuItem("Create Material")) {
            //ImGui::OpenPopup("CreateMaterialPopup");
            m_OpenCreateMaterialPopup = true;
        }



        ImGui::EndPopup();
    }
}

void ProjectPanel::drawMeshesGrid() {

    std::vector<ZEN::AssetID> toRemove;
    for (auto& assetID : m_AssetLibrary->getAllIDsOfType<ZEN::MeshData>())
        processThumbnail(assetID, m_AssetLibrary->getName(assetID), toRemove, ZEN::defaultMeshes, "MESH_NAME");

    for (auto& assetID : toRemove) {
        m_AssetLibrary->remove(assetID);
    }
}

void ProjectPanel::drawMaterialsGrid() {
    std::vector<ZEN::AssetID> toRemove;

    for (auto& assetID : m_AssetLibrary->getAllIDsOfType<ZEN::Material>()) {
        auto material = m_AssetLibrary->getMaterialRaw(assetID);

        void* texHandle = nullptr;
        if (material.textureID != 0) {
            texHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(
                m_Engine->getRenderer().getResourceManager()->getTexture(material.textureID)
            ));
        }

        processThumbnail(
            assetID,
            m_AssetLibrary->getName(assetID),
            toRemove,
            ZEN::defaultMaterials,
            "MATERIAL_NAME",
            texHandle,
            [&, assetID]() {
                m_SelectionContext.setMaterial(m_AssetLibrary->get<ZEN::Material>(assetID));
            }
        );
    }

    for (auto& assetID : toRemove) {
        m_AssetLibrary->remove(assetID);
    }
}

void ProjectPanel::drawTexturesGrid() {
    std::vector<ZEN::AssetID> toRemove;

    for (auto& assetID : m_AssetLibrary->getAllIDsOfType<ZEN::TextureData>()) {
        auto* tex = m_AssetLibrary->get<ZEN::TextureData>(assetID);
        if (!tex) continue;

        auto resourceManager = ZEN::Application::get().getEngine()->getRenderer().getResourceManager();
        int texID = resourceManager->get<ZEN::GPUTexture>(assetID)->drawableID;

        void* texHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(
            m_Engine->getRenderer().getResourceManager()->getTexture(texID)
        ));

        processThumbnail(
            assetID,
            m_AssetLibrary->getName(assetID),
            toRemove,
            {},
            "TEXTURE_NAME",
            texHandle,
            [&, assetID]() {
                m_SelectionContext.setMaterial(m_AssetLibrary->get<ZEN::Material>(assetID));
            }
        );
    }

    for (auto& assetID : toRemove) {
        m_AssetLibrary->remove(assetID);
    }
}

void ProjectPanel::createMaterialPopup() {
    ImGui::Text("Create a new material?");
    static char matName[256] = "";
    ImGui::InputText("##MaterialName", matName, IM_ARRAYSIZE(matName));
    ImGui::Separator();

    if (ImGui::Button("Create", ImVec2(120, 0))) {
        ZEN::Material material = *ZEN::AssetHandle<ZEN::Material>(ZEN::defaultMaterialID);
        m_AssetLibrary->createAsset(material, matName);
        ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
}

void ProjectPanel::drawAssetGrid() {
    ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
    drawContextMenu();

    //Material popup logic
    if (m_OpenCreateMaterialPopup) {
        ImGui::OpenPopup("CreateMaterialPopup");
        m_OpenCreateMaterialPopup = false;
    }
    if (ImGui::BeginPopupModal(
        "CreateMaterialPopup",
        nullptr,
        ImGuiWindowFlags_AlwaysAutoResize))
    {
        createMaterialPopup();
    }


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
