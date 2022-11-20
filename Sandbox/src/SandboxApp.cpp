//#include <Nut.h>
//#include <Nut/Core/Entrypoint.h>
//
//#include "Sandbox2D.h"
//
//class ExampleLayer : public Nut::Layer {
//public:
//	ExampleLayer()
//		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true){
//		
//	}
//
//	void OnUpdate(Nut::Timestep ts) override {
//		//NT_TRACE("Deltatime: {0}s ({1})", ts.GetSeconds(), ts.GetMilliseconds());
//	}
//
//	void OnEvent(Nut::Event& e) override {
//		m_CameraController.OnEvent(e);
//	}
//
//	void OnAttach() override { return; }
//	void OnDetach() override { return; }
//	void OnImGuiRender() override {}
//private:
//};
//
//class SandboxProgramm : public Nut::Application {
//public:
//	SandboxProgramm() {
//		//PushLayer(new ExampleLayer());
//		PushLayer(new Sandbox::Sandbox2D());
//	}
//
//	~SandboxProgramm() {
//
//	}
//};
//
//
//Nut::Application* Nut::CreateApplication(Nut::ApplicationCommandLineArgs args) {
//	return new SandboxProgramm();
//}
// 