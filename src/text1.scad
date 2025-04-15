use <C:/Users/rolan/Downloads/Atarian/SF Atarian System.ttf>


//shape1();
atari();

module shape1(){
    difference() {
        base_shape();
        atari();
    }
}
module base_shape(){
    //  rotate([0,0,180])
    linear_extrude(1)
        square([45,20]);
}

module atari() {
    translate([3,18,0])
    //rotate([180,180, 0])
    mirror([0,1,0])
    linear_extrude(0.4)
    text("ATARI", 16, "SF Atarian System:style=Regular", direction="ltr");
}
