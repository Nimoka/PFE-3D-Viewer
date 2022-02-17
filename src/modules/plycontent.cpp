#include "modules/plycontent.h"

#define MAX_CONTENT		20

PLYContentModule::PLYContentModule(Context* context, std::string name, Mesh* mesh)
: GUIModule(context) {
	this->mesh = mesh;
	this->title = name;

	this->Init();
}

PLYContentModule::PLYContentModule(PLYContentModule* module)
: GUIModule(module->GetContext()) {
	this->title = module->GetTitle();
	this->mesh = module->GetMesh();

	this->Init();
}

PLYContentModule::~PLYContentModule() {}

Mesh* PLYContentModule::GetMesh() {
	return this->mesh;
}

void PLYContentModule::Init() {
	this->tableFlags = ImGuiTableFlags_Borders;

	/* Compute X position range */
	this->minPosX = this->mesh->verticesPosition[0];
	this->maxPosX = this->mesh->verticesPosition[0];
	for (int i = 1; i < this->mesh->nbVertices; i++) {
		if (this->minPosX > this->mesh->verticesPosition[3 * i])
			this->minPosX = this->mesh->verticesPosition[3 * i];
		if (this->maxPosX < this->mesh->verticesPosition[3 * i])
			this->maxPosX = this->mesh->verticesPosition[3 * i];
	}

	/* Compute Y position range */
	this->minPosY = this->mesh->verticesPosition[1];
	this->maxPosY = this->mesh->verticesPosition[1];
	for (int i = 1; i < this->mesh->nbVertices; i++) {
		if (this->minPosY > this->mesh->verticesPosition[3 * i + 1])
			this->minPosY = this->mesh->verticesPosition[3 * i + 1];
		if (this->maxPosY < this->mesh->verticesPosition[3 * i + 1])
			this->maxPosY = this->mesh->verticesPosition[3 * i + 1];
	}

	/* Compute Z position range */
	this->minPosZ = this->mesh->verticesPosition[2];
	this->maxPosZ = this->mesh->verticesPosition[2];
	for (int i = 1; i < this->mesh->nbVertices; i++) {
		if (this->minPosZ > this->mesh->verticesPosition[3 * i + 2])
			this->minPosZ = this->mesh->verticesPosition[3 * i + 2];
		if (this->maxPosZ < this->mesh->verticesPosition[3 * i + 2])
			this->maxPosZ = this->mesh->verticesPosition[3 * i + 2];
	}

	if (this->mesh->haveColors) {
		/* Compute R intensity range */
		this->minColorR = this->mesh->verticesColor[0];
		this->maxColorR = this->mesh->verticesColor[0];
		for (int i = 1; i < this->mesh->nbVertices; i++) {
			if (this->minColorR > this->mesh->verticesColor[3 * i])
				this->minColorR = this->mesh->verticesColor[3 * i];
			if (this->maxColorR < this->mesh->verticesColor[3 * i])
				this->maxColorR = this->mesh->verticesColor[3 * i];
		}

		/* Compute G intensity range */
		this->minColorG = this->mesh->verticesColor[1];
		this->maxColorG = this->mesh->verticesColor[1];
		for (int i = 1; i < this->mesh->nbVertices; i++) {
			if (this->minColorG > this->mesh->verticesColor[3 * i + 1])
				this->minColorG = this->mesh->verticesColor[3 * i + 1];
			if (this->maxColorG < this->mesh->verticesColor[3 * i + 1])
				this->maxColorG = this->mesh->verticesColor[3 * i + 1];
		}

		/* Compute B intensity range */
		this->minColorB = this->mesh->verticesColor[2];
		this->maxColorB = this->mesh->verticesColor[2];
		for (int i = 1; i < this->mesh->nbVertices; i++) {
			if (this->minColorB > this->mesh->verticesColor[3 * i + 2])
				this->minColorB = this->mesh->verticesColor[3 * i + 2];
			if (this->maxColorB < this->mesh->verticesColor[3 * i + 2])
				this->maxColorB = this->mesh->verticesColor[3 * i + 2];
		}
	}

	if (this->mesh->haveMaterials) {
		/* Compute materials range */
		this->minMatID = this->mesh->facesMaterial[0];
		this->maxMatID = this->mesh->facesMaterial[0];
		for (int i = 1; i < this->mesh->nbFaces; i++) {
			if (this->minMatID > this->mesh->facesMaterial[i])
				this->minMatID = this->mesh->facesMaterial[i];
			if (this->maxMatID < this->mesh->facesMaterial[i])
				this->maxMatID = this->mesh->facesMaterial[i];
		}
	}

	this->nbUnusedVertices = this->mesh->ContainsUnusedVertices();
}

void PLYContentModule::Render() {
	if (this->mesh != nullptr) {
		if (ImGui::Begin(std::string(this->title + "###" + std::to_string(this->id)).c_str())) {
			ImGui::Text("Informations:");
			ImGui::Text("  Nb vertices: %u", this->mesh->nbVertices);
			if (this->nbUnusedVertices)
				ImGui::Text("    (%u unused)", this->nbUnusedVertices);
			ImGui::Text("  Nb faces: %u", this->mesh->nbFaces);
			ImGui::Text("  Have colors: %s", (this->mesh->haveColors ? "yes" : "no"));
			ImGui::Text("  Have materials: %s", (this->mesh->haveMaterials ? "yes" : "no"));
			ImGui::Text("");

			ImGui::Text("Values range:");
			ImGui::Text("  x: [ %.2f, %.2f ]", this->minPosX, this->maxPosX);
			ImGui::Text("  y: [ %.2f, %.2f ]", this->minPosY, this->maxPosY);
			ImGui::Text("  z: [ %.2f, %.2f ]", this->minPosZ, this->maxPosZ);
			if (this->mesh->haveColors) {
				ImGui::Text("  r: [ %.2f, %.2f ]", this->minColorR, this->maxColorR);
				ImGui::Text("  g: [ %.2f, %.2f ]", this->minColorG, this->maxColorG);
				ImGui::Text("  b: [ %.2f, %.2f ]", this->minColorB, this->maxColorB);
			}
			if (this->mesh->haveMaterials)
				ImGui::Text("  mat ID: [ %u, %u ]", this->minMatID, this->maxMatID);
			ImGui::Text("");

			ImGui::Text("Content:");
			ImGui::Text("  Vertices:");
			if (ImGui::BeginTable("Vertices", (this->mesh->haveColors ? 7 : 4), this->tableFlags)) {
				ImGui::TableNextColumn();
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

				int max = MAX_CONTENT;
				if (this->mesh->nbVertices < max)
					max = this->mesh->nbVertices;
				for (int i = 0; i < max; i++) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%u", i);
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
			if (this->mesh->nbVertices > MAX_CONTENT)
				ImGui::Text("(truncated to first %u vertices)", MAX_CONTENT);

			ImGui::Text("  Faces:");
			if (ImGui::BeginTable("Faces",
					(this->mesh->haveMaterials ? 5 : 4), this->tableFlags)) {
				ImGui::TableNextColumn();
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

				int max = MAX_CONTENT;
				if (this->mesh->nbFaces < max)
					max = this->mesh->nbFaces;
				for (int i = 0; i < max; i++) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("%u", i);
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
			if (this->mesh->nbFaces > MAX_CONTENT)
				ImGui::Text("(truncated to first %u faces)", MAX_CONTENT);

			ImGui::Text("Normals:");
			ImGui::Text("  Vertices' normals:");
			if (this->mesh->verticesNormals == nullptr) {
				if (ImGui::Button("Compute"))
					this->mesh->ComputeNormals();
			} else {
				if (ImGui::BeginTable("Vertices' normals", 4, this->tableFlags)) {
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableHeader("x");
					ImGui::TableNextColumn();
					ImGui::TableHeader("y");
					ImGui::TableNextColumn();
					ImGui::TableHeader("z");

					int max = MAX_CONTENT;
					if (this->mesh->nbVertices < max)
						max = this->mesh->nbVertices;
					for (int i = 0; i < max; i++) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("%u", i);
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", this->mesh->verticesNormals[3 * i]);
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", this->mesh->verticesNormals[3 * i + 1]);
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", this->mesh->verticesNormals[3 * i + 2]);
					}
				}
				ImGui::EndTable();
				if (this->mesh->nbVertices > MAX_CONTENT)
					ImGui::Text("(truncated to first %u faces)", MAX_CONTENT);
			}
		}
		ImGui::End();
	}
}
