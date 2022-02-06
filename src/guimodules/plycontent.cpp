#include "guimodules/plycontent.h"

PlyContentModule::PlyContentModule(std::string name, Mesh* mesh)
: GUIModule() {
	this->mesh = mesh;
	this->title = name;

	this->Init();
}

PlyContentModule::PlyContentModule(PlyContentModule* module)
: GUIModule() {
	this->title = module->GetTitle();
	this->mesh = module->GetMesh();

	this->Init();
}

PlyContentModule::~PlyContentModule() {}

Mesh* PlyContentModule::GetMesh() {
	return this->mesh;
}

void PlyContentModule::Init() {
	this->tableFlags = ImGuiTableFlags_Borders;
}

void PlyContentModule::Render() {
	if (this->mesh != nullptr) {
		if (ImGui::Begin(std::string(this->title + "###" + std::to_string(this->id)).c_str())) {
			ImGui::Text("Vertices:");
			if (ImGui::BeginTable("Vertices", (this->mesh->haveColors ? 6 : 3), this->tableFlags)) {
				ImGui::TableNextColumn();
				ImGui::TableHeader("x");
				ImGui::TableNextColumn();
				ImGui::TableHeader("y");
				ImGui::TableNextColumn();
				ImGui::TableHeader("z");
				if (this->mesh->haveColors) {
					ImGui::TableNextColumn();
					ImGui::TableHeader("r");
					ImGui::TableNextColumn();
					ImGui::TableHeader("g");
					ImGui::TableNextColumn();
					ImGui::TableHeader("b");
				}

				int max = 10;
				if (this->mesh->nbVertices < max)
					max = this->mesh->nbVertices;
				for (int i = 0; i < max; i++) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%.2f", this->mesh->verticesPosition[3 * i]);
					ImGui::TableNextColumn();
					ImGui::Text("%.2f", this->mesh->verticesPosition[3 * i + 1]);
					ImGui::TableNextColumn();
					ImGui::Text("%.2f", this->mesh->verticesPosition[3 * i + 2]);
					if (this->mesh->haveColors) {
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", this->mesh->verticesColor[3 * i]);
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", this->mesh->verticesColor[3 * i + 1]);
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", this->mesh->verticesColor[3 * i + 2]);
					}
				}
			}
			ImGui::EndTable();

			ImGui::Text("Faces:");
			if (ImGui::BeginTable("Faces",
					(this->mesh->haveMaterials ? 4 : 3), this->tableFlags)) {
				ImGui::TableNextColumn();
				ImGui::TableHeader("v0");
				ImGui::TableNextColumn();
				ImGui::TableHeader("v1");
				ImGui::TableNextColumn();
				ImGui::TableHeader("v2");
				if (this->mesh->haveMaterials) {
					ImGui::TableNextColumn();
					ImGui::TableHeader("mat");
				}

				int max = 10;
				if (this->mesh->nbFaces < max)
					max = this->mesh->nbFaces;
				for (int i = 0; i < max; i++) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%u", this->mesh->facesVertices[3 * i]);
					ImGui::TableNextColumn();
					ImGui::Text("%u", this->mesh->facesVertices[3 * i + 1]);
					ImGui::TableNextColumn();
					ImGui::Text("%u", this->mesh->facesVertices[3 * i + 2]);
					if (this->mesh->haveMaterials) {
						ImGui::TableNextColumn();
						ImGui::Text("%u", this->mesh->facesMaterial[i]);
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();
	}
}
