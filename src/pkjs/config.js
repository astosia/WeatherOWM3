// Clay Config: see https://github.com/pebble/clay
module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Theme settings"
      },
      {
        "type": "select",
        "messageKey": "ALIEN",
        "defaultValue": 1,
        "label": "Theme/Alien Colour",
        "options": [
          {
            "label": "White",
            "value": 1
          },
          {
            "label": "Blue and Black",
            "value": 2
          }
        ]
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
        "label": "Time Colour"
      },
      {
        "type": "color",
        "messageKey": "Text3Color",
        "defaultValue": "0x000000",
        "label": "Day of the week Colour"
      },
      {
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
      },
      {
        "type": "color",
        "messageKey": "FrameColor1",
        "defaultValue": "0xFFFFFF",
        "label": "Divider Bar Colour"
      },
       {
        "type": "color",
        "messageKey": "Text2Color",
        "defaultValue": "0x000000",
        "label": "Battery Bar Colour"
      },
      {
        "type": "color",
        "messageKey": "Text4Color",
        "defaultValue": "0x000000",
        "label": "Quiet Time & BT icon colour"
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
