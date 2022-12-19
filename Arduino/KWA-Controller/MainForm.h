#pragma once


namespace KwaController {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	private:
        // Baudrate used for serial communication with Arduino.
        // Must match rate defined in Arduino Sketch.
        const int BAUDRATE = 115200;
        // Arduino operation byte codes.
        const int ARDUINO_STOP_REC = 0;
        const int ARDUINO_START_REC = 1;
        const int ARDUINO_SEND_FPS = 2;  // Must match def. in Arduino sketch!

		const int FPS_MIN_VAL = 1;    // Minimum FPS value.
		const int FPS_MAX_VAL = 720;  // Maximum FPS value.
		const int FPS_DEFAULT = 720;  // FPS value set on app startup.

		// Titles, texts, captions of form components.
        static String^ BTN_TEXT_ARDUINO_CONNECT = "Connect";
        static String^ BTN_TEXT_ARDUINO_DISCONNECT = "Disconnect";
		static String^ BTN_TEXT_SYSTEM_START = "Turn lights/cam on";
        static String^ BTN_TEXT_SYSTEM_STOP = "Turn lights/cam off";

		System::UInt16 fps;  // FPS value for triggering camera.
	private: System::Windows::Forms::TrackBar^ trackBarFps;
	private: System::Windows::Forms::Label^ labelFps;
	private: System::Windows::Forms::TextBox^ textBoxFps;
	private: System::Windows::Forms::Label^ labelMinFpsVal;
	private: System::Windows::Forms::Label^ labelMaxFpsVal;


	private: System::Windows::Forms::ErrorProvider^ errorProvider;


	private:
		bool isSystemRunning;  // `true` if lights are on and cam is triggered; otherwise `false`
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			// Constructor code: custom initialization of components/vars, etc.
			//
			this->comboBoxSerialPort->Items->AddRange(this->serialPort->GetPortNames());
			this->serialPort->BaudRate = BAUDRATE;
			this->isSystemRunning = false;

			// Init GUI elements for setting FPS with default value.
			this->textBoxFps->Text = "" + FPS_DEFAULT;
			this->trackBarFps->Value = FPS_DEFAULT;

			// Call `UpdateGui()` last to ensure all variables in the constructor
			// have been initialized.
			this->UpdateGui();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::IO::Ports::SerialPort^ serialPort;
	protected:

	private: System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel1;
	private: System::Windows::Forms::ComboBox^ comboBoxSerialPort;
	private: System::Windows::Forms::Button^ buttonLightsAndCam;

	private: System::Windows::Forms::Button^ buttonSerialConnection;



	private: System::Windows::Forms::Label^ labelSerialPort;


	protected:
	private: System::ComponentModel::IContainer^ components;
	private:
		/// <summary>
		/// Update GUI components' text and state properties based on serial
		/// communication state with Arduino.
		/// </summary>
		void UpdateGui() {
			if (this->serialPort->IsOpen) {
				this->buttonSerialConnection->Text = BTN_TEXT_ARDUINO_DISCONNECT;
				this->buttonLightsAndCam->Enabled = true;
				// Port cannot be changed after communication with Arduino established.
				this->comboBoxSerialPort->Enabled = false;
			}
			else {
				this->buttonSerialConnection->Text = BTN_TEXT_ARDUINO_CONNECT;
				this->buttonLightsAndCam->Enabled = false;
				// Port can only be changed before establishing communication with Arduino.
				this->comboBoxSerialPort->Enabled = true;
			}

			if (this->isSystemRunning) {
				this->buttonLightsAndCam->Text = BTN_TEXT_SYSTEM_STOP;
				// Cannot change FPS value while system is running (recording).
				this->trackBarFps->Enabled = false;
				this->textBoxFps->Enabled = false;
			}
			else {
				this->buttonLightsAndCam->Text = BTN_TEXT_SYSTEM_START;
				// Can only change FPS value while system is not running (recording).
				this->trackBarFps->Enabled = true;
				this->textBoxFps->Enabled = true;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->serialPort = (gcnew System::IO::Ports::SerialPort(this->components));
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->buttonSerialConnection = (gcnew System::Windows::Forms::Button());
			this->buttonLightsAndCam = (gcnew System::Windows::Forms::Button());
			this->labelSerialPort = (gcnew System::Windows::Forms::Label());
			this->comboBoxSerialPort = (gcnew System::Windows::Forms::ComboBox());
			this->trackBarFps = (gcnew System::Windows::Forms::TrackBar());
			this->labelFps = (gcnew System::Windows::Forms::Label());
			this->textBoxFps = (gcnew System::Windows::Forms::TextBox());
			this->labelMinFpsVal = (gcnew System::Windows::Forms::Label());
			this->labelMaxFpsVal = (gcnew System::Windows::Forms::Label());
			this->errorProvider = (gcnew System::Windows::Forms::ErrorProvider(this->components));
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarFps))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorProvider))->BeginInit();
			this->SuspendLayout();
			// 
			// serialPort
			// 
			this->serialPort->BaudRate = 115200;
			this->serialPort->ReadTimeout = 500;
			this->serialPort->WriteTimeout = 500;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->AutoSize = true;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				50.2F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				49.8F)));
			this->tableLayoutPanel1->Controls->Add(this->buttonSerialConnection, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->buttonLightsAndCam, 1, 1);
			this->tableLayoutPanel1->Location = System::Drawing::Point(15, 122);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 27)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(277, 27);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// buttonSerialConnection
			// 
			this->buttonSerialConnection->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->buttonSerialConnection->Location = System::Drawing::Point(3, 3);
			this->buttonSerialConnection->Name = L"buttonSerialConnection";
			this->buttonSerialConnection->Size = System::Drawing::Size(133, 21);
			this->buttonSerialConnection->TabIndex = 1;
			this->buttonSerialConnection->Text = L"Connect";
			this->buttonSerialConnection->UseVisualStyleBackColor = true;
			this->buttonSerialConnection->Click += gcnew System::EventHandler(this, &MainForm::buttonSerialConnection_Click);
			// 
			// buttonLightsAndCam
			// 
			this->buttonLightsAndCam->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->buttonLightsAndCam->Location = System::Drawing::Point(142, 3);
			this->buttonLightsAndCam->Name = L"buttonLightsAndCam";
			this->buttonLightsAndCam->Size = System::Drawing::Size(132, 21);
			this->buttonLightsAndCam->TabIndex = 2;
			this->buttonLightsAndCam->Text = L"Turn Lights and Cam On";
			this->buttonLightsAndCam->UseVisualStyleBackColor = true;
			this->buttonLightsAndCam->Click += gcnew System::EventHandler(this, &MainForm::buttonLightsAndCam_Click);
			// 
			// labelSerialPort
			// 
			this->labelSerialPort->AutoSize = true;
			this->labelSerialPort->Location = System::Drawing::Point(12, 9);
			this->labelSerialPort->Name = L"labelSerialPort";
			this->labelSerialPort->Size = System::Drawing::Size(29, 13);
			this->labelSerialPort->TabIndex = 3;
			this->labelSerialPort->Text = L"Port:";
			// 
			// comboBoxSerialPort
			// 
			this->comboBoxSerialPort->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxSerialPort->FormattingEnabled = true;
			this->comboBoxSerialPort->Location = System::Drawing::Point(47, 6);
			this->comboBoxSerialPort->Name = L"comboBoxSerialPort";
			this->comboBoxSerialPort->Size = System::Drawing::Size(238, 21);
			this->comboBoxSerialPort->TabIndex = 0;
			this->comboBoxSerialPort->SelectedValueChanged += gcnew System::EventHandler(this, &MainForm::comboBoxSerialPort_SelectedValueChanged);
			this->comboBoxSerialPort->Validating += gcnew System::ComponentModel::CancelEventHandler(this, &MainForm::comboBoxSerialPort_Validating);
			this->comboBoxSerialPort->Validated += gcnew System::EventHandler(this, &MainForm::comboBoxSerialPort_Validated);
			// 
			// trackBarFps
			// 
			this->trackBarFps->LargeChange = 10;
			this->trackBarFps->Location = System::Drawing::Point(100, 43);
			this->trackBarFps->Maximum = 720;
			this->trackBarFps->Minimum = 1;
			this->trackBarFps->Name = L"trackBarFps";
			this->trackBarFps->Size = System::Drawing::Size(185, 45);
			this->trackBarFps->TabIndex = 4;
			this->trackBarFps->TickFrequency = 25;
			this->trackBarFps->Value = 700;
			this->trackBarFps->Scroll += gcnew System::EventHandler(this, &MainForm::trackBarFps_Scroll);
			// 
			// labelFps
			// 
			this->labelFps->AutoSize = true;
			this->labelFps->Location = System::Drawing::Point(13, 46);
			this->labelFps->Name = L"labelFps";
			this->labelFps->Size = System::Drawing::Size(30, 13);
			this->labelFps->TabIndex = 5;
			this->labelFps->Text = L"FPS:";
			// 
			// textBoxFps
			// 
			this->textBoxFps->Location = System::Drawing::Point(47, 43);
			this->textBoxFps->MaxLength = 3;
			this->textBoxFps->Name = L"textBoxFps";
			this->textBoxFps->Size = System::Drawing::Size(35, 20);
			this->textBoxFps->TabIndex = 6;
			this->textBoxFps->TextChanged += gcnew System::EventHandler(this, &MainForm::textBoxFps_TextChanged);
			this->textBoxFps->Validating += gcnew System::ComponentModel::CancelEventHandler(this, &MainForm::textBoxFps_Validating);
			this->textBoxFps->Validated += gcnew System::EventHandler(this, &MainForm::textBoxFps_Validated);
			// 
			// labelMinFpsVal
			// 
			this->labelMinFpsVal->AutoSize = true;
			this->labelMinFpsVal->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->labelMinFpsVal->Location = System::Drawing::Point(106, 75);
			this->labelMinFpsVal->Name = L"labelMinFpsVal";
			this->labelMinFpsVal->Size = System::Drawing::Size(13, 13);
			this->labelMinFpsVal->TabIndex = 7;
			this->labelMinFpsVal->Text = L"1";
			// 
			// labelMaxFpsVal
			// 
			this->labelMaxFpsVal->AutoSize = true;
			this->labelMaxFpsVal->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->labelMaxFpsVal->Location = System::Drawing::Point(267, 75);
			this->labelMaxFpsVal->Name = L"labelMaxFpsVal";
			this->labelMaxFpsVal->Size = System::Drawing::Size(25, 13);
			this->labelMaxFpsVal->TabIndex = 8;
			this->labelMaxFpsVal->Text = L"720";
			// 
			// errorProvider
			// 
			this->errorProvider->ContainerControl = this;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoValidate = System::Windows::Forms::AutoValidate::Disable;
			this->CausesValidation = false;
			this->ClientSize = System::Drawing::Size(304, 161);
			this->Controls->Add(this->labelMaxFpsVal);
			this->Controls->Add(this->labelMinFpsVal);
			this->Controls->Add(this->textBoxFps);
			this->Controls->Add(this->labelFps);
			this->Controls->Add(this->trackBarFps);
			this->Controls->Add(this->labelSerialPort);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->comboBoxSerialPort);
			this->MaximumSize = System::Drawing::Size(320, 200);
			this->MinimumSize = System::Drawing::Size(320, 0);
			this->Name = L"MainForm";
			this->Text = L"Kine Wheel Arena – DLC";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->tableLayoutPanel1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarFps))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorProvider))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Boolean ReadByteAndCatchError(System::Byte^ val) {
        try {
			System::Int32 result = this->serialPort->ReadByte();
			*val = result;  // If read succeeded, return byte value.
        }
		catch (const System::TimeoutException^ ex) {
			// The operation did not complete before the time-out period ended. 
			return false;
		}
        catch (const System::InvalidOperationException^ ex) {
			// The specified port is not open.
			return false;
		}

		// -1 indicates *end of stream*
		return *val > -1 ? true : false;  // Return true if no error ocurred
	}

	private: System::Boolean WriteAndCatchError(cli::array<unsigned char>^ buf, int offset, int count) {
        try {
            this->serialPort->Write(buf, offset, count);
        }
		catch (const System::TimeoutException^ ex) {
			// The operation did not complete before the time-out period ended. 
			return false;
		}
        catch (const System::InvalidOperationException^ ex) {
			// The specified port is not open.
			return false;
		}

		return true;  // Return true if no error ocurred
	}

	private: System::Void CloseSerialPort() {
        // Stop system if running.
        if (this->isSystemRunning) {
            this->buttonLightsAndCam_Click(nullptr, nullptr);
        }
        this->serialPort->Close();
	}

    /// <summary>
    /// Opens serial communication with the Arduino.
    /// </summary>
	private: System::Void buttonSerialConnection_Click(System::Object^ sender, System::EventArgs^ e) {
        cli::array<unsigned char>^ buf = { 0 };

		if (!this->serialPort->IsOpen) {
			// Explicitly validate form fields; `AutoValidation` property of the main form must be disabled.
			// When closing a serial port, this check is unnecessary.
			if (!ValidateChildren()) {
				return;
			}

			this->serialPort->Open();
			/**
			 * The IsOpen property tracks whether the port is open for use by the caller,
			 * not whether the port is open by any application on the machine.
			 * C.f. https://learn.microsoft.com/en-us/dotnet/api/system.io.ports.serialport.isopen?view=dotnet-plat-ext-7.0#remarks
			 *
			 * Therefore, we need to test whether the Arduino responds on the opened port.
			 * Write the first byte from `buf`, which contains op code to stop any running recording.
			 * C.f. https://learn.microsoft.com/en-us/dotnet/api/system.io.ports.serialport.write?view=dotnet-plat-ext-7.0#system-io-ports-serialport-write(system-byte()-system-int32-system-int32)
			 */
			buf[0] = ARDUINO_STOP_REC;
			String^ s;
			// NOTE: `ReadExisting()` returns the empty string if reading from an existing port, but
			// without an application on the other side (e.g., port where Arduino is not connected to).
			// Therefore, we need to test for the empty string and fail in that case, as we should
			// always get a *Waiting* response from the Arduino in response to our previously sent
			// *Stop* request.
			// C.f. https://learn.microsoft.com/en-us/dotnet/api/system.string?view=net-7.0#null-strings-and-empty-strings
			Byte b = 0;
			if (!this->WriteAndCatchError(buf, 0, 1) || !this->ReadByteAndCatchError(b)) {
				this->serialPort->Close();

				String^ errorMsg = "Could not communicate with Arduino on port " + this->serialPort->PortName + ".\n"
					"Check that the Arduino is connected to the computer and the selected port is correct.";

                // Set the ErrorProvider error with the text to display.
                this->errorProvider->SetError( this->comboBoxSerialPort, errorMsg );
			} else {
                // Clear 'received buffer', as the Arduino can send data on serial reset,
                // which is not relevant and might interfere with the program logic.
                s = this->serialPort->ReadExisting();
            }

		}
		else {
			this->CloseSerialPort();
		}
        this->UpdateGui();
	}

    private: System::Void buttonLightsAndCam_Click(System::Object^ sender, System::EventArgs^ e) {
		cli::array<unsigned char>^ buf = { 0, 0, 0 };

		// Explicitly validate form fields; `AutoValidation` property of the main form must be disabled.
		if (!ValidateChildren()) {
			// If invalid FPS value, don't proceed.
			return;
		}

		// TODO: Maybe first read everything from receive buffer, ensures that
		// we get responses corresponding to our requests sent below.
		this->serialPort->ReadExisting();

        buf[0] = ARDUINO_STOP_REC;
		// TODO: if error, show msg, set isSysRunning=False, disconnect, return
		WriteAndCatchError(buf, 0, 1);

		// Read response from Arduino; possible responses:
		//   (a) pre: sys running => post: sys waiting
		//   (b) pre: sys waiting => post: sys waiting
		//       * b/c 'stop' request won't change sys state when 'waiting'
		//   (c) pre: no communicaton => read timeout
		int responseCode = this->serialPort->ReadByte();
		// TODO: (?) Response code should be 'waiting', but not really necessary to test

		if (this->isSystemRunning) {  // lights on, cam triggered
			this->isSystemRunning = false;
            // Already sent *stop* request, nothing more to do.
		}
		else {  // light off, cam not triggered
            buf[0] = ARDUINO_START_REC;
			// TODO: Set frames dynamically; this is just a test!
            // FPS value (16-bit word, network byte order)
			buf[1] = static_cast<unsigned char>((this->fps >> 8) & 0xFF);  // Higher-order byte
            buf[2] = static_cast<unsigned char>(this->fps & 0xFF);         // Lower-order byte

			// Send 'start' and 'FPS' request in one write operation.
			if (!this->WriteAndCatchError(buf, 0, 3)) {
                this->isSystemRunning = false;
				// TODO: show err msg, close serial con, reset button, etc. states, return
			}

			// TODO: Do read error handling; on error, show msg and disconnect
            // Read response from Arduino.
            int responseCode = this->serialPort->ReadByte();

			if (responseCode == ARDUINO_SEND_FPS) {
                responseCode += 1;
                this->isSystemRunning = true;
				// Already sent FPS in request above, nothing more to do.
			}
			else {
                this->isSystemRunning = false;
				// TODO: Show err msg.
			}

		}

        UpdateGui();
    }

    /// <summary>
    /// Set selected serial COM port as serial communication port.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    /// <returns></returns>
    private: System::Void comboBoxSerialPort_SelectedValueChanged(System::Object^ sender, System::EventArgs^ e) {
        this->serialPort->PortName = comboBoxSerialPort->Text;
    }

    private: System::Void textBoxFps_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		/*
		 * When this method returns, contains the 16 - bit unsigned integer value that is equivalent
		 * to the number contained in s, if the conversion succeeded, or zero if the conversion failed.
		 *
		 * C.f. https://learn.microsoft.com/en-us/dotnet/api/system.uint16.tryparse?view=net-7.0#system-uint16-tryparse(system-string-system-uint16@)
		 */
		System::UInt16 fps;
		bool success = System::UInt16::TryParse(this->textBoxFps->Text, fps);
        if (success && fps >= FPS_MIN_VAL && fps <= FPS_MAX_VAL)
        {
			this->trackBarFps->Value = fps;
			this->fps = fps;
        }

		// Explicityly trigger validation; otherwise, validation will only occur if the main form's
		// `AutoValidation` property is not disabled and after the user changes focus to another
		// element with property `CausesValidation=true`.
		// UPDATE: If no validation triggered here, it only happens on 'Connect' button press.
		//this->ValidateChildren();
    }

    private: System::Void trackBarFps_Scroll(System::Object^ sender, System::EventArgs^ e) {
		this->textBoxFps->Text = "" + this->trackBarFps->Value;
		this->fps = this->trackBarFps->Value;
		// Explicityly trigger validation; otherwise, validation will only occur if the main form's
		// `AutoValidation` property is not disabled and after the user changes focus to another
		// element with property `CausesValidation=true`.
		// UPDATE: If no validation triggered here, it only happens on 'Connect' button press.
		//this->ValidateChildren();
    }

	private: bool IsFpsValueValid(String^ fps, interior_ptr<String^> errorMessage) {
		System::UInt16 iFps;
		if (System::UInt16::TryParse(fps, iFps) && iFps >= FPS_MIN_VAL && iFps <= FPS_MAX_VAL) {
			*errorMessage = "";
			return true;
		}
		else {
			*errorMessage = "FPS value must be a number in [" + this->FPS_MIN_VAL + ", " + this->FPS_MAX_VAL + "].";
			return false;
		}
	}

    private: System::Void textBoxFps_Validating(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
        String^ errorMsg;
        if (!IsFpsValueValid(this->textBoxFps->Text, &errorMsg) ) {
			// Cancel the event and select the text to be corrected by the user.
            e->Cancel = true;
            textBoxFps->Select( 0, textBoxFps->Text->Length );

            // Set the ErrorProvider error with the text to display.
            this->errorProvider->SetError( textBoxFps, errorMsg );
        }
    }
    private: System::Void textBoxFps_Validated(System::Object^ sender, System::EventArgs^ e) {
        // If all conditions have been met, clear the ErrorProvider of errors.
        this->errorProvider->SetError( textBoxFps, "" );
    }

    private: System::Void comboBoxSerialPort_Validating(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
		String^ errorMsg = "Select a valid serial port from the drop-down list.";
		// NOTE: `Validating` won't get called if the user never clicked(?) on the
		// GUI element, i.e., if the user leaves the default (no item selected),
		// validation won't run!
        /**
         * If user did not select port from drop-down list (SelectedItem == NULL),
         * display msg and abort.
         *
         * C.f. https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.messagebox?view=netframework-4.8
         */
        if (!this->comboBoxSerialPort->SelectedItem) {
            // Cancel the event.
            e->Cancel = true;

            // Set the ErrorProvider error with the text to display.
            this->errorProvider->SetError( this->comboBoxSerialPort, errorMsg );
        }
    }

    private: System::Void comboBoxSerialPort_Validated(System::Object^ sender, System::EventArgs^ e) {
        // If all conditions have been met, clear the ErrorProvider of errors.
        this->errorProvider->SetError( this->comboBoxSerialPort, "");
    }

    private: System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		// NOTE: Closing the program using the 'X' button on the top-right
		// of the GUI won't work if any form element has an error value set
		// and `AutoValidate` is enabled.
		this->AutoValidate = Windows::Forms::AutoValidate::Disable;

		// Stop Arduino recording and close serial port.
        this->CloseSerialPort();
    }
};
}

