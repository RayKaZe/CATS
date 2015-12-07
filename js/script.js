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
  table.innerHTML = ""; // clear the table

  for (var i=0; i<cards.length; i++) {
    var row = table.insertRow();
    var cell0 = row.insertCell(0);
    var cell1 = row.insertCell(1);
    var cell2 = row.insertCell(2);
    cell0.innerHTML = cards[i].KEY_CARDNAME;
    cell1.innerHTML = cards[i].KEY_CARDNUMBER;
    cell2.innerHTML = "<span class=\"glyphicon glyphicon-remove\"></span>";

    cell2.onclick = function(event){
      event.path.find( function (elem, ind, arr) {
        if (elem.tagName === "TR") {
          var i = elem.rowIndex;
          console.log(i);
          cards.splice(i, 1);
          updateTable();
        }
      });
    };
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

      // clear the input boxes
      $("#cardName").val("");
      $("#cardNumber").val("");

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
