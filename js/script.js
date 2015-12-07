function checkInput() {
    var cardNumberLen = $("#cardNumber").val().length
    var cardNameLen   = $("#cardName"  ).val().length

    if      (cardNumberLen ===  0) { alert("Missing card number" ); return 1; }
    else if (cardNumberLen >=  20) { alert("Card number too long"); return 1; }
    else if (cardNameLen   ===  0) { alert("Missing card name"   ); return 1; }
    else if (cardNameLen   >=  10) { alert("Card name too long"  ); return 1; }

    return 0;
};

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

function updateTable () {
  var table = document.getElementById("tableOfCards");

  // clear the table
  table.innerHTML = "";

  for (var i=0; i<cards.length; i++) {
    var row = table.insertRow();
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    cell1.innerHTML = cards[i].KEY_CARDNAME;
    cell2.innerHTML = cards[i].KEY_CARDNUMBER;
  }
}

document.addEventListener("DOMContentLoaded", function(event) {
  $("#addcard_button").click( function () {
    if (checkInput() === 0) {
      var card = {
                    "KEY_CARDNAME"   : $("#cardName").val(),
                    "KEY_CARDNUMBER" : $("#cardNumber").val()
                  };

      cards.push(card);
      updateTable();
    }
  })

  $("#save_button").click( function() {
    var location = 'pebblejs://close#'+ encodeURIComponent(JSON.stringify(cards));
    console.log(location);
    document.location = location;
  });

  cards = getCards();
  updateTable();
});
