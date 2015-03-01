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
    var options = [cardName.value, cardNumber.value];
    return options;
};
   
var submitButton = document.getElementById("addcard_button");
submitButton.addEventListener("click", 
  function() {
    if (checkInput() === 1) {
      alert("Incorrect card number")
      return;
    } else {
      // check for/open current CSV
      var allText =[];
      //var Lines = [];
      var txtFile = new XMLHttpRequest();
      txtFile.open("GET", "http://warwickengineers.co.uk/newCards.csv", true);
      txtFile.onreadystatechange = function() {
          allText = txtFile.responseText;
      }
      // CSV shizzle
      var options = saveOptions();
      var data = "data:text/csv;charset=utf-8,";
      data = options.join(",");
      csvContent = allText + "\n" + data;
      alert(csvContent)
      txtFile.write(csvContent);
      alert(txtFile);
      


    }
  }, 
false);