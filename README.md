> I developed my portfolio website — kutluhanaktar.com — from scratch and decided to migrate all of my codebase to GitHub to provide a simple and straightforward experimentation or replication method for my proof-of-concept projects. Nonetheless, since I focused on demonstrating my thought process and experiment results thoroughly in the written tutorial format, including but not limited to PCB and 3D model designing steps, I could not provide enough information to fully explain my concepts and instructions via GitHub descriptions only. Thus, I highly recommend inspecting the associated project tutorials and videos on my portfolio website or other platforms (maker communities), i.e., Hackster, Hackaday, and Instructables.

# Original Project Publication Date

**May 6, 2026**

# Description

**Based on Arduino UNO Q, this lab assistant is able to show real-time sensor readings, identify users by fingerprint, recognize lab equipment via object detection, generate AI lessons via Gemini, and talk through its full-fledged web dashboard.**

After hearing about the launch of the brand-new Arduino UNO Q, designed as the first SBC (single-board computer) with Arduino's philosophy of bridging the gap between employing professional development tools and implementing them as novices when creating introductory projects or as experts while prototyping complex mechanisms rapidly yet stably, I thought it would be a great opportunity to redesign my previous AI-driven lab assistant project and enable more developers, beginner or expert, to replicate, experiment, or improve this new AI-based ancillary lab assistant thanks to the built-in Arduino UNO Q features and its beginner-friendly development platform — Arduino App Lab.

As you may know, if you have read one of my previous project tutorials, I prefer building my AIoT projects on the target development boards and environments from scratch and enjoy developing unique methods, applications, and mechanisms to collect custom training data and achieve intended device features, strictly following my methodology of developing proof-of-concept research projects. Nonetheless, in this project, I heavily focused on developing all lab assistant features based on the provided UNO Q and Arduino App Lab characteristics, such as the built-in Bricks, native microprocessor-microcontroller communication procedure, and Linux-oriented SBC board architecture, to ensure that anyone with a UNO Q can effortlessly replicate and examine this lab assistant without needing to have a deep understanding of all aspects of this project; coding, web design, neural network training, LLM-implementation, 3D modeling, etc. In this regard, I hope this project serves as an entry point for developing research projects, encouraging readers to reverse-engineer the features of this AI-driven lab assistant to gain a deeper understanding of AIoT development on the edge.

As I was taking inspiration from my previous lab assistant project, I heavily modified the device structure and added a lot of new features specific to this iteration, for instance, designing a unique PCB (UNO Q shield) for utilizing various lab sensors to conduct LLM-assisted basic lab experiments. After months of hard work, I managed to complete the reimagined AI-driven ancillary lab assistant structure and develop all the features I envisioned on UNO Q by solely employing the Arduino App Lab development environment, providing foundational building blocks (Bricks).

🤖 To build the ancillary lab assistant structure:

✍🏻 I designed a unique PCB as a UNO Q shield (hat) to connect the selected lab sensors and create the analog lab assistant interface, including the capacitive fingerprint sensor.

✍🏻 Then, I modeled 3D parts to design the ancillary lab assistant base, containing the USB camera and the analog interface.

✍🏻 Finally, I designed a modular lab sensor ladder, organizing all sensors and secondary experiment tools, to create a compact but easy-to-use instrument.

🤖 To accomplish all of the ancillary lab assistant features I contemplated, performed by an Arduino App Lab application:

🛠️ I trained an Edge Impulse object detection model to identify various lab equipment.

🛠️ I programmed the MCU (STM32) to collect real-time sensor information and manage the analog lab assistant interface.

🛠️ I developed a feature-rich web dashboard as the primary user interface and control panel of the lab assistant, hosted directly by the Arduino App Lab.

🛠️ I incorporated Google Gemini to enable the lab assistant to generate LLM-based lessons about the detected lab equipment.

🛠️ Thanks to the built-in background Linux MPU-MCU communication service (Arduino Router), I built the interconnected interface background in Python, handling the data transfer between the web dashboard, the analog interface (MCU), and the Qualcomm QRB (MPU) running the essential App Lab Bricks (Docker containers); database registration, inference running, web dashboard (UI) hosting, etc.

🤖 The finalized ancillary lab assistant allows users to:

🔬 create web dashboard accounts and sign in via fingerprint authentication,

🔬 monitor real-time lab sensor readings via the analog interface or the web dashboard,

🔬 inspect LLM-generated sensor guides and experiment tips for each lab sensor via the web dashboard,

🔬 capitalizing on the built-in browser text-to-speech (TTS) module, listen to LLM-generated sensor guides and experiment tips,

🔬 identify lab equipment via the provided Edge Impulse FOMO object detection model,

🔬 use the predefined equipment questions or enter a specific one to generate AI lessons through Google Gemini,

🔬 access the list of LLM-generated lessons assigned to your account on the web dashboard anytime,

🔬 study LLM-generated lessons by reading or listen to them via the TTS module.

# Inspect the project tutorial on:

- **[kutluhanaktar.com](https://www.kutluhanaktar.com/projects/AI_driven_Ancillary_Lab_Assistant_w_UNO_Q_and_Gemini/)**
- **[Hackster]()**
- **[Instructables]()**
- **[Hackaday]()**

<img width="384" height="512" alt="pcb_solder_7" src="https://github.com/user-attachments/assets/9852a9ea-5ddf-4f94-bcbc-188012e775cf" /> <br>

<img width="512" height="279" alt="model_design_24_overall" src="https://github.com/user-attachments/assets/eb90a1ae-8929-4774-86d8-09ef31d90384" /> <br>

<img width="512" height="279" alt="model_design_25_overall" src="https://github.com/user-attachments/assets/32b9128b-2ff2-48ea-81c7-f3bf39a868d0" /> <br>

<img width="512" height="384" alt="assembly_8_base" src="https://github.com/user-attachments/assets/2cda9024-5b37-4370-917d-b5cb62756a7a" /> <br>

<img width="512" height="384" alt="assembly_15_base" src="https://github.com/user-attachments/assets/93a0caa1-38d8-4ec9-86a5-5303fccde14a" /> <br>

<img width="512" height="384" alt="assembly_50_sensor_ladder" src="https://github.com/user-attachments/assets/ec929759-a795-4a78-972c-80b44305af84" /> <br>

<img width="512" height="384" alt="pcb_base_set_2" src="https://github.com/user-attachments/assets/24cbe461-2650-43c5-9648-392b28731499" /> <br>

<img width="512" height="384" alt="pcb_base_set_12_cable_ties" src="https://github.com/user-attachments/assets/ad63393b-5c13-40e3-9fc3-135a970ad506" /> <br>

<img width="512" height="384" alt="pcb_base_set_13_cable_ties" src="https://github.com/user-attachments/assets/e84732bf-9315-45e0-86f1-2c3157c7f661" /> <br>

<img width="512" height="384" alt="pcb_connect_test_3" src="https://github.com/user-attachments/assets/93aa5b7f-799e-42c1-a8df-89c10457a65a" /> <br>

<img width="512" height="384" alt="pcb_connect_test_5" src="https://github.com/user-attachments/assets/56857687-09ee-4fae-abd0-e39fd96203c4" /> <br>

<img width="512" height="384" alt="pcb_connect_test_6" src="https://github.com/user-attachments/assets/32db71e4-78d7-4978-9e25-34dade1d364d" /> <br>

<img width="512" height="384" alt="analog_exp_3_prepare" src="https://github.com/user-attachments/assets/21f7e162-5b97-455b-9c91-f7e1bc61c1f2" /> <br>

<img width="512" height="384" alt="pcb_connect_test_7" src="https://github.com/user-attachments/assets/0859cfbf-5b7a-498c-be70-0c2ef03948a0" /> <br>

<img width="512" height="384" alt="final_showcase_with_equ_1" src="https://github.com/user-attachments/assets/ce658a64-9469-403a-a193-ae14cae34e88" /> <br>

<img width="512" height="384" alt="analog_run_2_manual_sensor" src="https://github.com/user-attachments/assets/c46bef4c-6c5d-4c2f-ae78-cf35b5a5176f" /> <br>

<img width="512" height="278" alt="web_dashboard_work_2_sensor" src="https://github.com/user-attachments/assets/f914236b-ea9c-48e9-bc7a-c9638b25729f" /> <br>

<img width="512" height="278" alt="web_dashboard_work_4_sensor" src="https://github.com/user-attachments/assets/70ee71c8-e792-4479-8c80-86f7aad43a06" /> <br>

<img width="512" height="384" alt="analog_run_4_dashboard_sensor" src="https://github.com/user-attachments/assets/33f344e9-edd1-4587-8622-82dc4aa8dd0a" /> <br>

<img width="512" height="278" alt="web_dashboard_work_30_sensor_exp_talk" src="https://github.com/user-attachments/assets/9aaa8547-ab45-45e8-aed0-3bce11418e54" /> <br>

<img width="512" height="384" alt="analog_exp_5_pressure" src="https://github.com/user-attachments/assets/ab1faa03-61e1-4289-9bb9-9c30503dc7f4" /> <br>

<img width="512" height="278" alt="web_dashboard_work_33_sensor_show_exps" src="https://github.com/user-attachments/assets/7117eccb-9639-42bf-888c-32eb6ff2bd2d" /> <br>

<img width="512" height="384" alt="analog_exp_7_alcohol" src="https://github.com/user-attachments/assets/026e5dc2-b4a3-4d2c-801e-74fa20c53145" /> <br>

<img width="512" height="278" alt="web_dashboard_work_35_sensor_show_exps" src="https://github.com/user-attachments/assets/c0a37034-74e5-4654-bcee-595427ba6f13" /> <br>

<img width="512" height="384" alt="analog_exp_9_weight" src="https://github.com/user-attachments/assets/74922282-9689-4e26-a140-403715f71d7f" /> <br>

<img width="512" height="278" alt="web_dashboard_work_37_sensor_show_exps" src="https://github.com/user-attachments/assets/8da87c66-0aa0-47eb-bece-12c3207dd5b2" /> <br>

<img width="512" height="278" alt="web_dashboard_work_39_sensor_show_exps" src="https://github.com/user-attachments/assets/04102787-6779-4af2-9d24-abd981f822a2" /> <br>

<img width="512" height="384" alt="analog_exp_11_water" src="https://github.com/user-attachments/assets/2bb28a6d-04a9-4d2f-8a4e-61b6119511b5" /> <br>

<img width="512" height="384" alt="analog_exp_18_gnss_found_signal" src="https://github.com/user-attachments/assets/03920361-28be-454d-a655-8ccb1215ac46" /> <br>

<img width="512" height="384" alt="analog_exp_20_gnss_found_signal" src="https://github.com/user-attachments/assets/19a12a3f-669a-4dd0-be9e-53bc79d19d8d" /> <br>

<img width="512" height="278" alt="web_dashboard_work_gnss_found_full_signal_2" src="https://github.com/user-attachments/assets/81b696f1-7b33-4535-91b3-34837afe6ad4" /> <br>

<img width="512" height="278" alt="web_dashboard_work_44_gemini_lesson" src="https://github.com/user-attachments/assets/f1124d77-e755-4701-8337-a40aa2719673" /> <br>

<img width="512" height="278" alt="web_dashboard_work_47_gemini_lesson" src="https://github.com/user-attachments/assets/f681b1aa-69ee-4310-b336-0927d2b08509" /> <br>

<img width="512" height="384" alt="analog_account_register_1" src="https://github.com/user-attachments/assets/8e695c3e-566d-48e9-94ef-0fea651d31f1" /> <br>

<img width="512" height="384" alt="analog_account_register_5" src="https://github.com/user-attachments/assets/c0fa88c4-6de5-40b5-af5f-b167af5b868f" /> <br>

<img width="512" height="384" alt="analog_account_register_6" src="https://github.com/user-attachments/assets/405b2b60-7a90-49bf-9f20-aef9f5778038" /> <br>

<img width="512" height="384" alt="analog_lesson_3_prepare" src="https://github.com/user-attachments/assets/81423ade-5bed-4e49-a3d9-ad86a7267828" /> <br>

<img width="512" height="384" alt="analog_lesson_4_prepare" src="https://github.com/user-attachments/assets/7130efdb-4f0c-488e-9730-bb84414f0823" /> <br>

<img width="512" height="278" alt="web_dashboard_work_54_gemini_lesson" src="https://github.com/user-attachments/assets/274ed2e0-4cae-42fe-95dd-650254add1a6" /> <br>

<img width="512" height="278" alt="web_dashboard_work_55_gemini_lesson" src="https://github.com/user-attachments/assets/4bd9855e-58e2-47b3-aa01-bd3128171b62" /> <br>

<img width="512" height="278" alt="web_dashboard_work_56_gemini_lesson" src="https://github.com/user-attachments/assets/835f92a4-f89e-458d-bd69-ff1e7794b749" /> <br>

<img width="512" height="278" alt="web_dashboard_work_59_gemini_lesson" src="https://github.com/user-attachments/assets/6cd11bc3-dbdf-486a-97c7-f7a698ff5b62" /> <br>

<img width="512" height="278" alt="web_dashboard_work_60_gemini_lesson" src="https://github.com/user-attachments/assets/732cf271-93a0-4f62-b171-fb3fb75994b2" /> <br>

<img width="512" height="278" alt="web_dashboard_work_72_gemini_lesson_custom_question" src="https://github.com/user-attachments/assets/70cc05a9-82ea-4913-977a-a6b6840ea994" /> <br>

<img width="512" height="278" alt="web_dashboard_work_74_gemini_lesson_custom_question" src="https://github.com/user-attachments/assets/aed622ab-c100-41bb-8196-84de90df56a6" /> <br>

<img width="512" height="278" alt="web_dashboard_work_76_gemini_lesson_custom_question" src="https://github.com/user-attachments/assets/a3e3acee-e7e9-45c3-a85c-cfbd94c25ace" /> <br>

<img width="512" height="278" alt="web_dashboard_work_67_save_new_sample" src="https://github.com/user-attachments/assets/ff1f86cd-3bab-4338-872d-a4c80701419c" /> <br>

<img width="512" height="278" alt="web_dashboard_work_69_save_new_sample" src="https://github.com/user-attachments/assets/1e487ef0-d870-4f06-a16e-4d2dca647d2d" /> <br>

<img width="512" height="278" alt="web_dashboard_work_88_signin" src="https://github.com/user-attachments/assets/f7c831e2-4062-43cb-9c42-30e1e780ab00" /> <br>

<img width="512" height="384" alt="analog_account_verify_1_not_f" src="https://github.com/user-attachments/assets/dfc9bbe1-8f74-4618-a09e-e9a3c75eb5b2" /> <br>

<img width="512" height="384" alt="analog_account_verify_6_success" src="https://github.com/user-attachments/assets/c2a9c570-313b-45b1-8ae5-446e6ff006ac" /> <br>

<img width="512" height="384" alt="analog_account_verify_7_success" src="https://github.com/user-attachments/assets/09f97088-2a6e-4030-83e0-ce0c8b286ba3" /> <br>

<img width="512" height="278" alt="web_dashboard_work_91_signin" src="https://github.com/user-attachments/assets/66202983-905c-4cd5-a99f-3243c499af53" /> <br>

<img width="512" height="278" alt="web_dashboard_work_92_signin" src="https://github.com/user-attachments/assets/38172bc2-85ef-40e8-865e-1ad803a8e041" /> <br>
