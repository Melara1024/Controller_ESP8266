var joypad = document.getElementById('joypad');
var turnleft = document.getElementById('turnleft');
var turnright = document.getElementById('turnright');

var originx = 0
var originy = 0

var lastx = 0
var lasty = 0

var oldXData;
var nowXData;

var oldYData;
var nowYData;

joypad.addEventListener('touchstart', function(event) {
    joypad.style.backgroundColor = 'red';
    originx = event.changedTouches[0].pageX
    originy = event.changedTouches[0].pageY
}, false);
joypad.addEventListener('touchmove', function(event) {
    event.preventDefault();
    // pageXとかpageYが一定値を超えたとき(10ごと)にのみ送信して頻度を低下させる？
    // 可能なら処理が間に合う一定時間ごとの送信にしたい(5-6msくらい？)

    
    // arduino側の処理頻度を低下させる
    // 最新の値のみをpost状態にしてそれ以外はabortする
    newx = event.changedTouches[0].pageX
    newy = event.changedTouches[0].pageY

    oldXData.abort()
    oldYData.abort();

    oldXData = nowXData;
    oldYData = nowYData;

    if (Math.abs(newx - lastx) > 16){
        nowXData = $.ajax({url: "/joyEvent?px="+newx - originx, success: function(result){
        $("#div1").html(result);}});
        lastx = newx
    }
    if (Math.abs(newy - lasty) > 16){
        nowYData = $.ajax({url: "/joyEvent?py="+newy - originy, success: function(result){
        $("#div1").html(result);}});
        lasty = newy
    }
}, false);
joypad.addEventListener('touchend', function(event) {
    joypad.style.backgroundColor = 'blue';
    originx = 0
    originy = 0
    $.ajax({url: "/joyEvent?px=0", success: function(result){
    $("#div1").html(result);}});
    $.ajax({url: "/joyEvent?py=0", success: function(result){
    $("#div1").html(result);}});
}, false);

turnleft.addEventListener('touchstart', function(event) {
    turnleft.style.backgroundColor = 'red';
    $.ajax({url: "/leftEvent?l=1", success: function(result){
    $("#div1").html(result);}});
}, false);
turnleft.addEventListener('touchmove', function(event) {
    event.preventDefault();
}, false);
turnleft.addEventListener('touchend', function(event) {
    turnleft.style.backgroundColor = 'yellow';
    $.ajax({url: "/leftEvent?l=0", success: function(result){
        $("#div1").html(result);}});
}, false);

turnright.addEventListener('touchstart', function(event) {
    turnright.style.backgroundColor = 'red';
    $.ajax({url: "/rightEvent?r=1", success: function(result){
    $("#div1").html(result);}});
}, false);
turnright.addEventListener('touchmove', function(event) {
    event.preventDefault();
}, false);
turnright.addEventListener('touchend', function(event) {
    turnright.style.backgroundColor = 'orange';
    $.ajax({url: "/rightEvent?r=0", success: function(result){
        $("#div1").html(result);}});
}, false);