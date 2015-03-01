function checkInput() {
    if (document.getElementById("cardNumber").value.length % 2 !==  0) { return 1; }
    else if (document.getElementById("cardNumber").value.length ===  0) { return 1; }
    else if (document.getElementById("cardNumber").value.length >=  20) { return 1; }
    else if (document.getElementById("cardName").value.length ===  0) { return 1; }
    else if (document.getElementById("cardName").value.length >=  10) { return 1; }
    else { return 0; }
};

function saveOptions() {
    var cardName = document.getElementById("cardName");
    var cardNumber = document.getElementById("cardNumber");
    var options = {"name": cardName.value.concat("\0"), "number": cardNumber.value.concat("\0")};
    return options;
};
   
var submitButton = document.getElementById("addcard_button");
submitButton.addEventListener("click", 
  function() {
    if (checkInput() === 1) {
      alert("Incorrect card number")
      return;
    } else {
    var options = saveOptions();
    var location = 'pebblejs://close#'+ encodeURIComponent(JSON.stringify(options));
    document.location = location;
    }
  }, 
false);