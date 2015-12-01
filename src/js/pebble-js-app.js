Pebble.addEventListener("ready", function (e) {
    console.log("PebbleKit JS ready! what what");
    sync_database();
});

// Settings button in Pebble app
Pebble.addEventListener("showConfiguration", function (e) {
    //Load the remote config page
    Pebble.openURL("http://catsconfig.tk/index.html?cards="+JSON.stringify(cards));
});

// Configuration page closes
Pebble.addEventListener("webviewclosed", function (e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));

    //Send to Pebble, persist there
    var message = {
      "KEY_CARDNAME"   : configuration.name,
      "KEY_CARDNUMBER" : configuration.number
    };

    console.log(JSON.stringify(message));

    Pebble.sendAppMessage(
      message,
      function(e) {
        console.log("Sending settings data...");
      },
      function(e) {
        console.log("Settings feedback failed!");
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

cards = []

Pebble.addEventListener("appmessage", function(e) {
  var dict = e.payload;
  console.log(JSON.stringify(dict));

  if (dict.NUM_ENTRIES!=undefined) {
    n = parseInt(dict.NUM_ENTRIES);
    if (n>0) {
      get_nth_entry(1);
    }
  } else if (dict.GET_NTH_ENTRY != undefined) {
    cards.push(dict)
    dict.GET_NTH_ENTRY
    if (dict.GET_NTH_ENTRY < n) {
      get_nth_entry(dict.GET_NTH_ENTRY+1);
    }
    console.log("change something on the html");
  }
});
