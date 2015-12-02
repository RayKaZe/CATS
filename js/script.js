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

function getCards() {
  var val    = "cards",
      result = "Not found",
      tmp    = [];
  location.search.substr(1)
    .split("&")
    .forEach(function (item) {
      tmp = item.split("=");
      if (tmp[0] === val)
        result = decodeURIComponent(tmp[1]);
  });

  result = decodeURIComponent(result);
  result = JSON.parse(result)
  return result;
}

document.addEventListener("DOMContentLoaded", function(event) {
  var j = getCards();
  var table = document.getElementById("tableOfCards");

  for (var i=0; i<j.length; i++) {
    var row = table.insertRow();
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    cell1.innerHTML = j[i].KEY_CARDNAME;
    cell2.innerHTML = j[i].KEY_CARDNUMBER;
  }
});