#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "GBuffer.h"
#include <GL/glew.h>
#include "PointLightComponent.h"
#include "SkeletalMeshComponent.h"

Renderer::Renderer(Game* gameInst)
	:game(gameInst),
	spriteShader(nullptr),
	meshShader(nullptr),
	skinnedShader(nullptr),
	mirrorBuffer(0),
	depthMapFBO(0),
	mirrorTexture(nullptr),
	_GBuffer(nullptr),
	GGlobalShader(nullptr),
	GPointLightShader(nullptr)
{
}

Renderer::~Renderer(){
}

bool Renderer::Initialize(float newScreenWidth, float newScreenHeight){
	screenWidth = newScreenWidth;
	screenHeight = newScreenHeight;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window = SDL_CreateWindow("Bones Engine", 100, 100,
		static_cast<int>(screenWidth), static_cast<int>(screenHeight), SDL_WINDOW_OPENGL);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	context = SDL_GL_CreateContext(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	glGetError();

	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}
	CreateSpriteVerts();

	if (!CreateMirrorTarget()) {
		SDL_Log("Failed to create render target for mirror.");
		return false;
	}

	if (!CreateShadowMapTarget()) {
		SDL_Log("Failed to create render target for shadow map.");
		return false;
	}

	_GBuffer = new GBuffer();
	int width = static_cast<int>(screenWidth);
	int height = static_cast<int>(screenHeight);
	if (!_GBuffer->Create(width, height)) {
		SDL_Log("Failed to create G-buffer.");
		return false;
	}

	return true;
}

void Renderer::Shutdown(){
	delete spriteVerts;
	spriteShader->Unload();
	delete spriteShader;
	meshShader->Unload();
	delete meshShader;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void Renderer::UnloadData(){
	for (auto i : textures)
	{
		i.second->Unload();
		delete i.second;
	}
	textures.clear();

	for (auto i : meshes)
	{
		i.second->Unload();
	}
	meshes.clear();
}

void Renderer::Draw(){
	// Draw the 3D scene to the G-buffer
	Draw3DScene(_GBuffer->GetBufferID(), view, projection, 1.0f, false);
	// Set the frame buffer back to zero (screen's frame buffer)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw from the GBuffer
	DrawFromGBuffer();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	spriteShader->SetActive();
	spriteVerts->SetActive();

	vector<SpriteComponent*>::iterator itr;
	for (itr = sprites.begin(); itr < sprites.end(); itr++)
	{
		if ((*itr)->GetVisible())
		{
			(*itr)->Draw(spriteShader);
		}
	}

	SDL_GL_SwapWindow(window);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	vector<SpriteComponent*>::iterator itr;
	for (itr = sprites.begin(); itr < sprites.end(); itr++)
	{
		if (myDrawOrder < (*itr)->GetDrawOrder()){
			break;
		}
	}
	sprites.insert(itr, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite){
	vector<SpriteComponent*>::iterator itr;
	for (itr = sprites.begin(); itr < sprites.end(); itr++)
	{
		if ((*itr) == sprite)
		{
			sprites.erase(itr);
			break;
		}
	}
}

void Renderer::AddMeshComp(MeshComponent* mesh){
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		skeletalMeshes.emplace_back(sk);
	} 
	else 
	{
		meshComps.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComp(MeshComponent* mesh){
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(skeletalMeshes.begin(), skeletalMeshes.end(), sk);
		skeletalMeshes.erase(iter);
	}
	else
	{
		auto iter = std::find(meshComps.begin(), meshComps.end(), mesh);
		meshComps.erase(iter);
	}
}

void Renderer::AddPointLight(PointLightComponent* light) {
	pointLights.emplace_back(light);
}

void Renderer::RemovePointLight(PointLightComponent* light) {
	auto iter = std::find(pointLights.begin(), pointLights.end(), light);
	pointLights.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName){
	Texture* tex = nullptr;
	auto iter = textures.find(fileName);

	if (iter != textures.end()) {
		tex = iter->second;
	}else {
		tex = new Texture();
		if (tex->Load(fileName)) {
			textures.emplace(fileName, tex);
		}else{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string & fileName){
	Mesh* m = nullptr;
	auto iter = meshes.find(fileName);

	if (iter != meshes.end()){
		m = iter->second;
	}else{
		m = new Mesh();
		if (m->Load(fileName, this)){
			meshes.emplace(fileName, m);
		}else{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

void Renderer::DrawDepthMap(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, float viewPortScale, bool lit) {
	glViewport(0, 0, static_cast<int>(screenWidth * viewPortScale), static_cast<int>(screenHeight * viewPortScale));
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

}

void Renderer::Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj,
	float viewPortScale, bool lit) {
	// Set the current frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Set viewport size based on scale
	glViewport(0, 0,
		static_cast<int>(screenWidth * viewPortScale),
		static_cast<int>(screenHeight * viewPortScale)
	);

	// Clear color buffer/depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw mesh components
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// Set the mesh shader active
	meshShader->SetActive();
	// Update view-projection matrix
	meshShader->SetMatrixUniform("uViewProj", view * proj);
	// Update lighting uniforms
	if (lit) {
		SetLightUniforms(meshShader, view);
	}

	vector<MeshComponent*>::iterator itr;
	for (itr = meshComps.begin(); itr < meshComps.end(); itr++)
	{
		if ((*itr)->GetVisible())
		{
			(*itr)->Draw(meshShader);
		}
	}

	// Draw any skinned meshes now
	skinnedShader->SetActive();
	// Update view-projection matrix
	skinnedShader->SetMatrixUniform("uViewProj", view * proj);
	// Update lighting uniforms
	if (lit) {
		SetLightUniforms(skinnedShader, view);
	}

	vector<SkeletalMeshComponent*>::iterator iter;
	for (iter = skeletalMeshes.begin(); iter < skeletalMeshes.end(); iter++)
	{
		if ((*iter)->GetVisible())
		{
			(*iter)->Draw(skinnedShader);
		}
	}
}

bool Renderer::CreateMirrorTarget() {
	int width = static_cast<int>(screenWidth) / 4;
	int height = static_cast<int>(screenHeight) / 4;

	// Generate a frame buffer for the mirror texture
	glGenFramebuffers(1, &mirrorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mirrorBuffer);

	// Create the texture we'll use for rendering
	mirrorTexture = new Texture();
	mirrorTexture->CreateForRendering(width, height, GL_RGB);

	// Add a depth buffer to this target
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Attach mirror texture as the output target for the frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mirrorTexture->GetTextureID(), 0);

	// Set the list of buffers to draw to for this frame buffer
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	// Make sure everything worked
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		// If it didn't work, delete the framebuffer,
		// unload/delete the texture and return false
		glDeleteFramebuffers(1, &mirrorBuffer);
		mirrorTexture->Unload();
		delete mirrorTexture;
		mirrorTexture = nullptr;
		return false;
	}
	return true;
}

bool Renderer::CreateShadowMapTarget() {
	glGenFramebuffers(1, &depthMapFBO);

	depthMapTexture = new Texture();
	depthMapTexture->CreateForRenderingShadowMap(1024, 1024, GL_DEPTH_COMPONENT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture->GetTextureID(), 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glDeleteFramebuffers(1, &depthMapFBO);
		depthMapTexture->Unload();
		delete depthMapTexture;
		depthMapTexture = nullptr;
		return false;
	}
	return true;
}

void Renderer::DrawFromGBuffer()
{
	// Clear the current framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Disable depth testing for the global lighting pass
	glDisable(GL_DEPTH_TEST);
	// Activate global G-buffer shader
	GGlobalShader->SetActive();
	// Activate sprite verts quad
	spriteVerts->SetActive();
	// Set the G-buffer textures to sample
	_GBuffer->SetTexturesActive();
	// Set the lighting uniforms
	SetLightUniforms(GGlobalShader, view);
	// Draw the triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Copy depth buffer from G-buffer to default frame buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _GBuffer->GetBufferID());
	int width = static_cast<int>(screenWidth);
	int height = static_cast<int>(screenHeight);
	glBlitFramebuffer(0, 0, width, height,
		0, 0, width, height,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// Enable depth test, but disable writes to depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// Set the point light shader and mesh as active
	GPointLightShader->SetActive();
	pointLightMesh = GetMesh("Assets/PointLight.obj");
	pointLightMesh->GetVertexArray()->SetActive();
	// Set the view-projeciton matrix
	GPointLightShader->SetMatrixUniform("uViewProj",
		view * projection);
	// Set the G-buffer textures for sampling
	_GBuffer->SetTexturesActive();

	// The point light color should add to existing color
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Draw the point lights
	vector<PointLightComponent*>::iterator iter;
	for (iter = pointLights.begin(); iter < pointLights.end(); iter++)
	{
		(*iter)->Draw(GPointLightShader, pointLightMesh);
	}
}

bool Renderer::LoadShaders() {
	//Sprite Shader
	spriteShader = new Shader();

	if (!spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
		return false;
	}

	spriteShader->SetActive();
	Matrix4 spriteViewProj = Matrix4::CreateSimpleViewProj(screenWidth, screenHeight);
	spriteShader->SetMatrixUniform("uViewProj", spriteViewProj);

	//Mesh Shader
	meshShader = new Shader();

	if (!meshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag")) {
		return false;
	}

	meshShader->SetActive();
	view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), screenWidth, screenHeight, 10.0f, 10000.0f);
	meshShader->SetMatrixUniform("uViewProj", view * projection);

	// Create skinned shader
	skinnedShader = new Shader();
	if (!skinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}

	skinnedShader->SetActive();
	skinnedShader->SetMatrixUniform("uViewProj", view * projection);
	
	// Create shader for drawing from GBuffer (global lighting)
	GGlobalShader = new Shader();
	if (!GGlobalShader->Load("Shaders/GBufferGlobal.vert", "Shaders/GBufferGlobal.frag")) {
		return false;
	}
	// For the GBuffer, we need to associate each sampler with an index
	GGlobalShader->SetActive();
	GGlobalShader->SetIntUniform("uGDiffuse", 0);
	GGlobalShader->SetIntUniform("uGNormal", 1);
	GGlobalShader->SetIntUniform("uGWorldPos", 2);
	// The view projection is just the sprite one
	GGlobalShader->SetMatrixUniform("uViewProj", spriteViewProj);
	// The world transform scales to the screen and flips y
	Matrix4 gbufferWorld = Matrix4::CreateScale(screenWidth, -screenHeight,
		1.0f);
	GGlobalShader->SetMatrixUniform("uWorldTransform", gbufferWorld);

	// Create a shader for point lights from GBuffer
	GPointLightShader = new Shader();
	if (!GPointLightShader->Load("Shaders/BasicMesh.vert",
		"Shaders/GBufferPointLight.frag"))
	{
		return false;
	}
	// Set sampler indices
	GPointLightShader->SetActive();
	GPointLightShader->SetIntUniform("uGDiffuse", 0);
	GPointLightShader->SetIntUniform("uGNormal", 1);
	GPointLightShader->SetIntUniform("uGWorldPos", 2);
	GPointLightShader->SetVector2Uniform("uScreenDimensions",
		Vector2(screenWidth, screenHeight));

	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	spriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader, const Matrix4& view){
	Matrix4 invView = view;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient light
	shader->SetVectorUniform("uAmbientLight", ambientLight);
	// Directional light
	shader->SetVectorUniform("uDirLight.mDirection", dirLight.direction);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", dirLight.diffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", dirLight.specColor);
}
