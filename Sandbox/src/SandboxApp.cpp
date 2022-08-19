#include <Nut.h>

class ExampleLayer : public Nut::Layer {
public:
	ExampleLayer()
		:Layer("Example"){}

	void OnUpdate() override {
		//NT_INFO("ExampleLayer::Update");
	}

	void OnEvent(Nut::Event& e) override {
		NT_TRACE("{0}", e);
	}

	void OnAttach() override { return; }
	void OnDetach() override { return; }
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
 