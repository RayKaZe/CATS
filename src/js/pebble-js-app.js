Pebble.addEventListener("ready", function (e) {
    console.log("PebbleKit JS ready! what what");
    sync_database();
});

// Settings button in Pebble app
Pebble.addEventListener("showConfiguration", function (e) {
    DEBUG = 0;
    //Load the remote config page
    if (DEBUG) {
      base_url = "localhost:8000/"
    } else {
      base_url = "http://catsconfig.tk/"
    }

    Pebble.openURL(base_url+"index.html?cards="+JSON.stringify(cards));
});

// Configuration page closes
Pebble.addEventListener("webviewclosed", function (e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));

    cards = configuration

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

function sync_database () {
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
    console.log("Getting", msg.GET_NTH_ENTRY, "th entry");
    Pebble.sendAppMessage(
      msg,
      function (e) { console.log("Getting", msg.GET_NTH_ENTRY, "th entry"); },
      function (e) { console.log("Getting", msg.GET_NTH_ENTRY, "entry failed"); }
    );
}

Pebble.addEventListener("appmessage", function(e) {
  var dict = e.payload;
  console.log(JSON.stringify(dict));

  if (dict.NUM_ENTRIES!=undefined) {
    n = parseInt(dict.NUM_ENTRIES);
    if (n>0) {
      cards = []
      get_nth_entry(1);
    }
  } else if (dict.GET_NTH_ENTRY != undefined) {
    cards.push(dict)
    dict.GET_NTH_ENTRY
    if (dict.GET_NTH_ENTRY < n) {
      get_nth_entry(dict.GET_NTH_ENTRY+1);
    }
    console.log("change something on the html");
  } else if (dict.CLEAR_PERSIST === 1) {
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
