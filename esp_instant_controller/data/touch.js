var joypad = document.getElementById('joypad');
var turnleft = document.getElementById('turnleft');
var turnright = document.getElementById('turnright');

var originx = 0
var originy = 0

var t;

joypad.addEventListener('touchstart', function(event) {
    joypad.style.backgroundColor = 'silver';
    originx = event.changedTouches[0].pageX;
    originy = event.changedTouches[0].pageY;
}, false);
joypad.addEventListener('touchmove', function(event) {
    event.preventDefault();
    if (performance.now() - t < 200 && performance.now() - t  > 0) return;
    
    var px = event.changedTouches[0].pageX - originx;
    var py = event.changedTouches[0].pageY - originy;

    $.ajax({url: "/joyEvent?px="+px, success: function(result){
    $("#div1").html(result);}});

    $.ajax({url: "/joyEvent?py="+py, success: function(result){
    $("#div1").html(result);}});

    t = performance.now();

}, false);
joypad.addEventListener('touchend', function(event) {
    joypad.style.backgroundColor = 'aquamarine';
    originx = 0;
    originy = 0;
    $.ajax({url: "/joyEvent?px=0", success: function(result){
    $("#div1").html(result);}});
    $.ajax({url: "/joyEvent?py=0", success: function(result){
    $("#div1").html(result);}});
}, false);

turnleft.addEventListener('touchstart', function(event) {
    turnleft.style.backgroundColor = 'silver';
    $.ajax({url: "/leftEvent?l=1", success: function(result){
    $("#div1").html(result);}});
}, false);
turnleft.addEventListener('touchmove', function(event) {
    event.preventDefault();
}, false);
turnleft.addEventListener('touchend', function(event) {
    turnleft.style.backgroundColor = 'aquamarine';
    $.ajax({url: "/leftEvent?l=0", success: function(result){
        $("#div1").html(result);}});
}, false);

turnright.addEventListener('touchstart', function(event) {
    turnright.style.backgroundColor = 'silver';
    $.ajax({url: "/rightEvent?r=1", success: function(result){
    $("#div1").html(result);}});
}, false);
turnright.addEventListener('touchmove', function(event) {
    event.preventDefault();
}, false);
turnright.addEventListener('touchend', function(event) {
    turnright.style.backgroundColor = 'aquamarine';
    $.ajax({url: "/rightEvent?r=0", success: function(result){
        $("#div1").html(result);}});
}, false);