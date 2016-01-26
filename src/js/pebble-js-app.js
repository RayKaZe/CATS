"use strict";

var DEBUG = false;
var cards = [];
var n = 0;

Pebble.addEventListener("ready", function (e) {
    console.log("PebbleKit JS ready!");
    sync_database();
});

// Settings button in Pebble app
Pebble.addEventListener("showConfiguration", function (e) {
    var base_url = "https://raykaze.github.io/CATS/";
    if (DEBUG) base_url = "localhost:8000/";

    console.log("In cards array when settings opened:");
    console.log(JSON.stringify(cards));

    Pebble.openURL(base_url+"index.html?cards="+JSON.stringify(cards));
});

// Configuration page closes
Pebble.addEventListener("webviewclosed", function (e) {
    if (!e.response) {
      console.log("Cancel button pressed");
      return;
    }

    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));

    cards = configuration;

    // clear data on watch
    Pebble.sendAppMessage(
      {CLEAR_PERSIST: true},
      function(e) {
        console.log("acked");
      },
      function(e) {
        console.log("nacked");
      }
    );
});

// Runs on PebbleKit JS ready
function sync_database () {
  console.log("Sync Database");
  var message = {"NUM_ENTRIES": true};
  Pebble.sendAppMessage(
    message,
    function (e) {
      console.log("Sending settings data...");
    },
    function (e) {
      console.log("Settings feedback failed!");
    }
  );
}

function get_nth_entry(i) {
    var msg = {GET_NTH_ENTRY: i};
    console.log("Getting entry " + msg.GET_NTH_ENTRY);
    Pebble.sendAppMessage(
      msg,
      function (e) { console.log("Got entry " + msg.GET_NTH_ENTRY); },
      function (e) { console.log("Getting entry " + msg.GET_NTH_ENTRY + " failed"); }
    );
}

// The watch sent an AppMessage to PebbleKit JS
Pebble.addEventListener("appmessage", function(e) {
  var dict = e.payload;
  console.log("AppMessage Payload: " + JSON.stringify(dict));

  // If any entries to get, start with the 1st
  if (dict.NUM_ENTRIES !== undefined) {
    n = parseInt(dict.NUM_ENTRIES);
    if (n > 0) {
      cards = [];
      get_nth_entry(1);
    }
  }
  // Push a specific entry to cards array
  else if (dict.GET_NTH_ENTRY !== undefined) {
    console.log("Entry " + dict.GET_NTH_ENTRY + " contains " + dict);
    cards.push(dict);
    // If more entries to get
    if (dict.GET_NTH_ENTRY < n) {
      get_nth_entry(dict.GET_NTH_ENTRY + 1);
    }
    console.log("change something on the html");
  }
  else if (dict.CLEAR_PERSIST === 1) {
    sendNextMessage();
  }
});

function sendNextMessage() {
  if (cards.length === 0) return;

  var entry = cards.shift();

  Pebble.sendAppMessage(
    {
      KEY_CARDNAME   : entry.KEY_CARDNAME,
      KEY_CARDNUMBER : entry.KEY_CARDNUMBER
    },
    function (e) {
      sendNextMessage();
    },
    function (e) {
      cards.unshift(entry);
      sendNextMessage();
    }
  );
}
