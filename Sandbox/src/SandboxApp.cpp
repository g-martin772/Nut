#include <Nut.h>

class ExampleLayer : public Nut::Layer {
public:
	ExampleLayer()
		:Layer("Example"){}

	void OnUpdate() override {
		
	}

	void OnEvent(Nut::Event& e) override {
		//NT_TRACE("{0}", e);
	}

	void OnAttach() override { 
	}
	void OnDetach() override { return; }

	void OnImGuiRender() override {
		ImGui::Begin("test");
		ImGui::Text("Hello World");
		ImGui::End();
	}
};

class Sandbox : public Nut::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};


Nut::Application* Nut::CreateApplication() {
	return new Sandbox();
}
 