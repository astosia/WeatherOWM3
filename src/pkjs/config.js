// Clay Config: see https://github.com/pebble/clay
module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
  "type": "heading",
  "defaultValue": "Weather will not show unless a weather provider is selected and set up.",
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
        "type": "toggle",
        "messageKey": "ALIEN",
        "defaultValue": false,
        "label": "Light/Dark Alien Theme"
      /*  "options": [
          {
            "label": "White",
            "value": "w"
          },
          {
            "label": "Blue and Black",
            "value": "b"
          }
        ]*/
      },
    /*  {
        "type": "color",
        "messageKey": "Back1Color",
        "defaultValue": "0x000000",
        "label": "Background Colour"
      },*/

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
                "messageKey": "WeatherUnit",
                "label": "Temperature in Fahrenheit",
                "defaultValue": false,
              },
              {
                "type": "select",
                "messageKey": "WeatherProv",
                "defaultValue": "owm",
                "label": "Weather Provider",
                "options": [
                  {
                    "label": "OpenWeatherMap",
                    "value": "owm"
                  },
                  {
                    "label": "DarkSky",
                    "value": "ds"
                  }
                ]
              },
              {
                 "type": "input",
                 "messageKey": "Lat",
                 "defaultValue": "",
                 "label": "Manual Location - Latitude",
                 "attributes": {
                 "placeholder": "eg: 51.4962"
                 }
               },
               {
                  "type": "input",
                  "messageKey": "Long",
                  "defaultValue": "",
                  "label": "Manual Location - Longitude",
                  "description": "Leave both blank to use GPS location for weather. You can use <a href =https://www.google.com/maps>Google Maps</a> or <a href =https://www.openstreetmap.org/>OpenStreetMap</a> to find latitude & longitude.",
                  "attributes": {
                    "placeholder": "eg: -0.0989"
                  }
                },
             {
                "type": "input",
                "messageKey": "APIKEY_User",
                "defaultValue": "",
                "label": "API Key",
                "description": "If left blank, the watch will attempt to request an api from your pmkey.xyz.  If you don't have an api key, weather data will not be displayed. Existing DarkSky APIs will work, but you can no longer sign up for a new one. You can still register for a free personal API key for <a href =https://home.openweathermap.org/users/sign_up/>OpenWeatherMap here</a>.",
                "attributes": {
                  "placeholder": "Paste your API Key here"
                }
              },
             {
                "type": "input",
                "messageKey": "EmailPMKEY",
                "defaultValue": "",
                "label": "pmkey.xyz User",
                "description": "pmkey.xyz is a free service for Pebble users that allows you to safely store all your API keys in a single place. Check it out and sign up at <a href=https://www.pmkey.xyz/>pmkey.xyz</a>.",
                "attributes": {
                  "placeholder": "eg: jane.smith@pmkey.xyz",
                  "type": "email"
                }
              },
              {
                "type": "input",
                "messageKey": "PINPMKEY",
                "defaultValue": "",
                "label": "pmkey.xyz PIN",
                "attributes": {
                  "placeholder": "eg: 12345"
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
          "defaultValue": "version v1.0",
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
