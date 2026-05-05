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

<img width="3072" height="4096" alt="pcb_solder_7" src="https://github.com/user-attachments/assets/360f7abe-b615-44c5-8868-0688f60921bf" />


<img width="2561" height="1397" alt="model_design_24_overall" src="https://github.com/user-attachments/assets/65a6ec09-efc6-44e6-b5b5-7fa68f768365" />


<img width="2561" height="1397" alt="model_design_25_overall" src="https://github.com/user-attachments/assets/e96f1ce5-086f-4129-8f5c-81b8e64a1af9" />


<img width="4095" height="3072" alt="assembly_8_base" src="https://github.com/user-attachments/assets/89bbb8c9-890a-40c8-8687-e9adac848ab2" />


<img width="4095" height="3072" alt="assembly_15_base" src="https://github.com/user-attachments/assets/a97253b6-5f69-4fea-84c2-1796fddc498a" />


<img width="4096" height="3072" alt="assembly_50_sensor_ladder" src="https://github.com/user-attachments/assets/aeefe1bc-0475-4ed3-af38-9997ed2bd5de" />


<img width="4096" height="3072" alt="pcb_base_set_2" src="https://github.com/user-attachments/assets/252c22d3-ef41-4d2f-b340-616be23de858" />


<img width="4096" height="3072" alt="pcb_base_set_12_cable_ties" src="https://github.com/user-attachments/assets/31eda34c-5ddc-45d3-824d-6e8adfe9d192" />


<img width="4096" height="3072" alt="pcb_base_set_13_cable_ties" src="https://github.com/user-attachments/assets/7c5ef89e-280c-4ab2-a1bf-6af8b8c4ad8e" />


<img width="4096" height="3072" alt="pcb_connect_test_3" src="https://github.com/user-attachments/assets/b93dbee9-2693-4ace-9e9b-cf7a91ddcd9c" />


<img width="4096" height="3072" alt="pcb_connect_test_5" src="https://github.com/user-attachments/assets/01d78fdd-8d81-4d73-a1ed-fc3d087ac2d1" />


<img width="4096" height="3072" alt="pcb_connect_test_6" src="https://github.com/user-attachments/assets/3a481ca6-63ed-4f4a-bc9e-c1b0464232c2" />


<img width="4096" height="3072" alt="analog_exp_3_prepare" src="https://github.com/user-attachments/assets/100a6dfb-f010-4843-92f7-5812d508c52a" />


<img width="4096" height="3072" alt="pcb_connect_test_7" src="https://github.com/user-attachments/assets/ed861458-7a11-4166-8911-86877bcde064" />


<img width="4096" height="3072" alt="final_showcase_with_equ_1" src="https://github.com/user-attachments/assets/1d91b0ca-a7c2-4daa-ab1b-9f3fb947edfb" />


<img width="4096" height="3072" alt="analog_run_2_manual_sensor" src="https://github.com/user-attachments/assets/811291f0-68ff-4b9a-a8e4-4c79d42e712a" />


<img width="2560" height="1392" alt="web_dashboard_work_2_sensor" src="https://github.com/user-attachments/assets/a08c98d3-a3ef-45f5-8eba-3d72b65301a6" />


<img width="2560" height="1392" alt="web_dashboard_work_4_sensor" src="https://github.com/user-attachments/assets/b7a36654-454c-4597-a2ff-a911026193c6" />


<img width="4096" height="3072" alt="analog_run_4_dashboard_sensor" src="https://github.com/user-attachments/assets/c93b549b-7684-4d3d-8fdc-349f293e612b" />


<img width="2560" height="1392" alt="web_dashboard_work_30_sensor_exp_talk" src="https://github.com/user-attachments/assets/fc661b64-6ebe-41c0-9e4a-cc7f9008d9d2" />


<img width="4096" height="3072" alt="analog_exp_5_pressure" src="https://github.com/user-attachments/assets/640284b0-a59c-4214-9a96-78643f4645ac" />


<img width="2560" height="1392" alt="web_dashboard_work_33_sensor_show_exps" src="https://github.com/user-attachments/assets/3ed4d8e4-24bc-4016-b1d5-a5d669a3a652" />


<img width="4096" height="3072" alt="analog_exp_7_alcohol" src="https://github.com/user-attachments/assets/ffa71b0e-7045-4095-89fb-72caf8ea5779" />


<img width="2560" height="1392" alt="web_dashboard_work_35_sensor_show_exps" src="https://github.com/user-attachments/assets/bbbf9142-5149-45d0-83a9-7363b9a89afb" />


<img width="4096" height="3072" alt="analog_exp_9_weight" src="https://github.com/user-attachments/assets/ce5eb90a-2ec4-465e-934a-41ae337eae56" />


<img width="2560" height="1392" alt="web_dashboard_work_37_sensor_show_exps" src="https://github.com/user-attachments/assets/83b71341-5fd6-4a9e-b590-f8e6bb778465" />


<img width="2560" height="1392" alt="web_dashboard_work_39_sensor_show_exps" src="https://github.com/user-attachments/assets/053f655c-fd0d-4ea4-8bc9-645ae6524bb6" />


<img width="4096" height="3072" alt="analog_exp_11_water" src="https://github.com/user-attachments/assets/cc548a31-ed70-4f29-97ac-f90efcbf0e5d" />


<img width="4096" height="3072" alt="analog_exp_18_gnss_found_signal" src="https://github.com/user-attachments/assets/1866c7fb-7550-460d-8d10-b163957f7a0f" />


<img width="4096" height="3072" alt="analog_exp_20_gnss_found_signal" src="https://github.com/user-attachments/assets/e707f2c7-bc23-4f40-a974-44b7425c7753" />


<img width="2560" height="1392" alt="web_dashboard_work_gnss_found_full_signal_2" src="https://github.com/user-attachments/assets/840f4e13-47c1-424d-8b9b-5799c2b7e66d" />


<img width="2560" height="1392" alt="web_dashboard_work_44_gemini_lesson" src="https://github.com/user-attachments/assets/5ef893b9-c7ec-4593-b572-f201bc97bc9b" />


<img width="2560" height="1392" alt="web_dashboard_work_47_gemini_lesson" src="https://github.com/user-attachments/assets/713d87e7-9f5b-4e5f-8b9d-cb5709379062" />


<img width="4096" height="3072" alt="analog_account_register_1" src="https://github.com/user-attachments/assets/a9a6d2d5-cf61-4239-9c53-5172565b00cd" />


<img width="4096" height="3072" alt="analog_account_register_5" src="https://github.com/user-attachments/assets/1d2bf83b-916f-4cb2-8430-f6fc3a56ec89" />


<img width="4096" height="3072" alt="analog_account_register_6" src="https://github.com/user-attachments/assets/ae47759f-92f6-48fa-8f0d-b745041a3297" />


<img width="4096" height="3072" alt="analog_lesson_3_prepare" src="https://github.com/user-attachments/assets/ecf44933-e4e7-412f-85e6-34d2ead67163" />


<img width="4096" height="3072" alt="analog_lesson_4_prepare" src="https://github.com/user-attachments/assets/97098693-c062-49b2-9e40-1477fa0706e3" />


<img width="2560" height="1392" alt="web_dashboard_work_54_gemini_lesson" src="https://github.com/user-attachments/assets/b8a59e83-e515-4129-a914-0715dfac7787" />


<img width="2560" height="1392" alt="web_dashboard_work_55_gemini_lesson" src="https://github.com/user-attachments/assets/bc59d2cd-669f-4a6b-8532-2a45a0206a0d" />


<img width="2560" height="1392" alt="web_dashboard_work_56_gemini_lesson" src="https://github.com/user-attachments/assets/4de4fc1e-8b76-4c0f-8f3a-027f3560af47" />


<img width="2560" height="1392" alt="web_dashboard_work_59_gemini_lesson" src="https://github.com/user-attachments/assets/640b1f7f-34b1-418f-8e30-e717e4364ad3" />


<img width="2560" height="1392" alt="web_dashboard_work_60_gemini_lesson" src="https://github.com/user-attachments/assets/09c8ce68-ae26-40db-ad34-e46f56019476" />


<img width="2560" height="1392" alt="web_dashboard_work_72_gemini_lesson_custom_question" src="https://github.com/user-attachments/assets/8f9268e2-adfd-4fed-98dd-dfc2fc24109c" />


<img width="2560" height="1392" alt="web_dashboard_work_74_gemini_lesson_custom_question" src="https://github.com/user-attachments/assets/fd17386c-6247-437b-90be-a32158fedef6" />


<img width="2560" height="1392" alt="web_dashboard_work_76_gemini_lesson_custom_question" src="https://github.com/user-attachments/assets/3f851504-2881-4179-b452-8e59ba2f249b" />


<img width="2560" height="1392" alt="web_dashboard_work_67_save_new_sample" src="https://github.com/user-attachments/assets/33ac1637-7dc2-4108-8c1a-7c89df04db1b" />


<img width="2560" height="1392" alt="image" src="https://github.com/user-attachments/assets/b9f05ea5-e529-4cba-8198-46e22094c4eb" />


<img width="2560" height="1392" alt="web_dashboard_work_88_signin" src="https://github.com/user-attachments/assets/b1061327-6ef9-478e-8621-9e48256d5e2d" />


<img width="4096" height="3072" alt="analog_account_verify_1_not_f" src="https://github.com/user-attachments/assets/0373ac4e-548b-486f-8907-c6a0268a1b3a" />


<img width="4096" height="3072" alt="analog_account_verify_6_success" src="https://github.com/user-attachments/assets/f120eb66-542a-49c9-8b9c-160521d205b3" />


<img width="4096" height="3072" alt="analog_account_verify_7_success" src="https://github.com/user-attachments/assets/6e59a9a5-91e8-4670-adc6-03ffab111514" />


<img width="2560" height="1392" alt="web_dashboard_work_91_signin" src="https://github.com/user-attachments/assets/c3f9148c-7e9c-4574-aafe-da5d6028fa84" />


<img width="2560" height="1392" alt="web_dashboard_work_92_signin" src="https://github.com/user-attachments/assets/7887b257-60b0-48ad-ac51-32407dfb635c" />

