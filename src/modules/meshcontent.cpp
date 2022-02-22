#include "modules/meshcontent.h"

#define MAX_CONTENT		15

MeshContentModule::MeshContentModule(void* context, std::string name,
		Mesh* mesh)
		: GUIModule(context)
		, mesh(mesh) {
	this->title = name;

	this->Init();
}

MeshContentModule::MeshContentModule(MeshContentModule* module)
		: GUIModule(module->GetContext())
		, mesh(module->GetMesh()) {
	this->title = module->GetTitle();

	this->Init();
}

MeshContentModule::~MeshContentModule() {}

Mesh* MeshContentModule::GetMesh() {
	return this->mesh;
}

void MeshContentModule::Init() {
	this->tableFlags = ImGuiTableFlags_Borders;
}

void MeshContentModule::Render() {
	if (this->mesh != nullptr) {
		if (ImGui::Begin(std::string(this->title + "###"
				+ std::to_string(this->id)).c_str())) {
			ImGui::Text("Informations:");
			ImGui::Text("  Nb vertices: %u",
					this->mesh->nbVertices);
			ImGui::Text("  Nb faces: %u",
					this->mesh->nbFaces);
			ImGui::Text("  Have colors: %s",
					(this->mesh->HaveColors() ? "yes" : "no"));
			ImGui::Text("  Have materials: %s",
					(this->mesh->HaveMaterials() ? "yes" : "no"));
			ImGui::Text("");

			ImGui::Text("Values range:");
			ImGui::Text("  x: [ %.2f, %.2f ]",
					this->mesh->GetBoundingBox().min().x(),
					this->mesh->GetBoundingBox().max().x());
			ImGui::Text("  y: [ %.2f, %.2f ]",
					this->mesh->GetBoundingBox().min().y(),
					this->mesh->GetBoundingBox().max().y());
			ImGui::Text("  z: [ %.2f, %.2f ]",
					this->mesh->GetBoundingBox().min().z(),
					this->mesh->GetBoundingBox().max().z());
			if (this->mesh->HaveMaterials())
				ImGui::Text("  mat ID: [ %u, %u ]",
						this->mesh->GetMaterialsRange().min()[0],
						this->mesh->GetMaterialsRange().max()[0]);
			ImGui::Text("");

			ImGui::Text("Content:");
			ImGui::Text("  Vertices:");
			if (ImGui::BeginTable("Vertices",
					(this->mesh->HaveColors() ? 7 : 4),
					this->tableFlags)) {
				ImGui::TableNextColumn();
				ImGui::TableNextColumn();
				ImGui::TableHeader("x");
				ImGui::TableNextColumn();
				ImGui::TableHeader("y");
				ImGui::TableNextColumn();
				ImGui::TableHeader("z");
				if (this->mesh->HaveColors()) {
					ImGui::TableNextColumn();
					ImGui::TableHeader("r");
					ImGui::TableNextColumn();
					ImGui::TableHeader("g");
					ImGui::TableNextColumn();
					ImGui::TableHeader("b");
				}

				unsigned int max = MAX_CONTENT;
				if (this->mesh->nbVertices < max)
					max = this->mesh->nbVertices;
				for (unsigned int i = 0; i < max; i++) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%u", i);
					ImGui::TableNextColumn();
					ImGui::Text("%.2f",
							this->mesh->verticesData[i].position.x());
					ImGui::TableNextColumn();
					ImGui::Text("%.2f",
							this->mesh->verticesData[i].position.y());
					ImGui::TableNextColumn();
					ImGui::Text("%.2f",
							this->mesh->verticesData[i].position.z());
					if (this->mesh->HaveColors()) {
						ImGui::TableNextColumn();
						ImGui::Text("%.2f",
								this->mesh->verticesData[i].color.x());
						ImGui::TableNextColumn();
						ImGui::Text("%.2f",
								this->mesh->verticesData[i].color.y());
						ImGui::TableNextColumn();
						ImGui::Text("%.2f",
								this->mesh->verticesData[i].color.z());
					}
				}
			}
			ImGui::EndTable();
			if (this->mesh->nbVertices > MAX_CONTENT)
				ImGui::Text("(truncated to first %u vertices)", MAX_CONTENT);

			ImGui::Text("  Faces:");
			if (ImGui::BeginTable("Faces",
					(this->mesh->HaveMaterials() ? 5 : 4), this->tableFlags)) {
				ImGui::TableNextColumn();
				ImGui::TableNextColumn();
				ImGui::TableHeader("v0");
				ImGui::TableNextColumn();
				ImGui::TableHeader("v1");
				ImGui::TableNextColumn();
				ImGui::TableHeader("v2");
				if (this->mesh->HaveMaterials()) {
					ImGui::TableNextColumn();
					ImGui::TableHeader("mat");
				}

				unsigned int max = MAX_CONTENT;
				if (this->mesh->nbFaces < max)
					max = this->mesh->nbFaces;
				for (unsigned int i = 0; i < max; i++) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%u", i);
					ImGui::TableNextColumn();
					ImGui::Text("%u",
							this->mesh->facesVertices[3 * i]);
					ImGui::TableNextColumn();
					ImGui::Text("%u",
							this->mesh->facesVertices[3 * i + 1]);
					ImGui::TableNextColumn();
					ImGui::Text("%u",
							this->mesh->facesVertices[3 * i + 2]);
					if (this->mesh->HaveMaterials()) {
						ImGui::TableNextColumn();
						ImGui::Text("%u",
								this->mesh->facesMaterials[i]);
					}
				}
			}
			ImGui::EndTable();
			if (this->mesh->nbFaces > MAX_CONTENT)
				ImGui::Text("(truncated to first %u faces)", MAX_CONTENT);

			ImGui::Text("Normals:");
			ImGui::Text("  Vertices' normals:");
			if (ImGui::BeginTable("Vertices' normals", 4, this->tableFlags)) {
				ImGui::TableNextColumn();
				ImGui::TableNextColumn();
				ImGui::TableHeader("x");
				ImGui::TableNextColumn();
				ImGui::TableHeader("y");
				ImGui::TableNextColumn();
				ImGui::TableHeader("z");

				unsigned int max = MAX_CONTENT;
				if (this->mesh->nbVertices < max)
					max = this->mesh->nbVertices;
				for (unsigned int i = 0; i < max; i++) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%u", i);
					ImGui::TableNextColumn();
					ImGui::Text("%.2f",
							this->mesh->verticesData[i].normal.x());
					ImGui::TableNextColumn();
					ImGui::Text("%.2f",
							this->mesh->verticesData[i].normal.y());
					ImGui::TableNextColumn();
					ImGui::Text("%.2f",
							this->mesh->verticesData[i].normal.z());
				}
			}
			ImGui::EndTable();
			if (this->mesh->nbVertices > MAX_CONTENT)
				ImGui::Text("(truncated to first %u faces)", MAX_CONTENT);
		}
		ImGui::End();
	}
}
