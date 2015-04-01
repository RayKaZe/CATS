$(function() {
    var App = {
        init: function() {
            App.attachListeners();
        },
        config: {
            reader: "code_128"
        },
        attachListeners: function() {
            $(".barcode input[type=file]").on("change", function(e) {
                if (e.target.files && e.target.files.length) {
                    App.decode(URL.createObjectURL(e.target.files[0]));
                }
            });
        },
        detachListeners: function() {
            $(".barcode input[type=file]").off("change");        },
        decode: function(src) {
            Quagga.decodeSingle({
                decoder: {
                    readers : [App.config.reader + '_reader']
                },
                locate : true,
                src : src
            }, function(result) {});
        },
        selectCodeListener: function(event) {
            event.preventDefault();
            $('#cardNumber').val(event.data);
        }
    };

    App.init();

    Quagga.onProcessed(function(result) {
        var drawingCtx = Quagga.canvas.ctx.overlay,
            drawingCanvas = Quagga.canvas.dom.overlay;

        if (result) {
            if (result.boxes) {
                drawingCtx.clearRect(0, 0, parseInt(drawingCanvas.getAttribute("width")), parseInt(drawingCanvas.getAttribute("height")));
                result.boxes.filter(function (box) {
                    return box !== result.box;
                }).forEach(function (box) {
                    Quagga.ImageDebug.drawPath(box, {x: 0, y: 1}, drawingCtx, {color: "green", lineWidth: 2});
                });
            }

            if (result.box) {
                Quagga.ImageDebug.drawPath(result.box, {x: 0, y: 1}, drawingCtx, {color: "#00F", lineWidth: 2});
            }

            if (result.codeResult && result.codeResult.code) {
                Quagga.ImageDebug.drawPath(result.line, {x: 'x', y: 'y'}, drawingCtx, {color: 'red', lineWidth: 3});
            }
        }
    });

    Quagga.onDetected(function(result) {
        var code = result.codeResult.code,
            clubnumber,
            $node,
            canvas = Quagga.canvas.dom.image;

        clubnumber = "63400" + code.slice(3);

        $node = $('<div>Found these clubcards:</div><li><h4 class="code"><a href="#"></a> <small></small></h4></li>');
        $node.find("h4.code a").html(clubnumber);
        $node.find("h4.code small").html("("+code+")");
        $node.find("h4.code a").click(code, App.selectCodeListener);
        $("#baroutput").prepend($node);
    });
});