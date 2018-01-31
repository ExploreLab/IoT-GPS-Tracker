# IoT-GPS-Tracker
This is article is GPS Tracker module using GY-NEO6MV2 NEO-6M GPS Module to scan Location and then nodeMCU will send data to Firebase for the map using Google MAP API and shown in the Web application.

Full Tutorial: https://myelectronicslab.com/gps-tracker/

Result: https://iot-gps-tracker.firebaseapp.com/

Dev: Jukrapun Chitpong

![Alt Text](https://firebasestorage.googleapis.com/v0/b/iot-gps-tracker.appspot.com/o/IoT_GPS_Diagram.jpg?alt=media&token=227d625b-37ca-451a-a0d0-7c0e63c43bd7)

![Alt Text](https://firebasestorage.googleapis.com/v0/b/iot-gps-tracker.appspot.com/o/IoT_GPS_055.jpg?alt=media&token=d6ce1c8a-df89-4ae8-b90d-178e5ff7fca9)

## Hardware Needed


NodeMCU or ESP8166-12E

GPS Module

USB Data Cable

## Software Needed


Web browser (Google Chrome)

Sublime

Arduino IDE (We using Arduino IDE version 1.8.3)

### Hardware Section.
NodeMCU  >> GPS Module

GPIO12 (D6) >> Tx PIN

GPIO13 (D7) >> Rx PIN

### Software Section.

#### 1. Firebase

1.1 Sign in to Firebase

1.2 Create Project

#### 2. Arduino

2.1 Open file "ESP_GPS.ino" and then Change YOUR SSID and PASS.

2.2 Change
- FirebaseIO [Database ]
- Database Secrets [Project  Settings > SERVICE ACCOUNTS > Database Secrets]

#### 3. Google MAP & FIREBASE
3.1 Google Maps APIs
- Create KEY Google Maps APIs 
- Open files "index.html" (We using Sublime Text)
- Copy KEY and Paste instead of  "YOUR_API_KEY " into your application HTML.
  ##### ref. https://developers.google.com/maps/documentation/javascript/signedin
  
3.2 Add Firebase to your JavaScript Project
- Create a Firebase project in the Firebase.
- Click Add Firebase to your web app.
- Copy script and Paste instead of  initialize Firebase script into your application HTML.
  Click Copy, then paste the code snippet into your application HTML.

3.3 Deploy Your Site.

Install the Firebase CLI
- install Firebase command line tools using npm (Node.js)
##### npm install -g firebase-tools
Initializing your site
Open a terminal window and navigate to or create a directory for your site
- Sign in to Google and Initiate your project
##### firebase login 
##### firebase init

Deploying your site
Add your static files to your deploy directory (the default is public)
- Deploy your website
##### firebase deploy
#### ref. https://firebase.google.com/docs/hosting/deploying

### Finally
##### https://iot-gps-tracker.firebaseapp.com

[![](http://img.youtube.com/vi/54FfeH21ptk/0.jpg)](http://www.youtube.com/watch?v=54FfeH21ptk "IoT-GPS-Tracker")

