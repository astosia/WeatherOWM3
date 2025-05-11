// Clay Config: see https://github.com/pebble/clay
module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
  "type": "heading",
  "defaultValue": "Weather will not show unless weather provider is set up.",
  "size":6
  },
  {
  "type": "heading",
  "defaultValue": "Set complications to the same colour as the background to hide them.",
  "size":6
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Theme settings"
      },
      {
        "type": "color",
        "messageKey": "Back1Color",
        "defaultValue": "0x000000",
        "label": "Background Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "HourColor",
        "defaultValue": "0x000000",
        "label": "Time Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text3Color",
        "defaultValue": "0x000000",
        "label": "Date & Temperature Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text7Color",
        "defaultValue": "0x000000",
        "label": "Weather Icon Colour",
        "allowGray":true
      },
/*      {
        "type": "color",
        "messageKey": "Text5Color",
        "defaultValue": "0x000000",
        "label": "Month Colour"
      },
      {
        "type": "color",
        "messageKey": "Text6Color",
        "defaultValue": "0x000000",
        "label": "Date Text Colour"
      },*/
      {
        "type": "color",
        "messageKey": "FrameColor1",
        "defaultValue": "0xFFFFFF",
        "label": "Divider Bar Colour",
        "allowGray":true
      },
       {
        "type": "color",
        "messageKey": "Text2Color",
        "defaultValue": "0x000000",
        "label": "Battery Bar Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text4Color",
        "defaultValue": "0x000000",
        "label": "Quiet Time & BT icon colour"
      },
       {
        "type": "section",
        "items": [
              {
                "type": "heading",
                "defaultValue": "Weather settings"
              },
              {
                "type": "slider",
                "messageKey": "UpSlider",
                "defaultValue": 30,
                "label": "Weather update frequency (minutes)",
                "description": "More frequent requests will drain your phone battery more quickly",
                "min": 15,
                "max": 120,
                "step": 15
              },
              {
                "type": "toggle",
                "messageKey": "CurrentOrFore",
                "label": "Current/Forecast conditions",
                "description": "Shows weather based on either current (toggle is off) or forecast (toggle is on) conditions",
                "defaultValue": false,
              },
              {
                "type": "toggle",
                "messageKey": "WeatherUnit",
                "label": "Temperature in Celcius/Fahrenheit",
                "description": "toggle off for C, on for F",
                "defaultValue": false,
              },
              {
                "type": "select",
                "messageKey": "WeatherProv",
                "defaultValue": "ds",
                "label": "Weather Provider",
                "options": [
                  {
                    "label": "Open-Meteo",
                    "value": "ds"
                  },
                  {
                    "label": "OpenWeatherMap",
                    "value": "owm"
                  }
                ]
              },
              {
                 "type": "input",
                 "messageKey": "Lat",
                 "defaultValue": "",
                 "label": "Manual Location - Latitude",
                 "attributes": {
                 "placeholder": "eg: 51.4962 (leave blank to use GPS)"
                 }
               },
               {
                  "type": "input",
                  "messageKey": "Long",
                  "defaultValue": "",
                  "label": "Manual Location - Longitude",
                  "description": "Leave both blank to use GPS location for sunrise & sunset times and weather. You can use <a href =https://www.google.com/maps>Google Maps</a> or <a href =https://www.openstreetmap.org/>OpenStreetMap</a> to find latitude & longitude.",
                  "attributes": {
                    "placeholder": "eg: -0.0989 (leave blank to use GPS)"
                  }
                },
                {
                   "type": "input",
                   "messageKey": "APIKEY_User",
                   "defaultValue": "",
                   "label": "API Key",
                   "description": "Weather data uses Open-Meteo by default which does not require an API key.  If you prefer OpenWeatherMap, you can <a href =https://home.openweathermap.org/users/sign_up/>register for a free personal API key here</a>.",
                   "attributes": {
                     "placeholder": "Paste OpenWeatherMap API Key, leave blank for Open-Meteo"
                   }
                 },
            ]
              },
          {
          "type": "submit",
          "defaultValue":"SAVE"
          },
          {
          "type": "heading",
          "defaultValue": "version v1.6",
          "size":6
          },
          {
          "type": "heading",
          "defaultValue": "Made in UK",
          "size":6
          }
       ]
      },
      ];
