# AI-driven Web-based Ancillary Lab Assistant | UNO Q & Gemini
#
# Arduino UNO Q
#
# By Kutluhan Aktar

import os
from arduino.app_bricks.video_objectdetection import VideoObjectDetection
from arduino.app_bricks.cloud_llm import CloudLLM, CloudModel
from arduino.app_bricks.web_ui import WebUI
from arduino.app_bricks.dbstorage_sqlstore import SQLStore
from arduino.app_utils import *
from datetime import datetime
from time import sleep
import re
import random
import string
import cv2


class ai_lab_assistant():
    def __init__(self, clean_tables=False):
        # Initialize the integrated Cloud LLM management module to utilize the provided Google (Gemini) API key to generate AI-based lab lessons.
        self.llm_gemini = CloudLLM(
            model=CloudModel.GOOGLE_GEMINI,
            system_prompt="You are a lab assistant and must generate HTML pages about the given questions by providing extensive information on the requested subject."
        )
        # Initialize the integrated object detection model classifier instance with video stream (over WebSocket) for the provided Edge Impulse FOMO object detection model to precisely identify lab equipment.
        self.edge_impulse_model = VideoObjectDetection(confidence=0.35, debounce_sec=5)
        # Define the callback function once the provided model detects an equipment.
        self.edge_impulse_model.on_detect_all(lambda detections: self.process_inference_results(detections))
        # Declare and create the SQL database to register user and lesson information.
        self.db = SQLStore("lab_assistant.db")
        # Create the essential database tables. The built-in table creation function checks whether the given table is already exists.
        if(clean_tables):
            self.db.drop_table("account_info")
            self.db.drop_table("lesson_info")
        self.db.create_table("account_info", {"user_id": "INT", "firstname": "TEXT", "lastname": "TEXT", "activation": "TEXT"})
        self.db.create_table("lesson_info", {"question": "TEXT", "equipment": "TEXT", "date": "TEXT", "user_id": "INT", "lesson_id": "TEXT", "filename": "TEXT"})
        # Declare the integrated WebUI Brick class instance to initiate the custom lab assistant web dashboard.
        self.web_ui = WebUI(assets_dir_path="/app/lab_web_dashboard")
        # Listen WebSocket messages from the client (web dashboard) to obtain the latest updates. 
        self.web_ui.on_message("interface_web_control", self.interface_web_control_on_app)
        self.web_ui.on_message("manage_account_actions", self.manage_account_actions_on_app)
        self.web_ui.on_message("save_new_image_sample", self.save_new_image_sample_on_app)
        # Expose REST API endpoints (HTTP GET or POST) to transfer current user account and its associated AI-generated lesson information to all clients, including the web dashboard.
        self.web_ui.expose_api("GET", "/account_lessons", self.update_web_dashboard_with_database_info)        
        # Declare the sensor variables array.
        self.sensor_values = {
                                 "pressure": 0,
                                 "alcohol_concentration": 0,
                                 "weight": 0,
                                 "no2": {"concentration": 0, "board_temp": 0},
                                 "geiger": {"cpm": 0, "nsvh": 0, "usvh": 0},
                                 "gnss": {"date": "", "utc": "", "lat_dir": "", "lon_dir": "", "latitude": 0, "longitude": 0, "altitude": 0, "sog": 0, "cog": 0}
                             }
        # Declare the essential account information holders.
        self.sign_up_account_info = None
        
        # Employ the Arduino Router background Linux service to enable STM32 MCU to borrow and run these functions on Qualcomm MPU.
        Bridge.provide("update_sensor_on_app", self.update_sensor_on_app)
        Bridge.provide("manage_account_actions_on_stm32", self.manage_account_actions_on_stm32)

    # Once the integrated Brick runs an inference with the provided Edge Impulse FOMO object detection model, process the retrieved results to notify the web dashboard accordingly.
    def process_inference_results(self, detections: dict):
        # According to my experiments, I noticed that the built-in detection function sorts the detection results while returning them as a dictionary based on confidence levels. Thus, I was able to get the first dictionary item to transfer the most accurate result once multiple items detected.
        label, result = next(iter(detections.items()))
        confidence = round(result[0]["confidence"], 2)
        # If the current user account is activated, transfer the processed detection result to the web dashboard.
        current_user = self.db.execute_sql("SELECT * FROM account_info WHERE activation = 'activated';")
        if(current_user != None):
            self.web_ui.send_message("latest_obj_detection_result", {"label": label, "confidence": confidence})
    
    # Employ the Google (Gemini) [gemini-2.5-flash] LLM to generate lessons on given lab equipment-related questions and save them as HTML pages.
    def generate_AI_lesson_w_gemini(self, lesson_info):
        retrieved_llm_response = self.llm_gemini.chat("Generate an HTML page on this question: " + lesson_info["question"])
        # Derive only the generated HTML page from the retrieved LLM response.
        processed_llm_response = re.search(r'(<!DOCTYPE html>.*?</html>)', retrieved_llm_response, re.DOTALL)
        if(processed_llm_response):
            # If the provided LLM produces the lesson as an HTML page successfully:
            generated_lesson_html = processed_llm_response.group(1)
            # Generate the unique 5-digit lesson ID.
            unique_lesson_id = ''.join(random.choices(string.ascii_letters + string.digits, k=5))
            # Get the lesson generation date in the required format.
            date = datetime.now().strftime("%m %d, %Y %H:%M:%S")
            # Save the LLM-generated lesson as an HTML file.
            lesson_filename = str(lesson_info["user_id"]) + "_" + lesson_info["equipment"] + "_" + unique_lesson_id + ".html"
            with open("lab_web_dashboard/lessons/"+lesson_filename, "w", encoding="utf-8") as new_lesson:
                new_lesson.write(generated_lesson_html)
            # Register the generated lesson information to the associated database table.
            self.db.execute_sql("INSERT INTO lesson_info (`question`, `equipment`, `date`, `user_id`, `lesson_id`, `filename`) VALUES ('"+lesson_info["question"]+"', '"+lesson_info["equipment"]+"', '"+date+"', "+lesson_info["user_id"]+", '"+unique_lesson_id+"', '"+lesson_filename+"');")
            # Notify the web dashboard accordingly.
            self.web_ui.send_message("generate_ai_lesson_action", {"response": "Google (Gemini) [gemini-2.5-flash] produced the requested lesson successfully!"})
        else:
            self.web_ui.send_message("generate_ai_lesson_action", {"response": "🪐 Google (Gemini) [gemini-2.5-flash] LLM could not generate an appropriately-formatted HTML page. Please try again!"})
        
    
    # Update the incoming sensor variables.
    def update_sensor_on_app(self, p, a, w, n_c, n_b, g_c, g_n, g_u, gn_d, gn_u, gn_lt_d, gn_ln_d, gn_lat, gn_lon, gn_alt, gn_sog, gn_cog):
        # Record the retrieved sensor variables to the associated array.
        self.sensor_values["pressure"] = round(p, 2)
        self.sensor_values["alcohol_concentration"] = round(a, 2)
        self.sensor_values["weight"] = round(w, 2)
        self.sensor_values["no2"]["concentration"] = round(n_c, 2); self.sensor_values["no2"]["board_temp"] = n_b
        self.sensor_values["geiger"]["cpm"] = g_c; self.sensor_values["geiger"]["nsvh"] = g_n; self.sensor_values["geiger"]["usvh"] = g_u
        self.sensor_values["gnss"]["date"] = gn_d; self.sensor_values["gnss"]["utc"] = gn_u; self.sensor_values["gnss"]["lat_dir"] = gn_lt_d; self.sensor_values["gnss"]["lon_dir"] = gn_ln_d; self.sensor_values["gnss"]["latitude"] = round(gn_lat, 4); self.sensor_values["gnss"]["longitude"] = round(gn_lon, 4); self.sensor_values["gnss"]["altitude"] = gn_alt; self.sensor_values["gnss"]["sog"] = gn_sog; self.sensor_values["gnss"]["cog"] = gn_cog
        # Transfer the obtained sensor information to the lab assistant web dashboard via the WebSocket connection.
        self.web_ui.send_message("sensor_values", self.sensor_values)

    # In accordance with the commands transferred by the web dashboard, manage the integrated SQL database, the complementary STM32 microcontroller operations, and the AI-based lesson generation via Gemini.
    # As I needed to maintain chain operations ran by the STM32 microcontroller and the web dashboard sequentially, I split the account actions in multiple functions.
    # START: Account actions.
    def manage_account_actions_on_app(self, sid, data):
        com = data["command"]
        if(com == "signin_user"):
            # Initiate the associated fingerprint sensor task on the STM32 microcontroller via the borrowed function.
            Bridge.call("interface_web_control", "verify_id", int(data["given_user_id"]))
            sleep(1)            
        elif(com == "signup_user"):
            self.sign_up_account_info = data;
            # Initiate the associated fingerprint sensor task on the STM32 microcontroller via the borrowed function.
            Bridge.call("interface_web_control", "register_id", -2)
            sleep(1)
        elif(com == "logout_user"):
            self.db.execute_sql("UPDATE account_info SET activation = 'not_activated' WHERE user_id = "+data["current_user_id"]+";")
        elif(com == "delete_user"):
            self.db.execute_sql("DELETE FROM account_info WHERE user_id = "+data["current_user_id"]+";")
            # Also delete all AI-generated lessons associated to this account.
            self.db.execute_sql("DELETE FROM lesson_info WHERE user_id = "+data["current_user_id"]+";")
        elif(com == "generate_new_ai_lesson"):
            self.generate_AI_lesson_w_gemini(data)

    def manage_account_actions_on_stm32(self, command, provided_user_id):     
        if(command == "signup"):
            if(self.sign_up_account_info == None):
                self.web_ui.send_message("signup_action", {"response": "❌ Python backend did not receive the given user information!"})
            else:
                if(provided_user_id == -1):
                    self.web_ui.send_message("signup_action", {"response": "❌ Fingerprint sensor cannot register!"})
                elif(provided_user_id == -2):
                    self.web_ui.send_message("signup_action", {"response": "🔍 Fingerprint sensor cannot find an available ID!"})
                else:
                    # Create a new user account with the provided user information and the given fingerprint scan ID as the user ID.
                    self.db.execute_sql("INSERT INTO account_info (`user_id`, `firstname`, `lastname`, `activation`) VALUES ("+str(provided_user_id)+", '"+self.sign_up_account_info["firstname"]+"', '"+self.sign_up_account_info["lastname"]+"', 'activated');")
                    self.sign_up_account_info = None
                    self.web_ui.send_message("signup_action", {"response": "New user account successfully created!"})
        elif(command == "signin"):
            if(provided_user_id == -1):
                self.web_ui.send_message("signin_action", {"response": "🔍 The given user ID was not verified by the fingerprint sensor! Try again!"})
            elif(provided_user_id == -2):
                self.web_ui.send_message("signin_action", {"response": "❌ Fingerprint sensor cannot capture fingerprints precisely!"})
            else:
                # Activate the requested account via its verified (matched) user (fingerprint) ID.
                account_check = self.db.execute_sql("SELECT * FROM account_info WHERE user_id = "+str(provided_user_id)+";")
                if(account_check != None):
                    self.db.execute_sql("UPDATE account_info SET activation = 'activated' WHERE user_id = "+str(provided_user_id)+";")
                    self.web_ui.send_message("signin_action", {"response": "Account activated successfully!"})
                else:
                    self.web_ui.send_message("signin_action", {"response": "✍ Given fingerprint belongs to a previously removed account! Please register a new account!"})
                                 
    # END: Account actions.
    
    # Obtain the account activation state. If an account is activated by scanning a registered fingerprint via the onboard capacitive fingerprint sensor, fetch the provided user and generated lesson information.
    # Then, generate HTML elements from the retrieved lesson information to easily update the web dashboard via the exposed API endpoint.
    def update_web_dashboard_with_database_info(self):
        current_user = self.db.execute_sql("SELECT * FROM account_info WHERE activation = 'activated';")
        if(current_user == None):
            html_content = ('<article class="account_notification">'
                            '<h1><span>Sign In: </span>Please utilize the fingerprint sensor to activate your user account!</h1>'
                            '<div>'
                            '<section> <span>User ID</span> <input name="user_id" placeholder="1"></input> </section>'
                            '<section> <span>UNO Q</span> <span id="sign_in_button" class="highlight">Sign In</span> </section>'
                            '</div>'
                            '</article>'
                            '<article class="account_notification">'
                            '<h1><span>Sign Up: </span>Please enter your credentials and register your fingerprint to create a new account!</h1>'
                            '<div>'
                            '<section> <span>Firstname</span> <input name="firstname" placeholder="Kutluhan"></input> </section>'
                            '<section> <span>Lastname</span> <input name="lastname" placeholder="Aktar"></input> </section>'
                            '<section> <span>UNO Q</span> <span id="sign_up_button" class="highlight">Sign Up</span> </section>'
                            '</div>'
                            '</article>'
                           )
            return {"activation": "not_activated", "html_content": html_content}
        else:
            current_user = current_user[0]
            current_user_lessons = self.db.execute_sql("SELECT * FROM lesson_info WHERE user_id = "+str(current_user["user_id"])+";")
            html_content = ('<article class="account_notification" name="logout_section" user_id="'+str(current_user["user_id"])+'">'
                            '<h1><span>Hi, '+current_user["firstname"]+' '+current_user["lastname"]+' 😊</span> ID: ['+str(current_user["user_id"])+']</h1>'
                            '<div>'
                            '<section> <span>UNO Q</span> <span id="logout_button" class="highlight">Logout</span> </section>'
                            '<section> <span>UNO Q</span> <span id="delete_user_button" class="highlight">Delete Account</span> </section>'
                            '</div>'
                            '</article>'
                            )
            if(current_user_lessons != None):
                # Sort the retrieved lessons array based on their creation dates to produce an ordered list from latest to earliest.
                current_user_lessons.sort(key=lambda l: datetime.strptime(l["date"], "%m %d, %Y %H:%M:%S"), reverse=True)
                # Then, proceed generating HTML lesson information cards.
                for index, lesson in enumerate(current_user_lessons):
                    if(index==0): html_content += "<h2>Latest Lesson</h2>"
                    if(index==1): html_content += "<h2>Previous Lessons</h2>"
                    html_lesson = ('<article lesson_filename="lessons/'+lesson["filename"]+'">'
                                   '<h1><span>Q: </span>'+lesson["question"]+'</h1>'
                                   '<div>'
                                   '<section> <span>Date</span> <span>'+lesson["date"]+'</span> </section>'
                                   '<section> <span>User ID</span> <span>'+str(lesson["user_id"])+'</span> </section>'
                                   '<section> <span>Lesson ID</span> <span>'+lesson["lesson_id"]+'</span> </section>'
                                   '<section> <span>Subject</span> <span class="highlight">'+lesson["equipment"].upper()+'</span> </section>'
                                   '</div>'
                                   '</article>'
                                  )
                    html_content += html_lesson
            else:
                html_content += "<h2>No lesson found!</h2>"
            return {"activation": "activated", "firstname": current_user["firstname"], "lastname": current_user["lastname"], "user_id": current_user["user_id"], "html_content": html_content}
    
    # Once the user requests via the web dashboard, obtain the latest captured frame by the USB camera through OpenCV. Then, save the retrieved frame as a new image sample with the given label.
    def save_new_image_sample_on_app(self, sid, data):
        # Stop the Edge Impulse classifier instance to release camera resources.
        self.edge_impulse_model.stop()
        sleep(1)
        # Save the latest generated frame (image) by the USB camera.
        usb_camera_feed = cv2.VideoCapture(0)
        success, latest_frame = usb_camera_feed.read()
        if(success):
            # Get the sample generation date.
            date = datetime.now().strftime("%Y%m%d_%H%M%S")
            sample_file_name = data["sample_label"] + "_" + date + ".jpg"
            cv2.imwrite("ei_model/new_samples/" + sample_file_name, latest_frame)
            # Notify the web dashboard accordingly.
            self.web_ui.send_message("save_sample_result", {"response": "🖼️ New image sample successfully saved! \n\n" + sample_file_name})
        else:
            # Notify the web dashboard accordingly.
            self.web_ui.send_message("save_sample_result", {"response": "❌ Cannot obtain the latest frame produced by the USB camera!"})
        # Release OpenCV camera resources before restarting the classifier instance.
        usb_camera_feed.release()
        sleep(5)
        # Resume the Edge Impulse classifier instance.
        self.edge_impulse_model.start()
        sleep(5)

    # Process the upcoming user commands for the borrowed interface_web_control function, transferred via the built-in WebSocket.
    def interface_web_control_on_app(self, sid, data):
        Bridge.call("interface_web_control", data["command"], data["interface_num"])
        sleep(1)
    
    # If requested, employ the built-in Python terminal to print sensor and app progression updates for debugging.
    def __debug(self, _debug):
        if(_debug):
            print("\n\n/////// Collected Sensor Information ///////\n\n")
            for(main_key, main_value) in self.sensor_values.items():
                if(isinstance(main_value, dict)):
                    for (key, value) in main_value.items():
                        print("{}[{}]: {}\n".format(main_key, key, value))
                else:
                    print("{}: {}\n".format(main_key, main_value))
            print("\n////////////////////////////////////////////////\n\n")
    
    # Declare the main Arduino App application loop.
    def main_loop(self):
        while True:
            # Set 'True' for debugging on the built-in terminal.
            self.__debug(False)
            sleep(10)


# Define the ai_lab_assistant class object.
ai_lab_assistant_obj = ai_lab_assistant();

# Initiate the main Arduino App application loop with the provided function, including the added Bricks.
App.run(user_loop=ai_lab_assistant_obj.main_loop)
