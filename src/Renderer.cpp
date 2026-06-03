#include "Renderer.hpp"

#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Instances/BasePart.hpp"
#include "Instances/Workspace.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Filesystem.hpp"
#include "CubeData.hpp"
#include "Signal.hpp"
#include "Texture.hpp"
#include "skyboxCross.hpp"
#include "types/Vector.hpp"
#include "types/Vector3.hpp"
#include "Signal.hpp"
#include "MemorySystem.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

static glm::vec3 GetSunDirection(float clockTime, float geographicLatitude) {
	float timeAngle = (clockTime / 24.0f) * 2.0f * glm::pi<float>();

	float latRad = glm::radians(geographicLatitude);

	float x = -glm::sin(timeAngle);
	float y = glm::cos(timeAngle);
	float z = 0.0f;

	glm::vec3 sunDirection;
	sunDirection.x = x;
	sunDirection.y = (y * glm::cos(latRad)) - (z * glm::sin(latRad));
	sunDirection.z = (y * glm::sin(latRad)) + (z * glm::cos(latRad));

	return glm::normalize(sunDirection);
}

void ResizeInstanceVBO(GLuint& vboID, size_t newCapacity, size_t elementSize, const void* currentData, size_t currentSize) {
	GLuint newVBO;
	glGenBuffers(1, &newVBO);
	glBindBuffer(GL_ARRAY_BUFFER, newVBO);
	glBufferData(GL_ARRAY_BUFFER, newCapacity * elementSize, nullptr, GL_DYNAMIC_DRAW);

	if (currentData && currentSize > 0) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, currentSize * elementSize, currentData);
	}

	glDeleteBuffers(1, &vboID);
	vboID = newVBO;
}

// events

void OnCFrameChanged(void* instance, void* payload);
void OnSizeChanged(void* instance, void* payload);
void OnColorChanged(void* instance, void* payload);
void OnDescendentAdded(void* instance, void* payload);
void OnDescendentRemoving(void* instance, void* payload);



namespace Engine {
namespace Renderer {
	// properties
	namespace {
		Shader g_shader;
		Shader g_skyboxShader;

		Mesh g_cubeMesh;
		Mesh g_skyboxMesh;
		Texture g_skyboxTexture;
		
		glm::mat4 g_cachedView = glm::mat4(1.0f);
		glm::mat4 g_cachedProjection = glm::mat4(1.0f);
		glm::vec3 g_cachedViewPos = glm::vec3(1.0f);
		glm::vec3 sunDir = GetSunDirection(14.0f, 41.733f);

		FlatMap<BasePart*, size_t> rendererparts;
		FlatMap<size_t, BasePart*> partsByIndex;
		Vector<glm::mat4> modelMatricesVBO;
		Vector<glm::vec3> colorsVBO;

		GLuint modelMatrixVBO_ID = 0;
		GLuint colorsVBO_ID = 0;
		size_t vboCapacity = 0;

		bool g_isInitalized = false;
	}

	// events
	namespace {
		void OnCFrameChanged(void* instance, void* payload);
		void OnSizeChanged(void* instance, void* payload);
		void OnColorChanged(void* instance, void* payload);
		void OnDescendentAdded(void* instance, void* payload);
		void OnDescendentRemoving(void* instance, void* payload);
	}

	void Initialize(Workspace* workspace) {
		const char* vertexSource = Engine::fs::readFile("../shaders/shader.vs");
		const char* fragmentSource = Engine::fs::readFile("../shaders/shader.fs");
		if (!vertexSource || !fragmentSource) {
			fprintf(stderr, "failed to load shaders\n");
			glfwTerminate();
			return;
		}
		
		g_shader.Initalize(vertexSource, fragmentSource);
		free((void*)vertexSource);
		free((void*)fragmentSource);

		const char* skyboxVSSource = Engine::fs::readFile("../shaders/skybox.vs");
		const char* skyboxFSSource = Engine::fs::readFile("../shaders/skybox.fs");
		g_skyboxShader.Initalize(skyboxVSSource, skyboxFSSource);
		free((void*)skyboxVSSource); 
		free((void*)skyboxFSSource);

		// initalize rendering in workspace data
		auto& workspaceChildren = workspace->GetChildren(); // FlatMap

		size_t currentIndex = 0;
		auto it = workspaceChildren.GetIterator();
		while (it.step()) {
			if (it.value.value->IsA(BasePart::GetClassIdStatic())) {
				BasePart* processingPart = static_cast<BasePart*>(it.value.value);
				Vector3 partSize = processingPart->GetSize();
				glm::mat4 model = glm::scale(processingPart->GetCFrame().matrix, glm::vec3(partSize.x, partSize.y, partSize.z));

				modelMatricesVBO.PushBack(model);
				Color3 partColor = processingPart->GetColor3();
				colorsVBO.PushBack(glm::vec3(partColor.R, partColor.G, partColor.B));

				processingPart->Internal_GetPropertyChangedSignal("Color")->Connect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnColorChanged);
				processingPart->Internal_GetPropertyChangedSignal("CFrame")->Connect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnCFrameChanged);
				processingPart->Internal_GetPropertyChangedSignal("Size")->Connect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnSizeChanged);

				rendererparts.Insert(processingPart, currentIndex);
				partsByIndex.Insert(currentIndex, processingPart);
				currentIndex++;
			}
		}

		vboCapacity = modelMatricesVBO.Size();
		// initalize mesh for a block

		g_cubeMesh.Initalize(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), GL_STATIC_DRAW);

		GLuint cubeMeshVAO = g_cubeMesh.GetVAO();
		GLuint cubeMeshVBO = g_cubeMesh.GetVBO();
		GLuint cubeMeshEBO = g_cubeMesh.GetEBO(); 

		glBindVertexArray(cubeMeshVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeMeshVBO);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMeshEBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// initalize instance vbos
		glGenBuffers(1, &modelMatrixVBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMatricesVBO.Size(), modelMatricesVBO.Data(), GL_DYNAMIC_DRAW);
		GLuint matrixStartLocation = 2;

		for (unsigned int i = 0; i < 4; i++) {
			unsigned int attributeLocation = matrixStartLocation + i;
			
			glEnableVertexAttribArray(attributeLocation);
			glVertexAttribPointer(
				attributeLocation, 
				4,                             // Each row is a vec4 (4 floats)
				GL_FLOAT, 
				GL_FALSE, 
				sizeof(glm::mat4),                      // Stride: Jump 64 bytes to reach the next matrix
				(void*)(i * sizeof(glm::vec4))          // Offset: Progressively step 16 bytes forward per row
			);
			
			glVertexAttribDivisor(attributeLocation, 1); 
		}

		glGenBuffers(1, &colorsVBO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, colorsVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colorsVBO.Size(), colorsVBO.Data(), GL_DYNAMIC_DRAW);

		unsigned int colorLocation = 6; 
		glEnableVertexAttribArray(colorLocation);
		glVertexAttribPointer(
			colorLocation, 
			3,                                 // vec3 (R, G, B)
			GL_FLOAT, 
			GL_FALSE, 
			sizeof(glm::vec3),                 // Stride: size of one color element
			(void*)0
		);

		glVertexAttribDivisor(colorLocation, 1);

		// initalize skybox mesh

		g_skyboxMesh.Initalize(skyboxCrossVertices, sizeof(skyboxCrossVertices), skyboxCrossIndices, sizeof(skyboxCrossIndices), GL_STATIC_DRAW);

		unsigned int skyboxVAO = g_skyboxMesh.GetVAO();
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, g_skyboxMesh.GetVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_skyboxMesh.GetEBO());

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

		g_skyboxTexture.Initialize("../assets/skybox_cross.png", true);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		g_isInitalized = true;
	}
	
	void Shutdown() {
		if (!g_isInitalized) return;
		g_cubeMesh.Destroy();
		g_skyboxMesh.Destroy();
		g_skyboxTexture.Destroy();
		g_isInitalized = false;
	}

	void BeginFrame() {
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Render() {
		// --- PASS 1: RENDER SKYBOX BACKGROUND ---
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);  
		glDisable(GL_CULL_FACE); 
		
		g_skyboxShader.use();
		
		glm::mat4 skyboxView = glm::mat4(glm::mat3(g_cachedView));
		
		g_skyboxShader.setMat4("view", skyboxView);
		g_skyboxShader.setMat4("projection", g_cachedProjection);
		
		g_skyboxTexture.Bind(0);
		g_skyboxShader.SetInt("u_SkyboxTexture", 0);

		glBindVertexArray(g_skyboxMesh.GetVAO());
		glDrawElements(GL_TRIANGLES, g_skyboxMesh.GetIndexCount(), GL_UNSIGNED_INT, 0);

		// --- PASS 2: RENDER SCENE OBJECTS ---
		glDepthFunc(GL_LESS); 
		glEnable(GL_CULL_FACE);
		
		g_shader.use();

		g_shader.setMat4("view", g_cachedView);
		g_shader.setMat4("projection", g_cachedProjection);
		g_shader.SetVec3("sunDir", sunDir);
		g_shader.SetVec3("viewPos", g_cachedViewPos);

		glBindVertexArray(g_cubeMesh.GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, g_cubeMesh.GetIndexCount(), GL_UNSIGNED_INT, 0, modelMatricesVBO.Size());
		
		glBindVertexArray(0);
	}

	
	void SetCamera(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos) {
		g_cachedView = view;
		g_cachedProjection = projection;
		g_cachedViewPos = viewPos;
	}

	// event function definitions
	namespace {
		void OnCFrameChanged(void* instance, void* payload) {
			auto* part = static_cast<BasePart*>(payload);
			size_t* partIndex = rendererparts.Find(part);
			if (!partIndex) return;

			size_t index = *partIndex;
			Vector3 partSize = part->GetSize();
			glm::mat4 model = glm::scale(part->GetCFrame().matrix, glm::vec3(partSize.x, partSize.y, partSize.z));
			
			modelMatricesVBO[index] = model;

			glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO_ID);
			glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &modelMatricesVBO[index]);
		}

		void OnSizeChanged(void* instance, void* payload) {
			OnCFrameChanged(instance, payload);
		}

		void OnColorChanged(void* instance, void* payload) {
			auto* part = static_cast<BasePart*>(payload);
			size_t* partIndex = rendererparts.Find(part);
			if (!partIndex) return;

			size_t index = *partIndex;
			Color3 partColor = part->GetColor3();
			colorsVBO[index] = glm::vec3(partColor.R, partColor.G, partColor.B);

			glBindBuffer(GL_ARRAY_BUFFER, colorsVBO_ID);
			glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &colorsVBO[index]);
		}

		void OnDescendentAdded(void* instance, void* payload) {
			auto* part = static_cast<BasePart*>(payload);
			if (!part->IsA(BasePart::GetClassIdStatic())) return;

			size_t index = modelMatricesVBO.Size();

			Vector3 partSize = part->GetSize();
			glm::mat4 model = glm::scale(part->GetCFrame().matrix, glm::vec3(partSize.x, partSize.y, partSize.z));
			Color3 partColor = part->GetColor3();

			modelMatricesVBO.PushBack(model);
			colorsVBO.PushBack(glm::vec3(partColor.R, partColor.G, partColor.B));

			if (modelMatricesVBO.Size() > vboCapacity) {
				size_t newCapacity = vboCapacity == 0 ? 16 : vboCapacity * 2;
				ResizeInstanceVBO(modelMatrixVBO_ID, newCapacity, sizeof(glm::mat4), modelMatricesVBO.Data(), index);
				ResizeInstanceVBO(colorsVBO_ID, newCapacity, sizeof(glm::vec3), colorsVBO.Data(), index);
				vboCapacity = newCapacity;
			}

			glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO_ID);
			glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &modelMatricesVBO[index]);

			glBindBuffer(GL_ARRAY_BUFFER, colorsVBO_ID);
			glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &colorsVBO[index]);

			part->Internal_GetPropertyChangedSignal("Color")->Connect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnColorChanged);
			part->Internal_GetPropertyChangedSignal("CFrame")->Connect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnCFrameChanged);
			part->Internal_GetPropertyChangedSignal("Size")->Connect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnSizeChanged);

			rendererparts.Insert(part, index);
			partsByIndex.Insert(index, part);
		}

		void OnDescendentRemoving(void* instance, void* payload) {
			auto* part = static_cast<BasePart*>(payload);
			size_t* partIndex = rendererparts.Find(part);
			if (!partIndex) return;

			size_t removeIndex = *partIndex;

			part->Internal_GetPropertyChangedSignal("Color")->Disconnect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnColorChanged);
			part->Internal_GetPropertyChangedSignal("CFrame")->Disconnect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnCFrameChanged);
			part->Internal_GetPropertyChangedSignal("Size")->Disconnect(Memory::GetSignalAllocator(), nullptr, &Renderer::OnSizeChanged);

			size_t lastIndex = modelMatricesVBO.Size() - 1;
			if (removeIndex != lastIndex) {
				BasePart** lastPartPtr = partsByIndex.Find(lastIndex);
				if (lastPartPtr) {
					BasePart* lastPart = *lastPartPtr;

					modelMatricesVBO[removeIndex] = modelMatricesVBO[lastIndex];
					colorsVBO[removeIndex] = colorsVBO[lastIndex];

					glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO_ID);
					glBufferSubData(GL_ARRAY_BUFFER, removeIndex * sizeof(glm::mat4), sizeof(glm::mat4), &modelMatricesVBO[removeIndex]);

					glBindBuffer(GL_ARRAY_BUFFER, colorsVBO_ID);
					glBufferSubData(GL_ARRAY_BUFFER, removeIndex * sizeof(glm::vec3), sizeof(glm::vec3), &colorsVBO[removeIndex]);

					rendererparts.Insert(lastPart, removeIndex);
					partsByIndex.Insert(removeIndex, lastPart);
				}
			}

			modelMatricesVBO.Pop();
			colorsVBO.Pop();

			rendererparts.Erase(part);
			partsByIndex.Erase(lastIndex);
		}
	}

} // namespace Renderer
} // namespace Engine

