#include "MainForm.h"


using namespace System;
using namespace System::Windows::Forms;


int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    KwaController::MainForm form;
    Application::Run(% form);
}

