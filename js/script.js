function checkInput() {
    //if (document.getElementById("cardNumber").value.length % 2 !==  0) { return 1; }
    if (document.getElementById("cardNumber").value.length ===  0) { alert("Missing card number"); return 1; }
    else if (document.getElementById("cardNumber").value.length >=  20) { alert("Card number too long"); return 1; }
    else if (document.getElementById("cardName").value.length ===  0) { alert("Missing card name"); return 1; }
    else if (document.getElementById("cardName").value.length >=  10) { alert("Card name too long"); return 1; }
    else { return 0; }
};

function saveOptions() {
    var cardName = document.getElementById("cardName");
    var cardNumber = document.getElementById("cardNumber");
    var zeros1 = 11 - cardName.value.length;
    var zeros2 = 11 - cardNumber.value.length;
    // loads of nulls go here???
    var options = {"name": cardName.value, "number": cardNumber.value};
    return options;
};
   
var addcardButton = document.getElementById("addcard_button");
addcardButton.addEventListener("click", 
  function() {
    if (checkInput() === 1) {
      return;
    } else {
      var options = saveOptions();
      var location = 'pebblejs://close#'+ encodeURIComponent(JSON.stringify(options));
      document.location = location;
    }
  }, 
false);