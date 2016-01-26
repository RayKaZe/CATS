"use strict";

function checkInput(cards) {
  cards.forEach(function(card) {
    var cardNumberLen = card["KEY_CARDNUMBER"].length;
    var cardNameLen   = card["KEY_CARDNAME"].length;
    if      (cardNumberLen ===  0) { alert("Missing card number" ); return 1; }
    else if (cardNumberLen >=  20) { alert("Card number too long"); return 1; }
    else if (cardNameLen   ===  0) { alert("Missing card name"   ); return 1; }
    else if (cardNameLen   >=  10) { alert("Card name too long"  ); return 1; }
  });
  return 0;
}

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
  result = JSON.parse(result);
  return result;
}

function populateList(cards) {
  var list = document.getElementById("card-list");

  for (var i=0; i<cards.length; i++) {
    var cardName = cards[i].KEY_CARDNAME;
    var cardNumber = cards[i].KEY_CARDNUMBER;
    var cardString = cardName + ":" + cardNumber;
    var newLabel = "<div class=\"item\">" + cardString + "<div class=\"delete-item\"></div></div>";
    list.innerHTML = newLabel + list.innerHTML;
  }

  if (cards.length === 0) {
    $("#debug-paragraph").html("No cards on watch");
  }
}

// Get query variables
function getQueryParam(variable, defaultValue) {
  // Find all URL parameters
  var query = location.search.substring(1);
  console.log("query", query);
  var vars = query.split('&');
  console.log("vars", vars)
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');

    // If the query variable parameter is found, decode it to use and return it for use
    if (pair[0] === variable) {
      var url = decodeURIComponent(pair[1]);
      return decodeURIComponent(url);
    }
  }
  return defaultValue || false;
}

function parseHtmlList() {

  var list = document.getElementById("card-list");
  var listItems = $("#card-list").children();

  // Remove "add one more..." object
  // var addOneMoreIndex = listItems.length - 1;
  // delete listItems[addOneMoreIndex];

  var cards = [];
  for (var i = 0; i < listItems.length - 1; i++) {
    var cardDetails = listItems[i].innerText.split(":");
    var card = {
      "KEY_CARDNAME": cardDetails[0],
      "KEY_CARDNUMBER": cardDetails[1]
    };
    cards.push(card);
  }

  return cards;
}

document.addEventListener("DOMContentLoaded", function(event) {
  $("#save_button").click( function() {
    // Set the return URL depending on the runtime environment
    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    console.log(return_to);

    var cards = parseHtmlList();
    checkInput(cards);

    var location = return_to + encodeURIComponent(JSON.stringify(cards));
    console.log(location);
    document.location = location;
  });

  // This gets round the dynamically added list elements not being bound to Slate.js
  $(document).on("click", ".delete-item", function() {
    $(this).parent().remove();
  });

  var cards = getCards();
  populateList(cards);
});
