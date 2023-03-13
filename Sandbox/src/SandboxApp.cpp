#include <Nut.h>
#include <Nut/Core/Entrypoint.h>

#include "Sandbox2D.h"

class ExampleLayer : public Nut::Layer {
public:
	ExampleLayer() : Layer("Example")
	{	
	}

	void OnUpdate(Nut::Timestep ts) override 
	{
	}

	void OnEvent(Nut::Event& e) override 
	{
	}

	void OnAttach() override { return; }
	void OnDetach() override { return; }
	void OnImGuiRender() override {}
private:
};

class SandboxProgramm : public Nut::Application {
public:
	SandboxProgramm() {
		PushLayer(new Sandbox::Sandbox2D());
	}

	~SandboxProgramm() {

	}
};


Nut::Application* Nut::CreateApplication(Nut::ApplicationCommandLineArgs args) {
	return new SandboxProgramm();
}
 