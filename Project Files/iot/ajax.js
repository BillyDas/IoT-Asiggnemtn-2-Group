

setInterval(() => {
    $.ajax({url: "CurrentTemp.php"}).done((output) => {
        $("#CurrentTemp").html(output);
    });
}, 500);

setInterval(() => {
    $.ajax({url: "FanStatus.php"}).done((output) => {
        $("#FanStatus").html(output);
    });
}, 500);