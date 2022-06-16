//#include "../include/imgui_app.h"
//
//Editor edit;
//
//void InputEditor()
//{
//    if (edit.IsInEditor == true)
//    {
//        IMGUI_CHECKVERSION();
//        ImGui::CreateContext();
//        ImGuiIO& io = ImGui::GetIO();
//
//        ImGui::StyleColorsDark();
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//
//    }
//
//}
//
//void RenderingEditor(GLFWwindow* window)
//{
//    if (edit.IsInEditor == true)
//    {
//        // render your GUI
//        ImGui::Begin("Demo window");
//        ImGui::Button("Hello!");
//        ImGui::End();
//
//        // Render dear imgui into screen
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        int display_w, display_h;
//        glfwGetFramebufferSize(window, &display_w, &display_h);
//        glViewport(0, 0, display_w, display_h);
//    }
//
//    else if (edit.IsInEditor == false)
//    {
//        ImGui_ImplOpenGL3_Shutdown();
//        ImGui_ImplGlfw_Shutdown();
//        ImGui::DestroyContext();
//    }
//}