Pebble.addEventListener("ready",
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

// Settings button in Pebble app
Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    Pebble.openURL("http://catsconfig.tk");
  }
);

// Configuration page closes
Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));

    //Send to Pebble, persist there
    var message = {"KEY_CARDNAME": configuration.name, "KEY_CARDNUMBER": configuration.number};
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
  }
);