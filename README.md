<img width="1249" height="578" alt="image" src="https://github.com/user-attachments/assets/918e55e3-e3ae-41e0-8a5f-60ba83b0c09d" />
💡 How it works

The ESP32 connects to WiFi and keeps running in the background.
When the button is pressed:

📸 It takes a picture using the camera
📍 Uses the latest received latitude & longitude
🔗 Creates a Google Maps link
📧 Sends an email with the image + location to multiple people

Location is updated through a simple web request, so you can connect it with a mobile app or browser.

✨ Features
One-click emergency alert
Real-time image capture
Live location sharing (Google Maps link)
Email notification to multiple recipients
Lightweight and low-cost hardware setup
🛠️ Built with
ESP32 Camera
Arduino (C++)
WiFi + Web Server
SMTP (Gmail using ESP Mail Client)
🚀 Why this project

The goal of this project is to create a low-cost safety device that anyone can use. It can be useful for:

Personal safety
Women safety systems
Emergency alerts in remote areas
Student or beginner IoT projects
