#include "modules/meshcontent.h"

#define MAX_CONTENT		15

MeshContentModule::MeshContentModule(void* context, std::string filename,
		Mesh* mesh)
		: GUIModule(context)
		, mesh(mesh) {
	this->title = "Mesh content";
	this->filename = filename;

	this->Init();
}

MeshContentModule::MeshContentModule(MeshContentModule* module)
		: GUIModule(module->GetContext())
		, mesh(module->GetMesh()) {
	this->title = module->GetTitle();
	this->filename = module->GetFilename();

	this->Init();
}

MeshContentModule::~MeshContentModule() {}

void MeshContentModule::Init() {
	this->tableFlags = ImGuiTableFlags_Borders;
}

void MeshContentModule::Render() {
	if (this->mesh != nullptr) {
		if (ImGui::Begin(std::string(this->title + "###"
				+ std::to_string(this->id)).c_str())) {
			ImGui::Text("Informations:");
			ImGui::Text("  Filename: %s", this->filename.c_str());
			ImGui::Text("  Nb vertices: %u",
					this->mesh->nbVertices);
			ImGui::Text("  Nb faces: %u",
					this->mesh->nbFaces);
			ImGui::Text("  Have colors: %s",
					(this->mesh->HaveColors() ? "yes" : "no"));
			ImGui::Text("  Have materials: %s",
					(this->mesh->HaveMaterials() ? "yes" : "no"));
			ImGui::Separator();

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
			ImGui::Separator();

			ImGui::Text("Content:");
			if (ImGui::CollapsingHeader("Vertices data")) {
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
				if (this->mesh->nbVertices > MAX_CONTENT) {
					ImGui::Text("(truncated to first %u vertices)",
							MAX_CONTENT);
				}
			}

			if (ImGui::CollapsingHeader("Faces data")) {
				if (ImGui::BeginTable("Faces",
						(this->mesh->HaveMaterials() ? 5 : 4),
						this->tableFlags)) {
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
			}

			ImGui::Separator();

			ImGui::Text("Normals:");
			if (ImGui::CollapsingHeader("Vertices' normals")) {
				if (ImGui::BeginTable("Vertices' normals", 4,
						this->tableFlags)) {
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

			if (this->mesh->HaveMaterials()) {
				ImGui::Separator();

				ImGui::Text("Materials:");
				if (ImGui::CollapsingHeader("Number of faces per material")) {
					if (ImGui::BeginTable("Number of faces per material", 2,
							this->tableFlags)) {
						ImGui::TableNextColumn();
						ImGui::TableHeader("mat");
						ImGui::TableNextColumn();
						ImGui::TableHeader("nb faces");

						unsigned char currentMaterial =
								this->mesh->GetMaterialsRange().min()[0];
						for (unsigned char i = 0; i < this->mesh->nbMaterials; i++) {
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("%u", currentMaterial++);
							ImGui::TableNextColumn();
							ImGui::Text("%u", this->mesh->nbFacesPerMaterial[i]);
						}
					}
					ImGui::EndTable();
				}
			}
		}
		ImGui::End();
	}
}

Mesh* MeshContentModule::GetMesh() {
	return this->mesh;
}

const std::string& MeshContentModule::GetFilename() {
	return this->filename;
}
